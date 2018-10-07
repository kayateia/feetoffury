/*
   Feet of Fury

   log3dmover.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "log3dmover.h"

Log3DMover::Log3DMover(float dstx, float dsty, float dstz) {
	m_dstx = dstx;
	m_dsty = dsty;
	m_dstz = dstz;
}

Log3DMover::~Log3DMover() { }

void Log3DMover::nextFrame(Drawable *t) {
	Vector pos = t->getTranslate();
	if (Math::fabs(pos.x - m_dstx) < 0.01f && Math::fabs(pos.y - m_dsty) < 0.01f && Math::fabs(pos.z - m_dstz) < 0.01f) {
		t->setTranslate(Vector(m_dstx, m_dsty, m_dstz));
		complete(t);
	} else {
		// Move 1/8 of the distance each frame
		float dx = m_dstx - pos.x;
		float dy = m_dsty - pos.y;
		float dz = m_dstz - pos.z;
		t->setTranslate(Vector(
			pos.x + dx/8.0f, pos.y + dy/8.0f, pos.z + dz/8.0f));
	}
}
