/*
   KallistiOS ##version##
   bubbles.c

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"

/*

This is a Parallax version of the "bubbles" KGL demo. The speed is about
the same, but it uses the Parallax functions instead of KGL.

*/

/* Draws a sphere out of GL_QUADS according to the syntax of glutSolidSphere

   This now uses triangle strips and PVR Direct Rendering for the best
   possible performance.

   Based very loosely on a routine from James Susinno.
*/
static float phase = 0.0f;
//static pvr_poly_cxt_t cxt;
//static pvr_poly_hdr_t hdr;
static void sphere( float radius, int slices, int stacks, float a ) {
	int	i, j;
	float	pitch, pitch2;
	float	x, y, z, g, b;
	float	yaw, yaw2;
	plx_dr_state_t	dr_state;

	/* Setup our Direct Render state: pick a store queue and setup QACR0/1 */
	plx_dr_init(&dr_state);

	/* Initialize xmtrx with the values from KGL */
	plx_mat_identity();
	plx_mat3d_apply(PLX_MAT_SCREENVIEW);
	plx_mat3d_apply(PLX_MAT_PROJECTION);
	plx_mat3d_apply(PLX_MAT_MODELVIEW);

	/* Put our own polygon header */
	plx_cxt_texture(NULL);
	//pvr_prim(&hdr, sizeof(hdr));

	/* Iterate over stacks */
	for ( i=0; i<stacks; i++ ) {
		pitch = 2*M_PI * ( (float)i/(float)stacks );
		pitch2 = 2*M_PI * ( (float)(i+1)/(float)stacks );

		/* Iterate over slices: each entire stack will be one
		   long triangle strip. */
		for ( j=0; j<=slices/2; j++ ) {
			yaw = 2*M_PI * ( (float)j/(float)slices );
			yaw2 = 2*M_PI * ( (float)(j+1)/(float)slices );

			/* x, y+1 */
			x = radius * fcos( yaw ) * fcos( pitch2 );
			y = radius * fsin( pitch2 );
			z = radius * fsin( yaw ) * fcos( pitch2 );
			plx_mat_tfip_3d(x, y, z);			/* Use ftrv to transform */
			g = a*fcos( yaw*2 ) / 4.0f + 0.25f;
			b = a*fsin( phase + pitch2 ) / 4.0f + 0.25f;
			plx_vert_fnd(&dr_state, PLX_VERT,
				x, y, z, 0.5f,
				0.0f, g, b);
                
			/* x, y */
			x = radius * fcos( yaw ) * fcos( pitch );
			y = radius * fsin( pitch );
			z = radius * fsin( yaw ) * fcos( pitch );
			plx_mat_tfip_3d(x, y, z);
			g = a*fcos( yaw*2 ) / 4.0f + 0.25f;
			b = a*fsin( phase + pitch ) / 4.0f + 0.25f;

			if (j == (slices/2)) {
				plx_vert_fnd(&dr_state, PLX_VERT_EOS,
					x, y, z, 0.5f,
					0.0f, g, b);
			} else {
				plx_vert_fnd(&dr_state, PLX_VERT,
					x, y, z, 0.5f,
					0.0f, g, b);
			}
		}
	}
}

#define SPHERE_CNT 12
static float r = 0, tz;
static inline float tabs(float a) { return a < 0.0f ? -a : a; }
static void sphere_frame_opaque(float a) {
	int i;

	if (tabs( -12.0f - tz ) > 0.1f) {
		tz += ((-12.0f) - tz) / 64.0f;
	} else {
		tz = -12.0f;
	}

	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, tz);
	plx_mat3d_rotate(r, 0.75f, 1.0f, 0.5f);
	plx_mat3d_push();

	for (i=0; i<SPHERE_CNT *3/4; i++) {
		plx_mat3d_translate(6.0f * fcos(i * 2*M_PI / SPHERE_CNT + phase), 0.0f, 6.0f * fsin(i * 2*M_PI / SPHERE_CNT + phase));
		plx_mat3d_rotate(r, 1.0f, 1.0f, 1.0f);
		sphere(0.8f, 14, 14, a);
		if (i < (SPHERE_CNT-1))
			plx_mat3d_peek();
	}
	plx_mat3d_pop();

	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, 0.0f, tz);
	plx_mat3d_rotate(-r, 0.75f, 1.0f, 0.5f);
	plx_mat3d_push();

	for (i=0; i<SPHERE_CNT *3/4; i++) {
		plx_mat3d_translate(4.0f * fcos(i * 2*M_PI / SPHERE_CNT - phase), 0.0f, 3.0f * fsin(i * 2*M_PI / SPHERE_CNT - phase));
		plx_mat3d_rotate(r, 1.0f, 1.0f, 1.0f);
		sphere(0.8f, 14, 14, a);
		if (i < (SPHERE_CNT-1))
			plx_mat3d_peek();
	}
	plx_mat3d_pop();

	r++;
	phase += 2*M_PI / 120.0f;
}

static float fogcolor[4] = { 0.2, 0.2, 0.3, 1.0 };
void bubblesInit() {
	/* float fc[4] = { 0.0, 0.0, 0.0, 0.0 };

	pvr_fog_far_depth(0.1f);
	pvr_fog_table_linear(0.0f, 0.1f); */
	/* glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fc);
	glFogf(GL_FOG_DENSITY, 0.2f);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 15.0f);
	glDisable(GL_FOG); */

	/* pvr_poly_cxt_col(&cxt, PVR_LIST_OP_POLY);
	cxt.gen.culling = PVR_CULLING_NONE;
	cxt.gen.fog_type = PVR_FOG_TABLE;
	pvr_poly_compile(&hdr, &cxt); */

	tz = -300.0f;
}

void bubblesDraw(float a) {
	// float fc[4] = { fogcolor[0]*a, fogcolor[1]*a, fogcolor[2]*a, 1.0f };
	// XXX
	// pvr_fog_table_color(1.0f, fogcolor[0]*a, fogcolor[1]*a, fogcolor[2]*a);
	// glFogfv(GL_FOG_COLOR, fc);
	glClearColor(fogcolor[0]*a, fogcolor[1]*a, fogcolor[2]*a, 1.0f);
	sphere_frame_opaque(a);
}

