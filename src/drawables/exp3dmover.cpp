/*
   Feet of Fury

   exp3dmover.cc

   Copyright (C)2003,2005 Cryptic Allusion, LLC
*/

#include "global.h"
#include "exp3dmover.h"

Exp3DMover::Exp3DMover(float dx, float dy, float dz, float maxx, float maxy, float maxz) {
	m_dx = dx;
	m_dy = dy;
	m_dz = dz;
	m_maxx = maxx;
	m_maxy = maxy;
	m_maxz = maxz;
}

Exp3DMover::~Exp3DMover() { }

void Exp3DMover::nextFrame(Drawable *t) {
	Vector p = t->getTranslate();

	bool xfin = m_dx < 0 ? (p.x <= m_maxx) : (p.x >= m_maxx);
	bool yfin = m_dy < 0 ? (p.y <= m_maxy) : (p.y >= m_maxy);
	bool zfin = m_dz < 0 ? (p.z <= m_maxz) : (p.z >= m_maxz);
	if (xfin && yfin && zfin) {
		t->setTranslate(Vector(m_maxx, m_maxy, m_maxz));
		complete(t);
	} else {
		// Move 1.15x of the distance each frame
		p += Vector(m_dx, m_dy, m_dz);
		t->setTranslate(p);
		m_dx *= 1.15f;
		m_dy *= 1.15f;
		m_dz *= 1.15f;
	}
}
