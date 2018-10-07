/* 
   Feet of Fury
   
   drawables/cdtray.c
   
   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "cdtray.h"

// Center of the circle -- this is dependent on the graphic
const float CDTray::m_cx = 493.0f, CDTray::m_cy = 454.0f, CDTray::m_r = 497.0f - 64.0f;

/* Math derivation for this is as follows:
   d = 128
   c = 2*PI*r
   #segs = c/d
   per = (2*PI)/#segs
       = (2*PI)/(c/d)
       = (2*PI*d)/c
       = (2*PI*d)/(2*PI*r)
       = d/r
 */
const float CDTray::m_per = 128.0f / CDTray::m_r;

CDTray::CDTray(int cnt, RefPtr<Texture> *txrs, Texture * shadow,
	Texture * shiny[2])
{
	m_frame = 0;
	m_cnt = cnt;

	m_cdrot = new int[cnt];
	for (int i=0; i<cnt; i++)
		m_cdrot[i] = 0;

	assert(txrs != NULL);
	m_textures = new RefPtr<Texture>[cnt];
	for (int i=0; i<cnt; i++) {
		assert(txrs[i] != NULL);
		m_textures[i] = txrs[i];
		m_textures[i]->setUVClamp(Texture::UVClamp, Texture::UVClamp);
	}
	m_txr_shiny[0] = shiny[0];
	m_txr_shiny[0]->setUVClamp(Texture::UVClamp, Texture::UVClamp);
	m_txr_shiny[1] = shiny[1];
	m_txr_shiny[1]->setUVClamp(Texture::UVClamp, Texture::UVClamp);
	
	assert(shadow != NULL);
	m_txr_shadow = shadow;
	m_txr_shadow->setUVClamp(Texture::UVClamp, Texture::UVClamp);
	
	m_selected = 0; m_last_sel = -1;
	m_move_dir = 0;
	m_move_cur = 0.0f;
}

CDTray::~CDTray() {
	delete[] m_textures;
	delete[] m_cdrot;
}

void CDTray::drawOne(int idx, float x, float y, float z, bool selected, bool shadow) {
	uint32 color;
	
	/* if (selected) {
		float r = fsin(m_frame * (60.0f/hz) * 2 * M_PI / 45.0f) / 4.0f + 0.75f;
		color = plx_pack_color(r, r, 1, 1);
	} else {
		color = plx_pack_color(1, 1, 1, 1);
	} */
	color = getColor();

	if (!selected) {
		plx_spr_inp(128, 128, x, y, z, color);
		return;
	}

	// 1 2
	// 3 4
	float size14 = 64.0f;
	float size23 = 64.0f;
	bool inv = false;
	vector_t pnts[4] = {
		Vector( -1.0f, +1.0f, 0.0f ),
		Vector( -1.0f, -1.0f, 0.0f ),
		Vector( +1.0f, +1.0f, 0.0f ),
		Vector( +1.0f, -1.0f, 0.0f )
	};
	vector_t pntsrot[4];
	//if (selected) {
		size23 *= fcos(m_cdrot[idx] * 2*M_PI / (hz*3));
		if (size23 < 0.0f) {
			m_txr_shiny[0]->select();
			inv = true;
		}
	//}
	//if (selected && !shadow) {
	if (!shadow) {
#define TRANS(dir) \
		plx_mat3d_mode(PLX_MAT_SCRATCH); \
		plx_mat3d_identity(); \
		plx_mat3d_translate(x, y, z); \
		plx_mat3d_scale(64, 64, 1); \
		if (inv) \
			plx_mat3d_rotate(180.0f + m_cdrot[idx] * 360 / (hz*3.0f), 1.0f, 1.0f, 0.0f); \
		else \
			plx_mat3d_rotate(m_cdrot[idx] * 360 / (hz*3.0f), 1.0f, 1.0f, 0.0f); \
		plx_mat3d_rotate(dir*m_cdrot[idx] * 360 / (hz*3.0f), 0.0f, 0.0f, 1.0f); \
		plx_mat_identity(); \
		plx_mat3d_apply(PLX_MAT_SCRATCH); \
		plx_mat_transform(pnts, pntsrot, 4, sizeof(vector_t)); \
		plx_mat3d_mode(PLX_MAT_MODELVIEW);

		TRANS(1.0f)
	} else {
		for (int i=0; i<4; i++)
			pntsrot[i] = pnts[i];
		pntsrot[0].x = pntsrot[0].x * size23 + x;
		pntsrot[0].y = pntsrot[0].y * size23 + y;
		pntsrot[0].z += z;
		pntsrot[1].x = pntsrot[1].x * size14 + x;
		pntsrot[1].y = pntsrot[1].y * size14 + y;
		pntsrot[1].z += z;
		pntsrot[2].x = pntsrot[2].x * size14 + x;
		pntsrot[2].y = pntsrot[2].y * size14 + y;
		pntsrot[2].z += z;
		pntsrot[3].x = pntsrot[3].x * size23 + x;
		pntsrot[3].y = pntsrot[3].y * size23 + y;
		pntsrot[3].z += z;
	}
	size23 = size23 < 0.0f ? -size23 : size23;

	plx_dr_state_t dr; plx_dr_init(&dr);
	plx_vert_ifd(&dr, PLX_VERT, pntsrot[0].x, pntsrot[0].y, z, color, 0.0f, 1.0f);
	plx_vert_ifd(&dr, PLX_VERT, pntsrot[1].x, pntsrot[1].y, z, color, 0.0f, 0.0f);
	plx_vert_ifd(&dr, PLX_VERT, pntsrot[2].x, pntsrot[2].y, z, color, 1.0f, 1.0f);
	plx_vert_ifd(&dr, PLX_VERT_EOS, pntsrot[3].x, pntsrot[3].y, z, color, 1.0f, 0.0f);

	if (inv) {
		TRANS(-1.0f);
	
		m_txr_shiny[1]->select();
		z += 0.5f;
		plx_vert_ifd(&dr, PLX_VERT, pntsrot[0].x, pntsrot[0].y, z, color, 0.0f, 1.0f);
		plx_vert_ifd(&dr, PLX_VERT, pntsrot[1].x, pntsrot[1].y, z, color, 0.0f, 0.0f);
		plx_vert_ifd(&dr, PLX_VERT, pntsrot[2].x, pntsrot[2].y, z, color, 1.0f, 1.0f);
		plx_vert_ifd(&dr, PLX_VERT_EOS, pntsrot[3].x, pntsrot[3].y, z, color, 1.0f, 0.0f);
	}
}

void CDTray::draw(ObjType list) {
	if (list != Trans)
		return;

	// float pf = m_frame * 2 * M_PI / 360.0f;
	float pf = -m_selected * m_per + 2*M_PI/8 + m_move_cur;

	// Figure out where to start and stop
	int sc = (int)(m_selected - m_move_cur / m_per);
	
	// Call draw_one() for each song
	float z = 50.0f;
	for (int i=sc - 4; i<sc + 6; i++) {
		// Figure out where the CD image goes
		float tx = m_cx - fcos(i * m_per + pf) * m_r;
		float ty = m_cy - fsin(i * m_per + pf) * m_r;

		// Figure out what the real CD index is
		int ri = i;
		while (ri < 0)
			ri += m_cnt;
		while (ri >= m_cnt)
			ri -= m_cnt;

		// Don't do useless drawing
		if ((tx-64) > 640.0f || (ty-64) > 480.0f)
			continue;

		// Do we spin it?
		bool spin = i == m_selected || (i == m_selected-1) || (i == m_selected+1);

		// Ok, draw it
		m_txr_shadow->select();
		drawOne(ri, tx, ty, z++, spin, true);

		m_textures[ri]->select();
		drawOne(ri, tx, ty, z++, spin, false);
	}

	// Draw a triangle with a black border
	Texture::deselect();
	
	float tx = m_cx - (m_r - 184.0f + 4.0f * fsin(m_frame * (60.0f/hz) * 2*M_PI/hz));
	float ty = m_cy - (m_r - 184.0f + 4.0f * fsin(m_frame * (60.0f/hz) * 2*M_PI/hz));

	Color tint = getColor();

	Color black = tint * Color(1, 0, 0, 0);
	plx_vert_inp(PLX_VERT, tx-2, ty-2, z, black);
	plx_vert_inp(PLX_VERT, tx+19, ty+3, z, black);
	plx_vert_inp(PLX_VERT_EOS, tx+3, ty+19, z++, black);

	plx_vert_inp(PLX_VERT, tx, ty, z, tint * Color(1, 0, 0));
	plx_vert_inp(PLX_VERT, tx+16, ty+4, z, tint * Color(0, 1, 0));
	plx_vert_inp(PLX_VERT_EOS, tx+4, ty+16, z, tint * Color(0, 0, 1));
}

void CDTray::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;

	for (int i=0; i<m_cnt; i++) {
		if (i == m_selected) {
			m_cdrot[i]++;
			m_cdrot[i] %= hz*3;
		} else {
			if (m_cdrot[i] != 0) {
				if (m_cdrot[i] % 2)
					m_cdrot[i]++;
				if (m_cdrot[i] < (hz*3 - m_cdrot[i]))
					m_cdrot[i] = (m_cdrot[i] - 2) % (hz*3);
				else
					m_cdrot[i] = (m_cdrot[i] + 2) % (hz*3);
			}
		}
	}

	if (m_move_dir) {
		m_move_cur += m_move_pf;
		if (m_move_dir < 0) {
			if (m_move_cur >= (m_per * -m_move_dir)) {
				m_move_cur = 0.0f;
				m_move_pf = 0.0f;
				m_last_sel = m_selected;
				m_selected += m_move_dir;
				while (m_selected < 0)
					m_selected += m_cnt;
				while (m_selected >= m_cnt)
					m_selected -= m_cnt;
				m_move_dir = 0;
			}
		} else {
			if (m_move_cur <= (m_per * -m_move_dir)) {
				m_move_cur = 0.0f;
				m_move_pf = 0.0f;
				m_last_sel = m_selected;
				m_selected += m_move_dir;
				while (m_selected < 0)
					m_selected += m_cnt;
				while (m_selected >= m_cnt)
					m_selected -= m_cnt;
				m_move_dir = 0;
			}
		}
	}
}

void CDTray::move(int dir, int frames) {
	m_move_dir = dir;
	m_move_cur = 0.0f;

	// Take 15 frames to move
	assert( frames != 0 );
	if (frames == 0) frames = 1;
	m_move_pf = (m_per * -dir) / (1.0f * frames);
}

void CDTray::setSelection(int idx) {
	m_selected = idx;
}
