/*

   Feet of Fury

   drawables/starfield.cc

   Copyright (C)2000-2002 Cryptic Allusion, LLC

*/

#include "global.h"
#include "starfield.h"

/* init stars so that they will be anywhere on the screen, but will start out
   as a single dot in the middle because of the Z coord.

   This routine has been mathmatically optimized to produce the best possible
   spread of stars on the given starfield parameters by solving the
   perspective equation for X and Y. */

#define zk 64
#define MAXX ((640*(zk+640))/zk)
#define MAXY ((480*(zk+480))/zk)

StarField::StarField(int ns) {
	int i;

	m_ns = ns;
	m_stars = new Vector[m_ns];
	for (i=0; i<m_ns; i++) {
		m_stars[i].x = randnum(MAXX)-(MAXX/2);
		m_stars[i].y = randnum(MAXY)-(MAXY/2);
		m_stars[i].z = randnum(640);
	}
}

StarField::~StarField() {
	delete[] m_stars;
}

void StarField::draw(ObjType list) {
	if (list != Opaque)
		return;

	int i;
	float x1, y1, xn, yn, zn, a, r, g, b;

	Color col = getColor();
	Vector pos = getPosition();

	/* Send polygon header to the TA using store queues */
	plx_cxt_texture(NULL);

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	/* Calculate each star's position and plot it on screen */
	for (i=0; i<m_ns; i++) {
		/* Calculate star perspective */
		xn = m_stars[i].x; yn = m_stars[i].y; zn = m_stars[i].z;
		x1 = xn*zk / (zn + zk);
		y1 = yn*zk / (zn + zk);

		if (x1>-320 && y1>-240 && x1<320 && y1<240) {
			a = (1.0f - (zn * 1.0f/640));
			r = col.r*a;
			g = col.g*a;
			b = col.b*a;

			plx_vert_fnd(&dr, PLX_VERT, x1+320, y1+240+2, pos.z, 1.0f, r, g, b);
			plx_vert_fnd(&dr, PLX_VERT, x1+320, y1+240, pos.z, 1.0f, r, g, b);
			plx_vert_fnd(&dr, PLX_VERT, x1+320+2, y1+240+2, pos.z, 1.0f, r, g, b);
			plx_vert_fnd(&dr, PLX_VERT_EOS, x1+320+2, y1+240, pos.z, 1.0f, r, g, b);
			// poly_pnt(x1+320, y1+240, 1.0f, 4.0f, 255 - c);
		} else {
			m_stars[i].z = 640;
		}

		/* Move star's Z coord to show motion inward */
		m_stars[i].z-=8;
	}
}
