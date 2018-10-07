/* 
   Feet of Fury
   
   drawables/lifemeter.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "lifemeter.h"

LifeMeter::LifeMeter(int list, Texture * txr, bool grow_left) {
	m_list = list;

	assert( txr != NULL );
	m_texture = txr;
	m_texture->setUVClamp(Texture::UVClamp, Texture::UVClamp);

	m_value = 0.0f;
	m_dvalue = 0.0f;
	m_value_to = 0.0f;
	m_grow_left = grow_left;
	m_frame = 0;
}

LifeMeter::~LifeMeter() {
}

void LifeMeter::setValue(float v) {
	m_value_to = v; 
	if (v < m_value)
		m_dvalue = -1.0f / hz;
	else if (v > m_value)
		m_dvalue = 1.0f / hz;
	else
		m_dvalue = 0.0f;
}

void LifeMeter::draw(ObjType list) {
	if (m_list != list)
		return;

	plx_cxt_texture(m_texture);
	plx_cxt_culling(PLX_CULL_NONE);

	Vector p = getPosition();

	// Figure out what the coords of the bar will be
	float x = p.x;
	switch (m_grow_left) {
	case true:
		x = p.x + 256.0f;
		break;
	case false:
		x = p.x;
		break;
	}

	// How big should the life bar be?
	// float bs = m_value * (32*8.0f);
	float bs = m_value * 224.0f;
	float ts = -11;
	if (m_grow_left) {
		bs = -bs;
		ts = -ts;
	}

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	if (bs != 0) {
		float y = p.y - 32 + 5, z = p.z + 0.1f;
		uint32 color = getColor() * Color(0.8f, m_r, m_g, m_b);

		if (Math::fabs(bs) > 11) {
			// Draw the main portion of the bar first

			plx_vert_ifd(&dr, PLX_VERT,     x+0,     y+22, z, color, 1.0f,        22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+0,     y+0,  z, color, 1.0f,        0.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+bs+ts, y+22, z, color, 11.0f/64.0f, 22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT_EOS, x+bs+ts, y+0,  z, color, 11.0f/64.0f, 0.0f/64.0f);

			// Now draw the tip
			plx_vert_ifd(&dr, PLX_VERT,     x+bs+ts, y+22, z, color, 11.0f/64.0f, 22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+bs+ts, y+0,  z, color, 11.0f/64.0f, 0.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+bs,    y+22, z, color, 0.0f/64.0f,  22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT_EOS, x+bs,    y+0,  z, color, 0.0f/64.0f,  0.0f/64.0f);
		} else {
			// Draw the tip
			plx_vert_ifd(&dr, PLX_VERT,     x+0,  y+22, z, color, 11.0f/64.0f, 22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+0,  y+0,  z, color, 11.0f/64.0f, 0.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT,     x+bs, y+22, z, color, 0.0f/64.0f,  22.0f/64.0f);
			plx_vert_ifd(&dr, PLX_VERT_EOS, x+bs, y+0,  z, color, 0.0f/64.0f,  0.0f/64.0f);
		}
	}
}

void LifeMeter::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	m_frame++;

	// Calculate an R/G/B value
	m_r = fsin(m_frame * 2 * M_PI / (hz*1.0f)) / 8.0f + 0.875f;
	m_g = fcos(m_frame * 2 * M_PI / (hz*2.0f)) / 8.0f + 0.875f;
	m_b = fsin(m_frame * 2 * M_PI / (hz*3.0f)) / 8.0f + 0.875f;

	if (m_dvalue != 0.0f) {
		m_value += m_dvalue;
		if (m_dvalue < 0.0f) {
			if (m_value <= m_value_to) {
				m_value = m_value_to;
				m_dvalue = 0.0f;
			}
		} else {
			if (m_value >= m_value_to) {
				m_value = m_value_to;
				m_dvalue = 0.0f;
			}
		}
	}
}

