/* 
   Feet of Fury
   
   drawables/textdisplay.cc
   
   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "textdisplay.h"

TextDisplay::TextDisplay(Font *fh, float ps, bool center) {
	m_fh = fh;
	assert( m_fh != NULL );

	m_ps = ps;
	m_bga = 0.0f;
	m_center = center;
}

TextDisplay::~TextDisplay() {
}

void TextDisplay::setBGAlpha(float a) {
	m_bga = a;
}

void TextDisplay::setText(const char **text, int lineCnt) {
	std::vector<string> s;
	
	s.resize(lineCnt);
	for (int i=0; i<lineCnt; i++)
		s[i] = text[i];
	setText(s);
}

void TextDisplay::setText(const std::vector<string> & text) {
	m_text = text;
	recalcTextSize();
}

void TextDisplay::recalcTextSize() {
	int line_cnt = m_text.size();

	// Now figure out how big we'll be
	m_w = m_h = 0;
	m_text_len = 0;
	for (int i=0; i<line_cnt; i++) {
		float w, h;

		m_fh->setSize(m_ps);
		m_fh->getTextSize(m_text[i], w, h);

		if (w > m_w)
			m_w = w;
		m_h += h * 1.5f;

		m_text_len += m_text[i].length();
	}

	// XXX WTF is this?!
	float l, u, r, d;
	m_fh->getCharExtents('\'', l, u, r, d);
}

void TextDisplay::getSize(float & w, float & h) {
	w = m_w;
	h = m_h;
}

void TextDisplay::setFont(Font *f) {
	m_fh = f;
}

void TextDisplay::setSize(float s) {
	m_ps = s;
	recalcTextSize();
}

void TextDisplay::draw(ObjType list) {
	// We only do translucent
	if (list != Trans)
		return;

	// Start at the center point
	Vector pos = getPosition();
	Color col = getColor();
	if (col.r == 1.0f && col.g == 1.0f && col.b == 1.0f) {
		col.r = col.g = col.b = 0.85f;
	}

	if (m_bga > 0.0f) {
		uint32 color = plx_pack_color(col.a * m_bga, 0.0f, 0.0f, 0.0f);
		plx_cxt_texture(NULL);
		plx_vert_inp(PLX_VERT, pos.x - (m_w + m_ps)/2.0f, pos.y + (m_h + m_ps)/2.0f, pos.z, color);
		plx_vert_inp(PLX_VERT, pos.x - (m_w + m_ps)/2.0f, pos.y - (m_h + m_ps)/2.0f, pos.z, color);
		plx_vert_inp(PLX_VERT, pos.x + (m_w + m_ps)/2.0f, pos.y + (m_h + m_ps)/2.0f, pos.z, color);
		plx_vert_inp(PLX_VERT_EOS, pos.x + (m_w + m_ps)/2.0f, pos.y - (m_h + m_ps)/2.0f, pos.z, color);
		pos.z += 1.0f;
	}

	// Move out for the proper size
	if (!m_center)
		pos.x -= m_w/2.0f;
	pos.y -= m_h/2.0f;

	m_fh->setSize(m_ps);
	m_fh->setColor(col);
	for (int i=0, cnt=m_text.size(); i<cnt; i++) {
		Vector t(pos);
		float sx, sy;
		m_fh->upperleftCoords(m_text[i], t);
		m_fh->getTextSize(m_text[i], sx, sy);

		if (m_center)
			t.x += -sx/2.0f;

		m_fh->draw(t, m_text[i]);
		
		pos.y += sy * 1.5f;
	}
}

void TextDisplay::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
}

