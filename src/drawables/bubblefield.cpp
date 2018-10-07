/*

  Feet of Fury

  drawables/bubblefield.cc

  Copyright (C)2001,2002,2003 Cryptic Allusion, LLC

  Wouldn't ya know it? This damned star field has to make at least one more
  appearance before I'll let it rest ;-). It's been changed quite a bit
  though to be more aquatic.

 */

#include "global.h"
#include "bubblefield.h"

#define zk 64
#define MAXX ((640*(zk+640))/zk)
#define MAXY ((480*(zk+480))/zk)

const float BubbleField::m_max_bubble_size = 64.0f;
const int BubbleField::m_delta = 8;
//const int BubbleField::m_randarray_len = 573;

/* init stars so that they will be anywhere on the screen, but will start out
   as a single dot in the middle because of the Z coord.

   This routine has been mathmatically optimized to produce the best possible
   spread of stars on the given starfield parameters by solving the
   perspective equation for X and Y. */
BubbleField::BubbleField(Texture * txr, int nb) {
	int i;

	m_txr = txr;
	m_nb = nb;

	// Init the bubble array
	m_bubbles = new OneBubble[m_nb];
	for (i=0; i<m_nb; i++) {
	#define fabs(x) ((x) < 0 ? -(x) : (x))
		do {
			m_bubbles[i].x = randnum(MAXX) - (MAXX/2);
			m_bubbles[i].y = randnum(MAXY) - (MAXY/2);
		} while (fabs(m_bubbles[i].x) <= 5 && fabs(m_bubbles[i].y) <= 5);
		
		m_bubbles[i].z = randnum(640) + 640;
		m_bubbles[i].vx = (((float)randnum(256)) - 128.0f) / (128.0f / 1.0f);
		m_bubbles[i].vy = (((float)randnum(256)) - 128.0f) / (128.0f / 1.0f);
		m_bubbles[i].vz = ((float)randnum(256)) / (256.0f / 3.0f) + 1.0f;
		m_bubbles[i].dm = randnum(12) + 8;
		m_bubbles[i].w = 1.0f / (randnum(255) + 1);
	}

	// Initialize our random array	
	/* m_randcur = 0;
	m_randarray = new int[m_randarray_len];
	for (i=0; i<m_randarray_len; i++)
		m_randarray[i] = randnum(16) - 8; */
}

BubbleField::~BubbleField() {
	// delete[] m_randarray;
	delete[] m_bubbles;
}

/* A faster random number function that just pulls from a pool of random
   numbers. It'll be random enough for what we're doing here. */
/* int BubbleField::pullRand() {
	int r;

	r = m_randarray[m_randcur];
	m_randcur++;
	if (m_randcur >= m_randarray_len)
		m_randcur = 0;
	return r;
} */

/* Draws a bubble */
void BubbleField::drawBubble(plx_dr_state_t * dr, float x, float y, float z, float wx, float wy, float dist) {
//	float size = m_max_bubble_size * dist / 256.0f;

//	plx_spr_fnd(dr, size * wx, size * wy, x, y, pz + (1.0f - (z/640.0f)),
//		dist * getAlpha() * 255.0f / 256.0f, 1.0f, 1.0f, 1.0f);
//	plx_spr_fnd(dr, size * wx, size * wy, x, y, pz,
//		dist * getAlpha() * 255.0f / 256.0f, 1.0f, 1.0f, 1.0f);
}

void BubbleField::draw(ObjType list) {
	if (list != Trans)
		return;

	int	i;
	float	x1, y1, xn, yn, zn, c;
	plx_dr_state_t	dr;
	float pz = getPosition().z;
	Color tint = getColor();

	m_txr->select();
	plx_dr_init(&dr);

	/* Calculate each bubble's position and plot it on screen */
	for (i=0; i<m_nb; i++) {
		/* Calculate bubble perspective */
		xn = m_bubbles[i].x; yn = m_bubbles[i].y; zn = m_bubbles[i].z;
		x1 = xn*zk / (zn + zk);
		y1 = yn*zk / (zn + zk);

		if (x1>(-320 - m_max_bubble_size) && y1>(-240 - m_max_bubble_size)
				&& x1<320 && y1<240
				&& zn >= -320) {
			c = (zn * 256/640);
			if (c > 255) c = 255;
			xn = (fsin(m_bubbles[i].w) / 16.0f) + 1.0f;
			yn = (fcos(m_bubbles[i].w) / 16.0f) + 1.0f;

			float dist = 255 - c;
			float size = m_max_bubble_size * dist / 256.0f;

			plx_spr_fnd(&dr,
				xn*size,
				yn*size,
				x1+320, y1+240, pz + 0.1f * (dist/256.0f),
				dist * tint.a / 256.0f, tint.r, tint.g, tint.b);
			// drawBubble(&dr, x1+320, y1+240, 1.0f, xn, yn, 255 - c);
		} else {
			do {
				m_bubbles[i].x = randnum(MAXX) - (MAXX/2);
				m_bubbles[i].y = randnum(MAXY) - (MAXY/2);
			} while (fabs(m_bubbles[i].x) <= 5 && fabs(m_bubbles[i].y) <= 5);
			m_bubbles[i].z = 640;
		}

		/* Move star's Z coord to show motion inward */
		m_bubbles[i].z -= m_bubbles[i].vz;

		/* Adjust bubble warp */
		m_bubbles[i].w += 1.0f / 128.0f;
		while (m_bubbles[i].w >= 1.0f)
			m_bubbles[i].w -= 1.0f;

		/* Check to see if we need to redo the motion vector */
		if (!(m_bubbles[i].dm--)) {
			m_bubbles[i].vx = (((float)randnum(256)) - 128.0f) / (128.0f / 4.0f);
			m_bubbles[i].vy = (((float)randnum(256)) - 128.0f) / (128.0f / 4.0f);
			m_bubbles[i].dm = randnum(8);
		}

		/* Movement vector for bubbly movement */
		m_bubbles[i].x += m_bubbles[i].vx;
		m_bubbles[i].y += m_bubbles[i].vy;
	}
}


