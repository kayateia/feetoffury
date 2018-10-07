/* 
   Feet of Fury
   
   drawables/winner.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "winner.h"
#include "res.h"

Winner::Winner() {
	m_txr_star = res.getTexture(Res::Gameplay, "star", true);
	m_txr_winner = res.getTexture(Res::Gameplay, "winner", true);
}

Winner::~Winner() {
}

// Set the winning side and reset our parameters
void Winner::setSide(int s) {
	m_side = s;
	m_frame = 0;

	m_stars[0] = StarPos(0, 358.0f, 166.0f, 0.35f);
	m_stars[1] = StarPos(1, 388.0f, 299.0f, 0.41f);
	m_stars[2] = StarPos(2, 439.0f, 163.0f, 0.60f);
	m_stars[3] = StarPos(3, 475.0f, 285.0f, 0.60f);
	m_stars[4] = StarPos(4, 551.0f, 219.0f, 1.00f);
	m_stars[5] = StarPos(5, 398.0f, 233.0f, 0.00f);
	if (s == 0) {
		for (int i=0; i<5; i++)
			m_stars[i].x = 640.0f - m_stars[i].x;
		m_stars[5].x -= 320.0f;
	}
}

void Winner::drawOne(StarPos *sp) {
	Color tint = getColor();

	if (sp->s == 0.0f) {
		float v1, v2;

		if (m_side == 0 || m_side == 1) {
			v1 = 0.0f;
			v2 = 0.5f;
		} else {
			v1 = 0.5f;
			v2 = 1.0f;
		}

		uint32 color = tint * sp->col;
		plx_dr_state_t dr;
		float x = sp->x, y = sp->y, z = getPosition().z + sp->idx;

		plx_dr_init(&dr);
		m_txr_winner->select();

		plx_vert_ifd(&dr, PLX_VERT, x-64, y+64, z, color, 0.0f, v2);
		plx_vert_ifd(&dr, PLX_VERT, x-64, y-64, z, color, 0.0f, v1);
		plx_vert_ifd(&dr, PLX_VERT, x+192, y+64, z, color, 1.0f, v2);
		plx_vert_ifd(&dr, PLX_VERT_EOS, x+192, y-64, z,color, 1.0f, v1);

		if (m_side < 0) {
			if (sp->s != 0.0f)
				x = (640.0f - sp->x);
			else
				x -= 320.0f;

			plx_vert_ifd(&dr, PLX_VERT, x-64, y+64, z, color, 0.0f, v2);
			plx_vert_ifd(&dr, PLX_VERT, x-64, y-64, z, color, 0.0f, v1);
			plx_vert_ifd(&dr, PLX_VERT, x+192, y+64, z, color, 1.0f, v2);
			plx_vert_ifd(&dr, PLX_VERT_EOS, x+192, y-64, z,color, 1.0f, v1);
		}
	} else {
		uint32 color = tint * sp->col;
		plx_dr_state_t dr;
		float s = 128.0f * sp->s, x = sp->x, y = sp->y, z = getPosition().z + sp->idx;

		plx_dr_init(&dr);
		m_txr_star->select();

		plx_spr_ind(&dr, s, s, x, y, z, color);

		if (m_side < 0) {
			if (sp->s != 0.0f)
				x = (640.0f - sp->x);
			else
				x -= 320.0f;

			plx_spr_ind(&dr, s, s, x, y, z, color);
		}
	}
}

void Winner::drawWinner() {
	for (int i=0; i<6; i++)
		drawOne(m_stars+i);
}

void Winner::nextFrame(uint64 tm) {
	m_frame++;
	if (!(m_frame % 5)) {
		int mfd = m_frame / 5;
		if (mfd < 5)
			m_stars[mfd].dcol.a = 0.05f;
	}
	if (m_frame == hz) {
		m_stars[5].dcol.a = 0.1f;
	}

	for (int i=0; i<6; i++) {
		if (m_stars[i].dcol.a > 0) {
			m_stars[i].push();
			if (m_stars[i].col.a >= 1.0f) {
				m_stars[i].col.a = 1.0f;
				if (i < 5) {
					m_stars[i].dcol = Color(-0.01f,
						-(1.0f - 0.24f) / 15,
						-(1.0f - 0.38f) / 15,
						-(1.0f - 0.56f) / 15);
				}
			}
		} else if (m_stars[i].dcol.a < 0) {
			m_stars[i].push();
			if (m_stars[i].col.r <= 0.24f) {
				m_stars[i].col = Color(1.0f, 0.24f, 0.38f, 0.56f);
				m_stars[i].dcol = Color(0,0,0,0);
			}
		}
	}
}
