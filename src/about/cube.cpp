/*
   Feet of Fury

   about/cube.c

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"

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

RefPtr<Texture> faces[6];
static float tz, r;

void cubeInit() {
	faces[0] = res.getTexture(Res::About, "bard", false);
	faces[1] = res.getTexture(Res::About, "foxx", false);
	faces[2] = res.getTexture(Res::About, "geki", false);
	faces[3] = res.getTexture(Res::About, "rx", false);
	faces[5] = res.getTexture(Res::About, "logo", false);
	faces[4] = faces[5];
	tz = -300.0f;
	r = 0.0f;
}

void cubeShutdown() {
	for (int i=0; i<6; i++)
		faces[i] = NULL;
}

static inline float tabs(float a) { return a < 0.0f ? -a : a; }
void cubeDraw(float a) {
	plx_dr_state_t dr;
	uint32 color = plx_pack_color(1.0f, a, a, a);
	float lx = -50.0f, ly = -50.0f, lz = -50.0f;

	if (tabs( -12.0f - tz ) > 0.1f) {
		tz += ((-12.0f) - tz) / 64.0f;
	} else {
		tz = -12.0f;
	}
	r++;

	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, tz);
	plx_mat3d_rotate(fsin(r*(60.0f/hz)*2*M_PI / (4*480.0f)) * 180.0f, 1.0f, 0.0f, 0.0f);
	plx_mat3d_rotate(fcos(r*(60.0f/hz)*2*M_PI / (4*360.0f)) * 180.0f, 0.0f, 1.0f, 0.0f);
	plx_mat3d_rotate(fsin(r*(60.0f/hz)*2*M_PI / (4*420.0f)) * 180.0f, 0.0f, 0.0f, 1.0f);
	plx_mat3d_scale(1.7f, 1.7f, 1.7f);
	plx_mat3d_apply_all();

	plx_dr_init(&dr);

	// XXX
	//plx_cxt_culling(PLX_CULL_NONE);
	//plx_cxt_fog(PLX_FOG_TABLE);

	/* Front Face */
	plx_cxt_texture(faces[0]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);

	/* Back Face */
	plx_cxt_texture(faces[1]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);

	/* Top Face */
	plx_cxt_texture(faces[2]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);

	/* Bottom Face */
	plx_cxt_texture(faces[3]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);

	/* Right face */
	plx_cxt_texture(faces[4]);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f, -1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f, -1.0f, -1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,      1.0f,  1.0f,  1.0f, color, 0.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS,  1.0f, -1.0f,  1.0f, color, 0.0f, 1.0f);

	/* Left Face */
	plx_cxt_texture(faces[5]);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f,  1.0f, color, 1.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f, -1.0f, -1.0f, color, 0.0f, 1.0f);
	vert_ifdm3(&dr, PLX_VERT,     -1.0f,  1.0f,  1.0f, color, 1.0f, 0.0f);
	vert_ifdm3(&dr, PLX_VERT_EOS, -1.0f,  1.0f, -1.0f, color, 0.0f, 0.0f);

	// XXX
	//plx_cxt_fog(PLX_FOG_NONE);
}
