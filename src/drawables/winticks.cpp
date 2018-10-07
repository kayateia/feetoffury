/* 
   Feet of Fury
   
   drawables/winticks.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "winticks.h"

WinTicks::WinTicks(Texture * txr, bool left_side) {
	m_texture = txr;
	assert(m_texture != NULL);

	m_wins = 0;
	m_left = left_side;
}

WinTicks::~WinTicks() {
}

void WinTicks::draw(ObjType list) {
	if (list != Trans)
		return;

	// Basic GL parameters
	m_texture->select();

	// Are we facing left or right?
	float dx;
	if (m_left)
		dx = -32.0f;
	else
		dx = 32.0f;

	// Now for the number of wins...
	Vector t = getPosition();
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	uint32 color = getColor();
	
	float uo, x = t.x, y = t.y, z = t.z;
	for (int i=0; i<2; i++) {
		if (i < m_wins) {
			// Offset of winning mark
			uo = 0.5f;
		} else {
			// Offset of non-winning mark
			uo = 0.0f;
		}

		
		plx_vert_ifd(&dr, PLX_VERT,     x-16, y+16, z, color, uo+0.0f, 0.5f);
		plx_vert_ifd(&dr, PLX_VERT,     x-16, y-16, z, color, uo+0.0f, 0.0f);
		plx_vert_ifd(&dr, PLX_VERT,     x+16, y+16, z, color, uo+0.5f, 0.5f);
		plx_vert_ifd(&dr, PLX_VERT_EOS, x+16, y-16, z, color, uo+0.5f, 0.0f);

		x += dx;
	}
}

void WinTicks::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
}

