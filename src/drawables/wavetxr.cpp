/*
   Feet of Fury

   drawables/wavetxr.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

/*

  This is an effect I originally called the "water banner". It basically
  takes a fixed size textured polygon and breaks it up into a grid of
  polygons which are warped to look like they're being rippled in water
  (like Ecco's menus).

  I originally wrote this for a DC demo called "Aquatic" that never got
  released. Most of the interesting pieces of that demo are present in
  Feet of Fury in one place or another (wave texture, bubblefield, etc).

  This was ported up from a raw PVR effect, so I kept it that way.

 */

#include "global.h"
#include "wavetxr.h"

#define STEP	12
#define FREQ	2

/* Offset table, precalculated */
struct offsval_t {
	float		vx, vy;		/* Movement vector from the center */
	float		d;		/* Distance from center */
	int		f;		/* Filler */
};
static int initialized = 0;
static offsval_t offtab[STEP+1][STEP+1];

static void init_tables() {
	int	x, y;
	float	d, c;

	c = STEP / 2.0f;
	for (y=0; y<=STEP; y++)
		for (x=0; x<=STEP; x++) {
			/* Grab the distance from the center */
			d = fsqrt((c-y)*(c-y)+(c-x)*(c-x));

			/* Store in the table */
			if (d == 0) {
				offtab[y][x].vx = offtab[y][x].vy = 0.0f;
			} else {
				offtab[y][x].vx = (c - x) / d;
				offtab[y][x].vy = (c - y) / d;
			}
			offtab[y][x].d = d;
			offtab[y][x].f = 0;
		}
}

WaveTexture::WaveTexture(Texture *txr, int list) {
	// One-time init that must be present for all wave textures
	if (!initialized) {
		init_tables();
		initialized = 1;
	}

	assert(txr != NULL);
	m_txr = txr;

	m_phase = 0.0f;
	m_mag = 1.0f;
	m_list = list;
}

WaveTexture::~WaveTexture() {
}

void WaveTexture::setTexture(Texture * t) {
	assert( t != NULL );
	m_txr = t;
}

void WaveTexture::draw(ObjType list) {
	if (list != m_list)
		return;

	float		x, y, dx, dy;
	float		u, v, du, dv;
	float		x1, y1;
	float		ox, oy, t, ct;
	float		tw, th;
	int		ix, iy;
	const Vector &	sc = getScale();
	Vector		pos = getPosition();
	Color		tint = getColor();
	plx_dr_state_t	dr;

	// Setup our Direct Render state
	plx_dr_init(&dr);

	// Send polygon header
	m_txr->select();

	tw = m_txr->getW() * sc.x;
	th = m_txr->getH() * sc.y;
	x1 = pos.x - tw/2;
	y1 = pos.y - th/2;
	
	dx = (float)tw / STEP;
	dy = (float)th / STEP;
	du = 1.0f / STEP;
	dv = 1.0f / STEP;
	for (iy=0, y=0, v=0; y<th; y+=dy, v+=dv, iy++) {
		for (ix=0, x=0, u=0; x<=tw; x+=dx, u+=du, ix++) {
			// x2, y2      x4, y4
			// x1, y1      x3, y3

			// x1, y1
			t = fsin((offtab[iy+1][ix].d + m_phase)*FREQ);
			ox = offtab[iy+1][ix].vx * t * m_mag;
			oy = offtab[iy+1][ix].vy * t * m_mag;
			ct = 1.0f + (m_mag/10.0f) * (t - 1.0f)*0.6f/2.0f;
			plx_vert_ifd(&dr, PLX_VERT, (ox+x)+x1, (oy+y+dy)+y1, pos.z,
				tint * Color(ct, ct, ct), u, v+dv);

			// x2, y2
			t = fsin((offtab[iy][ix].d + m_phase)*FREQ);
			ox = offtab[iy][ix].vx * t * m_mag;
			oy = offtab[iy][ix].vy * t * m_mag;
			ct = 1.0f + (m_mag/10.0f) * (t - 1.0f)*0.6f/2.0f;
			plx_vert_ifd(&dr, x == tw ? PLX_VERT_EOS : PLX_VERT,
				(ox+x)+x1, (oy+y)+y1, pos.z,
				tint * Color(ct, ct, ct), u, v);
		}
	}
}

void WaveTexture::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	m_phase -= (1.0f / FREQ) / (2 * M_PI);
}
