/*
   Feet of Fury

   drawables/bubble.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "bubble.h"

Bubble::Bubble(Bubble::ColorScheme color) {
	m_phase = 0.0f;
	m_radius = 1.0f;
	m_stacks = 20;
	m_slices = 20;
	m_color = color;
	m_throbber = new BubbleThrobber();
	animAdd(m_throbber);

	/* pvr_poly_cxt_col(&m_cxt, PVR_LIST_TR_POLY);
	m_cxt.gen.culling = PVR_CULLING_NONE;
	pvr_poly_compile(&m_hdr, &m_cxt); */
}

Bubble::~Bubble() { }

void Bubble::draw(ObjType list) {
	pushTransformMatrix();
	if (list == Trans) {
		switch (m_color) {
		case BlueGreen:
			drawSphereBlue();
			break;
		case Gray:
			drawSphereGray();
			break;
		case RedGreen:
			drawSphereRed();
			break;
		}
	}
	Drawable::draw(list);
	popTransformMatrix();
}

void Bubble::nextFrame(uint64 t) {
	m_phase += 2.0*M_PI / 240.0f;
	Drawable::nextFrame(t);
}

/*
   Draws a sphere out of triangle strips
   (based very loosely on a routine from James Susinno)
*/
void Bubble::drawSphereBlue() {
	int i, j;
	float alpha = getColor().a * 0.5f;
	plx_dr_state_t dr_state;

	/* Setup our Direct Render state */
	plx_dr_init(&dr_state);

	/* Initialize xmtrx with the values from Parallax */
	plx_mat3d_apply_all();

	/* Put our own polygon header */
	// pvr_prim(&m_hdr, sizeof(m_hdr));

	/* Iterate over stacks */
	for ( i=0; i<m_stacks; i++ ) {
		float pitch, pitch2;
		pitch = 2*M_PI * ( (float)i/(float)m_stacks );
		pitch2 = 2*M_PI * ( (float)(i+1)/(float)m_stacks );

		/* Iterate over slices: each entire stack will be one
		   long triangle strip. */
		for ( j=0; j<=m_slices/2; j++ ) {
			float x, y, z, g, b;
			float yaw, yaw2;
            
			yaw = 2*M_PI * ( (float)j/(float)m_slices );
			yaw2 = 2*M_PI * ( (float)(j+1)/(float)m_slices );

			/* x, y+1 */
			x = m_radius * fcos( yaw ) * fcos( pitch2 );
			y = m_radius * fsin( pitch2 );
			z = m_radius * fsin( yaw ) * fcos( pitch2 );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 2.0f + 0.5f;
			b = fsin( m_phase + pitch2 ) / 2.0f + 0.5f;
			plx_vert_fnd(&dr_state, PLX_VERT,
				x, y, z < 0.0001 ? 0.0001 : z,
				alpha, 0.0f, g, b);
                
			/* x, y */
			x = m_radius * fcos( yaw ) * fcos( pitch );
			y = m_radius * fsin( pitch );
			z = m_radius * fsin( yaw ) * fcos( pitch );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 2.0f + 0.5f;
			b = fsin( m_phase + pitch ) / 2.0f + 0.5f;
			if (j == (m_slices/2)) {
				plx_vert_fnd(&dr_state, PLX_VERT_EOS,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, 0.0f, g, b);
			} else {
				plx_vert_fnd(&dr_state, PLX_VERT,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, 0.0f, g, b);
			}
		}
	}
}

// Evil cut and paste copy (to maintain the DR speed) for gray spheres
void Bubble::drawSphereGray() {
	int i, j;
	float alpha = getColor().a * 0.5f;
	plx_dr_state_t dr_state;

	/* Setup our Direct Render state */
	plx_dr_init(&dr_state);

	/* Initialize xmtrx with the values from Parallax */
	plx_mat3d_apply_all();

	/* Put our own polygon header */
	// pvr_prim(&m_hdr, sizeof(m_hdr));

	/* Iterate over stacks */
	for ( i=0; i<m_stacks; i++ ) {
		float pitch, pitch2;
		pitch = 2*M_PI * ( (float)i/(float)m_stacks );
		pitch2 = 2*M_PI * ( (float)(i+1)/(float)m_stacks );

		/* Iterate over slices: each entire stack will be one
		   long triangle strip. */
		for ( j=0; j<=m_slices/2; j++ ) {
			float x, y, z, g, b;
			float yaw, yaw2;
            
			yaw = 2*M_PI * ( (float)j/(float)m_slices );
			yaw2 = 2*M_PI * ( (float)(j+1)/(float)m_slices );

			/* x, y+1 */
			x = m_radius * fcos( yaw ) * fcos( pitch2 );
			y = m_radius * fsin( pitch2 );
			z = m_radius * fsin( yaw ) * fcos( pitch2 );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 4.0f + 0.25f;
			plx_vert_fnd(&dr_state, PLX_VERT,
				x, y, z < 0.0001 ? 0.0001 : z,
				alpha, g, g, g);
                
			/* x, y */
			x = m_radius * fcos( yaw ) * fcos( pitch );
			y = m_radius * fsin( pitch );
			z = m_radius * fsin( yaw ) * fcos( pitch );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 4.0f + 0.25f;
			if (j == (m_slices/2)) {
				plx_vert_fnd(&dr_state, PLX_VERT_EOS,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, g, g, g);
			} else {
				plx_vert_fnd(&dr_state, PLX_VERT,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, g, g, g);
			}
		}
	}
}


// Yet another evil cut and paste copy for red/yellow spheres
void Bubble::drawSphereRed() {
	int i, j;
	float alpha = getColor().a * 0.5f;
	plx_dr_state_t dr_state;

	/* Setup our Direct Render state */
	plx_dr_init(&dr_state);

	/* Initialize xmtrx with the values from Parallax */
	plx_mat3d_apply_all();

	/* Put our own polygon header */
	// pvr_prim(&m_hdr, sizeof(m_hdr));

	/* Iterate over stacks */
	for ( i=0; i<m_stacks; i++ ) {
		float pitch, pitch2;
		pitch = 2*M_PI * ( (float)i/(float)m_stacks );
		pitch2 = 2*M_PI * ( (float)(i+1)/(float)m_stacks );

		/* Iterate over slices: each entire stack will be one
		   long triangle strip. */
		for ( j=0; j<=m_slices/2; j++ ) {
			float x, y, z, g, b;
			float yaw, yaw2;
            
			yaw = 2*M_PI * ( (float)j/(float)m_slices );
			yaw2 = 2*M_PI * ( (float)(j+1)/(float)m_slices );

			/* x, y+1 */
			x = m_radius * fcos( yaw ) * fcos( pitch2 );
			y = m_radius * fsin( pitch2 );
			z = m_radius * fsin( yaw ) * fcos( pitch2 );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 2.0f + 0.5f;
			b = fsin( m_phase + pitch2 ) / 2.0f + 0.5f;
			plx_vert_fnd(&dr_state, PLX_VERT,
				x, y, z < 0.0001 ? 0.0001 : z,
				alpha, g, b, 0.0f);
                
			/* x, y */
			x = m_radius * fcos( yaw ) * fcos( pitch );
			y = m_radius * fsin( pitch );
			z = m_radius * fsin( yaw ) * fcos( pitch );
			plx_mat_tfip_3d(x, y, z);
			g = fcos( yaw*2 ) / 2.0f + 0.5f;
			b = fsin( m_phase + pitch2 ) / 2.0f + 0.5f;
			if (j == (m_slices/2)) {
				plx_vert_fnd(&dr_state, PLX_VERT_EOS,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, g, b, 0.0f);
			} else {
				plx_vert_fnd(&dr_state, PLX_VERT,
					x, y, z < 0.0001 ? 0.0001 : z,
					alpha, g, b, 0.0f);
			}
		}
	}
}


