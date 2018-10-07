/* 
   Feet of Fury
   
   main/loadhoops.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "loadhoops.h"

/* Implements the rotating "hoops" for the loading screen */

LoadHoops::LoadHoops() {
	m_frame = 0;
}

LoadHoops::~LoadHoops() {
}

void LoadHoops::draw(ObjType list) {
	if (list != Opaque)
		return;

	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);
	drawOuterRing();
	drawMidRing();
	drawInnerRing();
}

void LoadHoops::nextFrame(uint64 tm) {
	m_frame++;
}

// Methods to draw the various rings
void LoadHoops::drawOuterRing() {
	// Do the rotations
	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, -10.0f);
	plx_mat3d_rotate(m_frame * (60.0f/hz) * 2, 0.0f, 1.0f, -0.5f);
	plx_mat3d_rotate(m_frame * (60.0f/hz) * 2.2f, 0.0f, 0.0f, -1.0f);
	plx_mat3d_apply_all();

	// Draw the ring
	const int segcnt = 120;
	const float rad = 6.0f;
	int i;
	uint32 color = getColor() * Color(0.05f, 0.4f, 0.58f);
	plx_dr_state_t dr;

	plx_dr_init(&dr);

	for (i=0; i<=segcnt; i++) {
		// Get the x,y for the current outer point
		float t = i * ((2*M_PI)/segcnt);
		float cox = fcos(t) * rad;
		float coy = fsin(t) * rad;

		// Current inner point
		float cix = fcos(t) * rad * 0.9f;
		float ciy = fsin(t) * rad * 0.9f;

		// Adjust for peaks around the corners
		if ( (i % (segcnt/8)) == 0 ) {
			cix *= 0.9f;
			ciy *= 0.9f;
		}

		// Do we need to start a new strip?
		if (i>0 && i<segcnt && (i % 40) == 0) {
			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT_EOS, cix, ciy, 0.0f, color);

			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT, cix, ciy, 0.0f, color);
		} else if (i == segcnt) {
			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT_EOS, cix, ciy, 0.0f, color);
		} else {
			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT, cix, ciy, 0.0f, color);
		}
	}
}

void LoadHoops::drawMidRing() {
	// Do the rotations
	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, -10.0f);
	plx_mat3d_rotate(m_frame * (60.0f/hz) * 2, 1.0f, 0.0f, -0.9f);
	plx_mat3d_apply_all();

	// Draw the ring
	const int segcnt = 120;
	const float rad = 4.0f;
	int i;
	uint32 color = getColor() * Color(0.09f, 0.56f, 0.79f);
	plx_dr_state_t dr;

	plx_dr_init(&dr);

	for (i=0; i<=segcnt; i++) {
		// Get the x,y for the current outer point
		float t = i * ((2*M_PI)/segcnt);
		float cox = fcos(t) * rad;
		float coy = fsin(t) * rad;

		// Current inner point
		float cix = fcos(t) * rad * 0.9f;
		float ciy = fsin(t) * rad * 0.9f;

		// If we're at a corner, then skip some
		if (i == segcnt || ((i + segcnt/16) % (segcnt/8)) == 0 ) {
			// End this strip
			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT_EOS, cix, ciy, 0.0f, color);

			i += 4;
		} else {
			// Draw two points
			plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
			plx_vert_indm3(&dr, PLX_VERT, cix, ciy, 0.0f, color);
		}
	}
}

void LoadHoops::drawInnerRing() {
	// Do the rotations
	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, -10.0f);
	plx_mat3d_rotate(m_frame * (60.0f/hz) * 2, 1.0f, 1.0f, -3.0f);
	plx_mat3d_apply_all();

	// Draw the ring
	const int segcnt = 120;
	const float rad = 2.0f;
	int i;
	uint32 color = getColor() * Color(0.07f, 0.57f, 0.33f);
	plx_dr_state_t dr;

	plx_dr_init(&dr);

	for (i=0; i<=segcnt; i++) {
		// Get the x,y for the current outer point
		float t = i * ((2*M_PI)/segcnt);
		float cox = fcos(t) * rad;
		float coy = fsin(t) * rad;

		// Current inner point
		float cix = fcos(t) * rad * 0.9f;
		float ciy = fsin(t) * rad * 0.9f;

		// Draw two points
		plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
		plx_vert_indm3(&dr, PLX_VERT, cix, ciy, 0.0f, color);

		// Get the next points
		t = (i+1) * ((2*M_PI)/segcnt);
		cox = fcos(t) * rad;
		coy = fsin(t) * rad;
		cix = fcos(t) * rad * 0.9f;
		ciy = fsin(t) * rad * 0.9f;

		// Draw those too
		plx_vert_indm3(&dr, PLX_VERT, cox, coy, 0.0f, color);
		plx_vert_indm3(&dr, PLX_VERT_EOS, cix, ciy, 0.0f, color);

		// Skip some space
		i += 2;
	}
}

