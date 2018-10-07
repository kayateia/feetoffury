/*
   Feet of Fury

   soundfader.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "soundfader.h"
#include "ogg.h"

SoundFader::SoundFader(float dst, float dv) {
	m_dstvol = dst;
	m_dv = dv;
}

SoundFader::~SoundFader() { }

void SoundFader::nextFrame(Drawable *t) {
	bool comp = false;
	float cur = oggVolume();
	if (m_dv < 0.0f) {
		if (cur <= m_dstvol) {
			cur = m_dstvol;
			comp = true;
		} else
			cur += m_dv;
	} else {
		if (cur >= m_dstvol) {
			cur = m_dstvol;
			comp = true;
		} else {
			cur += m_dv;
		}
	}
	oggVolume(cur);
	if (comp)
		complete(t);
}
