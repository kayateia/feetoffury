/* 
   Feet of Fury
   
   drawables/scroller.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "scroller.h"

Scroller::Scroller(Font * fnt, float ps, const char * text) {
	m_font = fnt;
	m_frame = 0;
	m_ps = ps;
	setText(text);
}

Scroller::~Scroller() {
}

void Scroller::setText(const char * cc) {
	m_text = cc;
	m_text_len = strlen(cc);
	assert( m_text_len > 0 );
	if (m_text_len == 0) {
		m_text = " ";
		m_text_len = 1;
	}

	m_xoffs = 800.0f;
}

void Scroller::draw(ObjType list) {
	if (list != Trans)
		return;

	Vector pos = getPosition();
	Color col = getColor();

	plx_cxt_texture(NULL);
	plx_cxt_send(list);

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	// Draw a background behind it
	float ps = m_ps + 4;
	plx_vert_fnd(&dr, PLX_VERT, 0, pos.y + ps/2, pos.z, 0.4f*col.a, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, 0, pos.y - ps/2, pos.z, 0.4f*col.a, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, 640, pos.y + ps/2, pos.z, 0.4f*col.a, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT_EOS, 640, pos.y - ps/2, pos.z, 0.4f*col.a, 0.0f, 0.0f, 0.0f);

	float cl, cu, cr, cd;
	float xo = m_xoffs;

	// Nothing to display yet
	if (xo >= 640.0f)
		return;

	m_font->setSize(m_ps);
	m_font->setColor(col);

	// Skip everything that's already gone off the screen
	int i = 0;
	if (xo < 0.0f) {
		while (xo < 0.0f) {
			m_font->getCharExtents(m_text[i], cl, cu, cr, cd);
			xo += cr;
			i++;
		}
		if (xo > 0.0f) {
			i--; xo -= cr;
			assert( i >= 0 );
		}
		if (i == m_text_len) {
			m_xoffs = xo = 0.0f;
			i = 0;
		}
	}

	m_font->drawBegin();
	m_font->drawCharSetPos(Vector(xo, pos.y + m_ps/2, pos.z+0.1f));
	while (xo < 640.0f) {
		xo += m_font->drawChar(m_text[i]);
		i++;
		i = i % m_text_len;
	}
	m_font->drawEnd();
}

void Scroller::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_xoffs-=4;
	m_frame++;
}

