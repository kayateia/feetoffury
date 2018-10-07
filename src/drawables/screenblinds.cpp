/* 
   Feet of Fury
   
   drawables/screenblinds.cc
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "screenblinds.h"

ScreenBlinds::ScreenBlinds(bool closing) {
	m_frame = 0;
	m_closing = closing;
}

ScreenBlinds::~ScreenBlinds() {
}

void ScreenBlinds::draw(ObjType list) {
	// No textures needed
	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(list);

	Color col = getColor();

	float width;

	if (m_closing)
		width = m_frame * 640.0f / (hz/2);
	else
		width = 640.0f - (m_frame * 640.0f / (hz/2));

	if (list == Opaque) {
		int type = 0;
		for (float y = 0.0f; y < 480.0f; y += 32.0f) {
			if (!type)
				drawLeftBar(col, y, width);
			else
				drawRightBar(col, y, width);
			type ^= 1;
		}
	} else if (list == Trans) {
		int type = 0;
		for (float y = 0.0f; y < 480.0f; y += 32.0f) {
			if (!type)
				drawLeftTip(col, y, width);
			else
				drawRightTip(col, y, width);
			type ^= 1;
		}
	}
}

void ScreenBlinds::drawLeftBar(const Color & col, float y, float width) {
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	uint32 color = col;

	plx_vert_ind(&dr, PLX_VERT, 0.0f, y+32.0f, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT, 0.0f, y, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT, width, y+32.0f, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT_EOS, width, y, 900.0f, color);
}

void ScreenBlinds::drawLeftTip(const Color & col, float y, float width) {
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	uint32 color1 = col;
	uint32 color2 = Color(0.0f, col.r, col.g, col.b);

	plx_vert_ind(&dr, PLX_VERT, width, y+32.0f, 900.0f, color1);
	plx_vert_ind(&dr, PLX_VERT, width, y, 900.0f, color1);
	plx_vert_ind(&dr, PLX_VERT, width+64.0f, y+32.0f, 900.0f, color2);
	plx_vert_ind(&dr, PLX_VERT_EOS, width+64.0f, y, 900.0f, color2);
}

void ScreenBlinds::drawRightBar(const Color & col, float y, float width) {
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	uint32 color = col;

	float x1 = 640.0f - width;
	plx_vert_ind(&dr, PLX_VERT, x1, y+32.0f, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT, x1, y, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT, 640.0f, y+32.0f, 900.0f, color);
	plx_vert_ind(&dr, PLX_VERT_EOS, 640.0f, y, 900.0f, color);
}

void ScreenBlinds::drawRightTip(const Color & col, float y, float width) {
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	uint32 color2 = col;
	uint32 color1 = Color(0.0f, col.r, col.g, col.b);
	float x1 = 640.0f - width;

	plx_vert_ind(&dr, PLX_VERT, x1-64.0f, y+32.0f, 900.0f, color1);
	plx_vert_ind(&dr, PLX_VERT, x1-64.0f, y, 900.0f, color1);
	plx_vert_ind(&dr, PLX_VERT, x1, y+32.0f, 900.0f, color2);
	plx_vert_ind(&dr, PLX_VERT_EOS, x1, y, 900.0f, color2);
}



void ScreenBlinds::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;

	if (m_frame == (hz/2)) {
		complete(this);
	}
}

