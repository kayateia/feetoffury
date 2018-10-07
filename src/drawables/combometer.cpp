/* 
   Feet of Fury
   
   drawables/combometer.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "combometer.h"
#include "hsv.h"

ComboMeter::ComboMeter(int list, Texture * txr) {
	m_list = list;

	assert( txr != NULL );
	m_texture = txr;

	m_value = 0.0f;
	m_frame = 0;
	m_spec_ptr = 0.0f;
}

ComboMeter::~ComboMeter() {
}

void ComboMeter::draw(ObjType list) {
	if (m_list != list)
		return;

	plx_cxt_texture(m_texture);
	plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(m_list);

	// How big should the combo bar be?
	float bs = m_value * -(32*5.0f - 5.0f);

	if (bs != 0) {
		const Vector & p = getTranslate();
		Color col = getColor(); col.a *= 0.8f;
		uint32 color = plx_pack_color(col.a, col.r * m_r, col.g * m_g, col.b * m_b);
		plx_dr_state_t dr;
		plx_dr_init(&dr);

		plx_vert_ifd(&dr, PLX_VERT,     p.x+22, p.y+0,  p.z+0.1f, color, 1.0f, 54.0f/64.0f);
		plx_vert_ifd(&dr, PLX_VERT,     p.x+0,  p.y+0,  p.z+0.1f, color, 1.0f, 32.0f/64.0f);
		plx_vert_ifd(&dr, PLX_VERT,     p.x+22, p.y+bs, p.z+0.1f, color, 0.0f, 54.0f/64.0f);
		plx_vert_ifd(&dr, PLX_VERT_EOS, p.x+0,  p.y+bs, p.z+0.1f, color, 0.0f, 32.0f/64.0f);
	}
}

void ComboMeter::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	m_frame++;

	// Calculates R/G/B values
	if (m_value >= 1.0f) {
		m_spec_ptr += 1/12.0f;
	} else {
		m_spec_ptr += 1/120.0f;
	}
	while (m_spec_ptr > 1.0f)
		m_spec_ptr -= 1.0f;
	hsvSpectrum(m_spec_ptr, m_r, m_g, m_b);
}

