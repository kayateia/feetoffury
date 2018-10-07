/*      
   Feet of Fury
        
   topbar.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "topbar.h"

Topbar::Topbar(float sizemax, float speed, Orientation o, Pattern p, float offset) {
	m_frame = 0;
	m_barsize = 1.0f;

	m_barcodesize = 0.0f;
	switch(p) {
	case Barcode:
		for (int i=0; i<64; i++) {
			m_barcode[i] = (float)(((rand() >> (32-4)) & 15) + 1);
			m_barcodesize += m_barcode[i];
		}
		break;
	case Checker:
		for (int i=0; i<64; i++) {
			m_barcode[i] = sizemax;
			m_barcodesize += sizemax;
		}
		break;
	default:
		Debug::printf("Invalid pattern type");
		assert( 0 );
	}
	
	m_baralpha = 1.0f;
	m_barcodeoffs = offset;
	m_barcodealpha = 0.0f;

	m_barsizemax = sizemax;
	m_barcodespeed = speed;

	m_orient = o;
}

Topbar::~Topbar() {
}

void Topbar::draw(ObjType list) {
	// We only display on translucent
	if (list != Trans)
		return;

	// DR setup
	Color c = getColor();
	uint32 color = c * Color(m_baralpha, 1.0f, 1.0f, 1.0f);
	plx_dr_state_t dr;
	plx_dr_init(&dr);
	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);

	// 0 - 29: phasing in
	float xmax = 0.0f;
	Vector pos = getPosition();

	switch (m_orient) {
	case Horizontal:
		plx_vert_ind(&dr, PLX_VERT,       0.0f, pos.y + m_barsize/2, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT,       0.0f, pos.y - m_barsize/2, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT,     640.0f, pos.y + m_barsize/2, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT_EOS, 640.0f, pos.y - m_barsize/2, pos.z, color);
		xmax = 640.0f;
		break;

	case Vertical:
		plx_vert_ind(&dr, PLX_VERT,     pos.x + m_barsize/2,   0.0f, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT,     pos.x - m_barsize/2,   0.0f, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT,     pos.x + m_barsize/2, 480.0f, pos.z, color);
		plx_vert_ind(&dr, PLX_VERT_EOS, pos.x - m_barsize/2, 480.0f, pos.z, color);
		xmax = 480.0f;
		break;
	};
	
	color = c * Color(m_barcodealpha, 1.0f, 1.0f, 1.0f);

	// What phase are we in?
	if (m_frame >= 7) {
		// 60 -:     drawing barcode pattern, half-alpha white bar

		// Draw barcode
		float x;
		if (m_barcodespeed < 0)
			x = m_barcodeoffs;
		else	
			x = -m_barcodesize + m_barcodeoffs;
		int i = 0;
		while (x < xmax) {
			if (!(i % 2)) {
				switch (m_orient) {
				case Horizontal:
					plx_vert_ind(&dr, PLX_VERT, x, pos.y + (m_barsize/2 + 0.0f), pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT, x, pos.y - (m_barsize/2 + 0.0f), pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT, x+m_barcode[i], pos.y + (m_barsize/2 + 0.0f), pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT_EOS, x+m_barcode[i], pos.y - (m_barsize/2 + 0.0f), pos.z+0.1f, color);
					break;

				case Vertical:
					plx_vert_ind(&dr, PLX_VERT, pos.x + (m_barsize/2 + 0.0f), x, pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT, pos.x - (m_barsize/2 + 0.0f), x, pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT, pos.x + (m_barsize/2 + 0.0f), x+m_barcode[i], pos.z+0.1f, color);
					plx_vert_ind(&dr, PLX_VERT_EOS, pos.x - (m_barsize/2 + 0.0f), x+m_barcode[i], pos.z+0.1f, color);
					break;
				}
			}
				
			x += m_barcode[i];
			i = (i+1) % 64;
		}
	}
}

void Topbar::nextFrame(uint64 tm) {
	m_frame++;

	// Animate bar size
	if (m_frame < 15)
		m_barsize = m_frame * m_barsizemax / 15.0f;
	else
		m_barsize = m_barsizemax;

	// Animate bar/barcode alpha
	if (m_frame >= 7 && m_frame < (7+15)) {
		m_baralpha -= (1.0f - 0.1f) / 15.0f;
		m_barcodealpha += 0.5f / 15.0f;
	}

	// Animate barcode
	if (m_frame >= 7) {
		m_barcodeoffs += m_barcodespeed;
		if (m_barcodespeed < 0) {
			while (m_barcodeoffs <= -m_barcodesize)
				m_barcodeoffs += m_barcodesize;
		} else {
			while (m_barcodeoffs >= m_barcodesize)
				m_barcodeoffs -= m_barcodesize;
		}
	}

	Drawable::nextFrame(tm);
}

