/* 
   Feet of Fury
   
   drawables/cube.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "cube.h"

Cube::Cube(ObjType list, Texture * texture) {
	m_list = list;
	m_texture = texture;
}

Cube::~Cube() {
}

void Cube::draw(ObjType list) {
	if (list != m_list)
		return;

	pushTransformMatrix();
	drawCube();
	popTransformMatrix();
}

#define vert_ifdm3(st, flg, x, y, z, color, u, v) do { \
	float tx = x, ty = y, tz = z; \
	plx_mat_tfip_3d(tx, ty, tz); \
	if (tz <= 0.0f) { \
		tx = lx; ty = ly; tz = lz; \
	} else { \
		lx = tx; ly = ty; lz = tz; \
	} \
	plx_vert_ifd(st, flg, tx, ty, tz, color, u, v); \
} while (0)

void Cube::drawCube() {
	plx_dr_state_t dr;
	Color c = getColor();
	uint32 color = c;
	float lx = -50.0f, ly = -50.0f, lz = -50.0f;
	
	plx_cxt_texture(m_texture);
	if (m_list == Trans && c.a != 1.0f)
		plx_cxt_culling(PLX_CULL_NONE);
	else
		plx_cxt_culling(PLX_CULL_CCW);
	plx_mat3d_apply_all();

	plx_dr_init(&dr);
	
	/* Front Face */
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);

	/* Back Face */
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);

	/* Top Face */
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);

	/* Bottom Face */
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);

	/* Right face */
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);

	/* Left Face */
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
}

void Cube::setTexture(Texture * txr) {
	m_texture = txr;
}



