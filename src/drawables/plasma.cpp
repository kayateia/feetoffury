/*
   Feet of Fury

   drawables/plasma.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "plasma.h"
#include "hsv.h"

/*
   Simple plasma effect from Stars 

   This will be used as the background texture for the bubbles effect.
   We use an especially nasty trick here to make the PVR do the dirty
   work for what used to be an impossibly slow task. Basically the
   plasma generator only generates a tiny 64x64 window of plasma, but
   into a twiddled texture. The PVR then uses this texture with
   bi-linear filtering to produce a nice smooth background.

   As far as I know, this is one of the few real "demo" effects I've
   seen for the DC so far, meaning a trick that makes the hardware do
   something it wasn't really meant to do. There are probably others
   that people have done but it's hard to tell without the source to
   a lot of it.
 */

static short pcos[512], psin[512];
static int initted = 0;
static void plasma_init() {
	if (initted)
		return;
	for (int i=0; i<512; i++) {
		pcos[i] = (short)(fcos(i*2*(2*M_PI/512.0f)) * 256.0f);
		psin[i] = (short)(fsin(i*2*(2*M_PI/512.0f)) * 256.0f);
	}
	initted = 1;
}

Plasma::Plasma(int list) {
	m_list = list;
	m_xang = m_yang = 0;
	m_xamp = m_yamp = 256;
	m_hsv = 0.0f;

	m_txr[0] = new Texture(64, 64, Texture::FmtRGB565);
	m_txr[1] = new Texture(64, 64, Texture::FmtRGB565);
	m_w = m_h = 64.0f;

	m_txr_cur = 0;
	
	plasma_init();
}

Plasma::~Plasma() {
}

void Plasma::setSize(float w, float h) {
	m_w = w;
	m_h = h;
}

void Plasma::draw(ObjType list) {
	if (list != m_list)
		return;

	Vector p = getPosition();
	Color c = getColor();
	
	float r, g, b;
	hsvSpectrum(m_hsv, r, g, b);
	r *= c.r; g *= c.g; b *= c.b;

	plx_cxt_texture(m_txr[m_txr_cur]);
	plx_cxt_send(list);

	plx_vertex_t	v;
	v.flags = PLX_VERT;
	v.x = p.x - m_w/2;
	v.y = p.y + m_h/2;
	v.z = p.z;
	v.u = 0.0f;
	v.v = 1.0f;
	//v.argb = PVR_PACK_COLOR(1.0f, 2*55/256.0f, 2*77/256.0f, 2*142/256.0f);
	v.argb = plx_pack_color(1.0f, r, g, b);
	v.oargb = 0;
	plx_prim(&v, sizeof(v));

	v.y = p.y - m_h/2;
	v.v = 0.0f;
	plx_prim(&v, sizeof(v));

	v.x = p.x + m_w/2;
	v.y = p.y + m_h/2;
	v.u = 1.0f; v.v = 1.0f;
	plx_prim(&v, sizeof(v));

	v.flags = PLX_VERT_EOS;
	v.y = p.y - m_h/2;
	v.v = 0.0f;
	plx_prim(&v, sizeof(v));
}

void Plasma::nextFrame(uint64 tm) {
	int	x,y,p,q, r;

	m_xang++; m_yang--;
	m_xang &= 511; m_yang &= 511;
	m_xamp = 2*pcos[m_yang];
	m_yamp = 2*psin[m_xang];

	uint16	*vrout = (uint16 *)m_txr[m_txr_cur]->canvasLock();

	// Redo the plasma texture for the next frame
	for (y=30;y<30+64;y++) {
		for (x=30;x<30+64;x++) {
			p=x+(psin[(m_xang+(y*2+y))&511]*m_xamp/256);
			q=y+(pcos[(m_yang+(x*4  ))&511]*m_yamp/256);
			r=((p+q)&255);
			if (r >= 128)
				r = 255 - r;
			p = (r >> (8 - 5));
			r = (r >> (8 - 6));
			vrout[(y-30)*64+(x-30)] =
				(p << 11) | (r << 5) | p;
		}
	}

	m_txr[m_txr_cur]->canvasUnlock();

	m_hsv += 1.0f / (8*hz);
	while (m_hsv >= 1.0f)
		m_hsv -= 1.0f;

	m_txr_cur ^= 1;
}

