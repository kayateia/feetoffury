/* 
   Feet of Fury
   
   drawables/bouncetxr.cc
   
   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "bouncetxr.h"

BounceTxr::BounceTxr(int list, Texture * txr) {
	m_list = list;
	m_txr = txr;
	m_frame = 0;
}

BounceTxr::~BounceTxr() {
}

void BounceTxr::draw(ObjType list) {
	if (list != m_list || m_txr == NULL)
		return;

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	m_txr->select();

	Vector t = getPosition();
	const Vector & s = getScale();
	Color tint = getColor();
	plx_mat_identity();
	plx_mat_translate(t.x, t.y, t.z);
	plx_mat_rotate_z(30.0f * fsin(m_frame * (60.0f/hz) * 2*M_PI / 80.0f) * M_PI/180.0f);
	plx_mat_scale(s.x, s.y, s.z);

	plx_spr_fmd(&dr, m_txr->getW(), m_txr->getH(), 0.0f, 0.0f, 0.0f, tint.a, tint.r, tint.g, tint.b);
}

void BounceTxr::nextFrame(uint64 tm) {
	m_frame++;
}

void BounceTxr::setTexture(Texture * txr) {
	m_txr = txr;
}
