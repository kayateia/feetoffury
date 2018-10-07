/* 
   Feet of Fury
   
   drawables/cubecube.cc
   
   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "cubecube.h"

// XXX
#include "../gameplay/bgeffects/top_scope.h"

#define vert_ifdm3(st, flg, x1, y1, z1, color, u, v) do { \
	float tx = x1, ty = y1, tz = z1; \
	plx_mat_tfip_3d(tx, ty, tz); \
	if (tz <= 0.0f) { \
		tx = lx; ty = ly; tz = lz; \
	} else { \
		lx = tx; ly = ty; lz = tz; \
	} \
	plx_vert_ifd(st, flg, tx, ty, tz, color, u, v); \
} while (0)

CubeCube::CubeCube(MenuPlayStyle::MPSRes * res, int init) {
	m_res = res;
	m_r = 0;

	switchSelect(init);
	while (m_destrot.y < 0)
		m_destrot.y += 360;
	while (m_destrot.y >= 360)
		m_destrot.y -= 360;
	m_currot = m_destrot;
}

CubeCube::~CubeCube() {
}

void CubeCube::draw(ObjType list) {
	struct cpos_t {
		cpos_t(Vector p, Color t) { pos = p; tint = t; }
		Vector	pos;
		Color	tint;
	} cpos[8] = {
		cpos_t( Vector(-1, +1, -1), Color( 1, 1, 0 ) ),		// C / Yellow
		cpos_t( Vector(+1, +1, -1), Color( 0, 1, 0 ) ),		// R / Green
		cpos_t( Vector(+1, +1, +1), Color( 0, 0, 1 ) ),		// H / Blue
		cpos_t( Vector(-1, +1, +1), Color( 1, 0, 0 ) ),		// D / Red
		
		cpos_t( Vector(-1, -1, -1), Color( 0.7, 1, 0.2 ) ),	// CR / Lime(Y+G)
		cpos_t( Vector(+1, -1, -1), Color( 1.0, 0.7, 0.2 ) ),	// CD / Orange(Y+R)
		cpos_t( Vector(+1, -1, +1), Color( 1, 0, 1 ) ),		// DH / Magenta(R+B)
		cpos_t( Vector(-1, -1, +1), Color( 0, 1, 1 ) )		// RH / Cyan(G+B)
	};

	Vector pos = getTranslate();
	Vector scale = getScale();

	plx_mat3d_identity();
	plx_mat3d_translate(pos.x, pos.y, pos.z);
	plx_mat3d_rotate(m_currot.x, 1, 0, 0);
	plx_mat3d_rotate(m_currot.y, 0, 1, 0);
	plx_mat3d_scale(scale.x, scale.y, scale.z);
	plx_mat3d_push();

	if (list == Opaque) {
		Texture * ctxrs[8][6] = {
			// Confusion: drunken, vortex, turbo, phasing
			{ m_res->tech_txr[AttackDrunkenArrow], 
				m_res->tech_txr[AttackVortex], 
				m_res->tech_txr[AttackTurbo], 
				m_res->tech_txr[AttackPhasingArrows], 
				m_res->tech_txr[AttackVortex], 
				m_res->tech_txr[AttackTurbo] },
			// Rescue: reprieve(eraser), dispel, reverse
			{ m_res->tech_txr[AttackEraser],
				m_res->tech_txr[AttackDispel],
				m_res->tech_txr[AttackReverse],
				m_res->tech_txr[AttackEraser],
				m_res->tech_txr[AttackDispel],
				m_res->tech_txr[AttackReverse] },
			// Health: health
			{ m_res->tech_txr[AttackHealth],
				m_res->tech_txr[AttackHealth],
				m_res->tech_txr[AttackHealth],
				m_res->tech_txr[AttackHealth],
				m_res->tech_txr[AttackHealth],
				m_res->tech_txr[AttackHealth] },
			// Damage: fireball, bomb
			{ m_res->tech_txr[AttackFireball],
				m_res->tech_txr[AttackBomb],
				m_res->tech_txr[AttackFireball],
				m_res->tech_txr[AttackBomb],
				m_res->tech_txr[AttackFireball],
				m_res->tech_txr[AttackBomb] },
			// Confusion/Rescue
			{ m_res->tech_txr[AttackDrunkenArrow],
				m_res->tech_txr[AttackEraser],
				m_res->tech_txr[AttackVortex],
				m_res->tech_txr[AttackTurbo],
				m_res->tech_txr[AttackDispel],
				m_res->tech_txr[AttackPhasingArrows] },
			// Confusion/Damage
			{ m_res->tech_txr[AttackDrunkenArrow], 
				m_res->tech_txr[AttackVortex], 
				m_res->tech_txr[AttackTurbo], 
				m_res->tech_txr[AttackPhasingArrows], 
				m_res->tech_txr[AttackFireball], 
				m_res->tech_txr[AttackBomb] },
			// Damage/Health
			{ m_res->tech_txr[AttackFireball], 
				m_res->tech_txr[AttackHealth], 
				m_res->tech_txr[AttackBomb], 
				m_res->tech_txr[AttackHealth], 
				m_res->tech_txr[AttackFireball], 
				m_res->tech_txr[AttackHealth] },
			// Rescue/Health
			{ m_res->tech_txr[AttackEraser], 
				m_res->tech_txr[AttackHealth], 
				m_res->tech_txr[AttackDispel], 
				m_res->tech_txr[AttackHealth], 
				m_res->tech_txr[AttackReverse], 
				m_res->tech_txr[AttackEraser] }
		};

		// Draw our actual selection cubes
		Color tint = getColor();
		for (int i=0; i<8; i++) {
			const Vector & v = cpos[i].pos;
			plx_mat3d_translate(v.x*2, v.y*2, v.z*2);
			if (i == m_selected) {
				float s = fsin(m_r * 2*M_PI/(hz/2)) * 0.2f + 1.2f;
				plx_mat3d_scale(s,s,s);
			}
			plx_mat3d_apply_all();
			//printf("drawing cube %d: %p %p %p %p %p %p\n", i,
			//	ctxrs[i][0], ctxrs[i][1], ctxrs[i][2], ctxrs[i][3], ctxrs[i][4], ctxrs[i][5]);
			drawCube(ctxrs[i], cpos[i].tint * tint);
			plx_mat3d_peek();
		}
	} else if (list == Trans) {
		// Now then, we also need to draw 6 crossbars. We'll just cheat
		// and reuse code from the o-scope drawable.
		plx_cxt_texture(NULL);
		plx_cxt_culling(PLX_CULL_NONE);
		plx_cxt_send(list);

		Color tint = getColor();
		float r = 0.4f, g = 0.4f, b = 0.4f;
		uint32 colors[6] = {
			Color(0.4f, r, g, b) * tint,
			Color(0.7f, r, g, b) * tint,
			Color(0.5f, r, g, b) * tint,
			Color(0.4f, r, g, b) * tint,
			Color(0.6f, r, g, b) * tint,
			Color(0.7f, r, g, b) * tint
		};
		{
			float pnts[2][3] = {
				{ -0.3f, -1.1f, -0.3f },
				{ +0.3f, +1.1f, +0.3f }
			};
			const Vector offs[4] = {
				Vector(-2,0,-2),
				Vector(-2,0,+2),
				Vector(+2,0,-2),
				Vector(+2,0,+2)
			};

			for (int i=0; i<4; i++) {
				plx_mat3d_translate(offs[i].x, 0, offs[i].z);
				plx_mat3d_apply_all();
				TopScope::drawCube(pnts[0], pnts[1], colors);
				plx_mat3d_peek();
			}
		}
		{
			float pnts[2][3] = {
				{ -1.1f, -0.3f, -0.3f },
				{ +1.1f, +0.3f, +0.3f }
			};
			const Vector offs[4] = {
				Vector(0,-2,-2),
				Vector(0,-2,+2),
				Vector(0,+2,-2),
				Vector(0,+2,+2)
			};

			for (int i=0; i<4; i++) {
				plx_mat3d_translate(0, offs[i].y, offs[i].z);
				plx_mat3d_apply_all();
				TopScope::drawCube(pnts[0], pnts[1], colors);
				plx_mat3d_peek();
			}
		}
		{
			float pnts[2][3] = {
				{ -0.3f, -0.3f, -1.1f },
				{ +0.3f, +0.3f, +1.1f }
			};
			const Vector offs[4] = {
				Vector(-2,-2,0),
				Vector(-2,+2,0),
				Vector(+2,-2,0),
				Vector(+2,+2,0)
			};

			for (int i=0; i<4; i++) {
				plx_mat3d_translate(offs[i].x, offs[i].y, 0);
				plx_mat3d_apply_all();
				TopScope::drawCube(pnts[0], pnts[1], colors);
				plx_mat3d_peek();
			}
		}
	}
	plx_mat3d_pop();
}

#define fabs Math::fabs

void CubeCube::nextFrame(uint64 tm) {
	m_r++;

	// This is the same basic algorithm as the logxymover, except
	// we have to deal with the pain in the ass that is modular math
	// for the degree measurement.
	if (m_currot != m_destrot) {
		Vector d = (m_destrot - m_currot) * (1.0f/8.0f);
		m_currot += d;

		d = m_destrot - m_currot;
		if (fabs(d.x) < 0.1f) {
			m_currot.x = m_destrot.x;
		}
		if (fabs(d.y) < 0.1f) {
			while (m_destrot.y < 0)
				m_destrot.y += 360;
			while (m_destrot.y >= 360)
				m_destrot.y -= 360;
			m_currot.y = m_destrot.y;
		}
	}
}

static inline float tabs(float a) { return a < 0.0f ? -a : a; }
void CubeCube::drawCube(Texture * texture[6], const Color & a) {
	plx_dr_state_t dr;
	uint32 color = a;
	float lx = -50.0f, ly = -50.0f, lz = -50.0f;
	
	plx_dr_init(&dr);
	
	plx_cxt_culling(PLX_CULL_NONE);

	/* Front Face */
	plx_cxt_texture(texture[0]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);

	/* Back Face */
	plx_cxt_texture(texture[1]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);

	/* Top Face */
	plx_cxt_texture(texture[2]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);

	/* Bottom Face */
	plx_cxt_texture(texture[3]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);

	/* Right face */
	plx_cxt_texture(texture[4]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);

	/* Left Face */
	plx_cxt_texture(texture[5]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
}

void CubeCube::switchSelect(int idx) {
	Debug::printf("switching to select %d\n", idx);
	m_selected = idx;

	// X axis rotation is based on top/bottom
	if (idx < 4)
		m_destrot.x = 45;
	else
		m_destrot.x = -45;

	// Y axis rotation is based on mod 4
	idx = idx % 4;
	switch (idx) {
	case 0:
		m_destrot.y = 90+45; break;
	case 1:
		m_destrot.y = 90+90+45; break;
	case 2:
		m_destrot.y = 90+90+90+45; break;
	case 3:
		m_destrot.y = 45; break;
	}

	// If it's faster to go the opposite way, boost the rotation
	if (fabs((m_destrot.y + 360) - m_currot.y) < fabs(m_destrot.y - m_currot.y)) {
		m_destrot.y += 360;
	}
	if (fabs((m_destrot.y - 360) - m_currot.y) < fabs(m_destrot.y - m_currot.y)) {
		m_destrot.y -= 360;
	}
}

int CubeCube::getSelect() const {
	return m_selected;
};
