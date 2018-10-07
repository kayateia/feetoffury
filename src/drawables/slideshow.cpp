/* 
   Feet of Fury
   
   drawables/slideshow.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "slideshow.h"

SlideShow::SlideShow(int numpics, int delay, float speed) {
	m_pics = new RefPtr<Texture>[numpics];
	m_flips = new bool[numpics];
		
	m_pic_cnt = numpics;
	m_pic_cur = m_pic_to = -1;
	m_cntdown = m_delay = delay;
	m_fading = false;
	m_fadespeed = speed;
}

SlideShow::~SlideShow() {
	delete[] m_pics;
	delete[] m_flips;
}

// Set a picture in the slideshow
void SlideShow::setPic(int idx, Texture * txr, bool hflip) {
	assert( idx < m_pic_cnt );
	assert( txr != NULL );

	m_pics[idx] = txr;
	m_flips[idx] = hflip;
}

// Force a switch to another slide in no-auto mode
void SlideShow::setSlide(int s) {
	if (m_fading)
		m_pic_cur = m_pic_to;
	
	m_pic_to = s;
	m_fade = 0.0f;
	m_fading = true;
}

void SlideShow::draw(ObjType list) {
	// Check which list
	if (list != Trans)
		return;

	float u1, v1, u2, v2;
	v1 = 0.0f; v2 = 1.0f;

	const Vector & sc = getScale();
	Vector pos = getPosition();
	Color col = getColor();
	
	if (m_pic_cur != -1) {
		if (m_flips[m_pic_cur]) {
			u1 = 1.0f;
			u2 = 0.0f;
		} else {
			u1 = 0.0f;
			u2 = 1.0f;
		}

		Texture *t = m_pics[m_pic_cur];
		float w = t->getW() * sc.x / 2.0f, h = t->getH() * sc.y / 2.0f;
		plx_vertex_t v;

		t->select();

		float x1 = pos.x-w, y1 = pos.y-h, x2 = pos.x+w, y2 = pos.y+h;
		
		v.flags = PLX_VERT;
		v.x = x1;
		v.y = y2;
		v.z = pos.z;
		v.u = u1;
		v.v = v2;
		v.argb = plx_pack_color(col.a * (1.0f - m_fade), col.r, col.g, col.b);
		v.oargb = 0;
		plx_prim(&v, sizeof(v));

		v.y = y1;
		v.v = v1;
		plx_prim(&v, sizeof(v));

		v.x = x2;	v.u = u2;
		v.y = y2;	v.v = v2;
		plx_prim(&v, sizeof(v));

		v.flags = PLX_VERT_EOS;
		v.y = y1;
		v.v = v1;
		plx_prim(&v, sizeof(v));
	}
	if (m_pic_to != -1) {
		if (m_flips[m_pic_to]) {
			u1 = 1.0f;
			u2 = 0.0f;
		} else {
			u1 = 0.0f;
			u2 = 1.0f;
		}

		Texture *t = m_pics[m_pic_to];
		float w = t->getW() * sc.x / 2.0f, h = t->getH() * sc.y / 2.0f;
		plx_vertex_t v;

		t->select();

		float x1 = pos.x-w, y1 = pos.y-h, x2 = pos.x+w, y2 = pos.y+h;
		
		v.flags = PLX_VERT;
		v.x = x1;
		v.y = y2;
		v.z = pos.z + 0.5f;
		v.u = u1;
		v.v = v2;
		v.argb = plx_pack_color(col.a * m_fade, col.r, col.g, col.b);
		v.oargb = 0;
		plx_prim(&v, sizeof(v));

		v.y = y1;
		v.v = v1;
		plx_prim(&v, sizeof(v));

		v.x = x2;	v.u = u2;
		v.y = y2;	v.v = v2;
		plx_prim(&v, sizeof(v));

		v.flags = PLX_VERT_EOS;
		v.y = y1;
		v.v = v1;
		plx_prim(&v, sizeof(v));
	}
}

void SlideShow::nextFrame(uint64 tm) {
	if (m_delay > 0 && m_cntdown <= 0) {
		m_pic_to = (m_pic_cur + 1) % m_pic_cnt;
		m_fade = 0.0f;
		m_fading = true;
		m_cntdown = m_delay;
	}
	if (m_fading) {
		m_fade += m_fadespeed / hz;
		if (m_fade >= 1.0f) {
			m_fade = 0.0f;
			m_pic_cur = m_pic_to;
			m_fading = false;
		}
	}
	if (m_delay > 0)
		m_cntdown--;

	Drawable::nextFrame(tm);
}

