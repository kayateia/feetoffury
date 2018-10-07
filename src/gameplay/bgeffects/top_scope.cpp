/*
   Feet of Fury

   gameplay/bgeffects/top_scope.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "top_scope.h"
#include "hsv.h"

#include <Tiki/stream.h>

TopScope::FilterCB::FilterCB(TopScope * us) { m_us = us; }
TopScope::FilterCB::~FilterCB() { }
	
void TopScope::FilterCB::filter(int freq, int chncount, void * buffer, int smpcnt) {
	assert( m_us );
	m_us->callback(freq, chncount, buffer, smpcnt);
}


TopScope::TopScope() {
	m_mutex = new Thread::Mutex();
	m_buffercnt[0] = m_buffercnt[1] = m_buffercnt[2] = 0;
	m_lasttime = 0;
	m_takeframes = 0;
	m_newbuffer = false;
	m_rot = 0.0f;

	for (int i=0; i<SCOPE_WAVECNT; i++) {
		m_wave[i].pcur = 0.0f;
		m_wave[i].pdelta = 0.0f;
	}

	m_filter = new FilterCB(this);
	OggVorbis::getStream()->filterAdd(m_filter);
}

TopScope::~TopScope() {
	// This dance is to ensure we don't free the mutex while the sound callback
	// is still active, since we delete this object during gameplay sometimes
	m_mutex->lock();
	OggVorbis::getStream()->filterRemove(m_filter);
	m_filter = NULL;
	m_mutex->unlock();
	m_mutex = NULL;
}

void TopScope::callback(int sndhz, int channels, void * buffer, int samplecnt) {
	int actual;
	uint64 t;

	Thread::AutoLock lock(m_mutex);

	// Get the actual data available and cap it
	actual = samplecnt;
	if (actual > 16384*2)
		actual = 16384*2;

	// Shift the buffers
	memcpy(m_buffer+0, m_buffer+1, m_buffercnt[1]*4);
	m_buffercnt[0] = m_buffercnt[1];
	memcpy(m_buffer+1, m_buffer+2, m_buffercnt[1]*4);
	m_buffercnt[1] = m_buffercnt[2];
	memcpy(m_buffer+2, buffer, actual);
	m_buffercnt[2] = actual/4;

	// Update our timing stats
	t = Time::gettime() / 1000;
	if (m_lasttime != 0) {
		// Figure how much time elapsed and estimate the number of frames
		// we ought to take to morph between the buffers
		uint32 elapsed = (uint32)(t - m_lasttime);
		m_takeframes = elapsed * hz / 1000;
	}
	m_lasttime = t;
	m_newbuffer = true;
}

// Could this be done with one triangle strip? Probably. Am I too lazy
// to figure it out? Definitely.
void TopScope::drawCube(float p1[3], float p2[3], uint32 colors[6]) {
	float xsize = (p2[0] - p1[0]) / 2.0f;
	float ysize = (p2[1] - p1[1]) / 2.0f;
	float zsize = (p2[2] - p1[2]) / 2.0f;
	float xcent = p1[0] + xsize;
	float ycent = p1[1] + ysize;
	float zcent = p1[2] + zsize;
	plx_dr_state_t dr; plx_dr_init(&dr);

	// Left end
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent+ysize, zcent+zsize, colors[0]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent-ysize, zcent+zsize, colors[0]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent+ysize, zcent-zsize, colors[0]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent-ysize, zcent-zsize, colors[0]);

	// Front face (first point already submitted)
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent+ysize, zcent-zsize, colors[1]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent-ysize, zcent-zsize, colors[1]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent+ysize, zcent-zsize, colors[1]);

	// Right face (first point already submitted)
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent-ysize, zcent-zsize, colors[2]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent+ysize, zcent+zsize, colors[2]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent-ysize, zcent+zsize, colors[2]);

	// Back face (first point already submitted)
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent+ysize, zcent+zsize, colors[3]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent-ysize, zcent+zsize, colors[3]);
	plx_vert_indm3(&dr, PLX_VERT_EOS, xcent-xsize, ycent+ysize, zcent+zsize, colors[3]);

	// Top face
	// For some weird-ass reason, continuing the triangle strip above causes an
	// artifact on the left face (???)
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent+ysize, zcent+zsize, colors[3]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent+ysize, zcent-zsize, colors[4]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent+ysize, zcent+zsize, colors[4]);
	plx_vert_indm3(&dr, PLX_VERT_EOS, xcent+xsize, ycent+ysize, zcent-zsize, colors[4]);

	// Bottom face
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent-ysize, zcent-zsize, colors[4]);
	plx_vert_indm3(&dr, PLX_VERT, xcent-xsize, ycent-ysize, zcent+zsize, colors[5]);
	plx_vert_indm3(&dr, PLX_VERT, xcent+xsize, ycent-ysize, zcent-zsize, colors[5]);
	plx_vert_indm3(&dr, PLX_VERT_EOS, xcent+xsize, ycent-ysize, zcent+zsize, colors[5]);
}

void TopScope::draw(ObjType list) {
	if (list != Trans)
		return;

	int idx, cnt;
	float x, p, y;
	float xmid = 0.0f, xsize = 9.0f; // 12.0f;
	float ymid = 0.0f, ysize = 1.5f; // 2.0f;
	float zmid = 0;
	float linewid = 1.0f;
	uint32 color;

	const Color & tint = getColor();

	plx_dr_state_t dr; plx_dr_init(&dr);

	plx_mat3d_identity();
	plx_mat3d_translate(0.0f, -0.4f, -12.0f);
	plx_mat3d_rotate(m_rot*2, 0.75f, 1.0f, 0.5f);

	plx_mat_identity();
	plx_mat3d_apply(PLX_MAT_SCREENVIEW);
	plx_mat3d_apply(PLX_MAT_PROJECTION);
	plx_mat3d_apply(PLX_MAT_MODELVIEW);

	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);

	// Bounding box
	{
		float pnts[2][3] = {
			{ xmid - xsize/2.0f, ymid - ysize, zmid-linewid },
			{ xmid + xsize/2.0f, ymid + ysize, zmid+linewid }
		};
		float r = 0.4f, g = 0.4f, b = 0.4f;
		// hsvSpectrum(((int)(rot*8.0f) % 360) / 360.0f, &r, &g, &b);
		uint32 colors[6] = {
			Color(0.2f, r, g, b) * tint,
			Color(0.4f, r, g, b) * tint,
			Color(0.3f, r, g, b) * tint,
			Color(0.25f, r, g, b) * tint,
			Color(0.35f, r, g, b) * tint,
			Color(0.4f, r, g, b) * tint
		};
		drawCube(pnts[0], pnts[1], colors);
	}

	// From here on out we need undisturbed data
	Thread::AutoLock lock(m_mutex);

	if (m_takeframes != 0 && m_buffercnt[0] != 0) {
		float last = 0.0f;
		float delta;
		int i;
		float r, g, b;
		uint32 midclr;
		// cnt = sndbuffer_cnt * takeframes / 60;
		cnt = m_buffercnt[0];

		if (m_newbuffer) {
			for (i=0, p=0.0f; i<SCOPE_WAVECNT; p += 1.0f/SCOPE_WAVECNT, i++) {
				idx = ((int)(p*cnt)) * 2;
				m_wave[i].pdelta = ((ymid + m_buffer[1][idx] * ysize / 32768.0f) - m_wave[i].pcur) / m_takeframes;
			}
			m_newbuffer = false;
		}
		
		hsvSpectrum(((int)(m_rot*4.0f) % 360) / 360.0f, r, g, b);

		midclr = Color(1.0f, r/2.0f, g/2.0f, b/2.0f) * tint;
		plx_vert_indm3(&dr, PLX_VERT, -xsize/2.0f, ymid, zmid+linewid/2.0f, midclr);
		plx_vert_indm3(&dr, PLX_VERT, -xsize/2.0f, ymid, zmid-linewid/2.0f, midclr);
		for (x = xmid-xsize/2.0f, p = 0.0f; x < xmid+xsize/2.0f; x += xsize/SCOPE_WAVECNT, p+=1.0f/SCOPE_WAVECNT) {
			idx = (int)(p * SCOPE_WAVECNT);
			if (idx >= SCOPE_WAVECNT) idx = SCOPE_WAVECNT;
			if (idx < 0) idx = 0;

			delta = m_wave[idx].pcur - last;
			if (delta < -ysize) delta = -ysize;
			if (delta > ysize) delta = ysize;
			
			delta = 0.5f + delta/(2.0f*ysize);
			if (delta > 1.0f) delta = 1.0f;
			if (delta < 0.2f) delta = 0.2f;

			color = Color(1.0f, r*delta, g*delta, b*delta) * tint;	// help ya help ya help ya!
			last = m_wave[idx].pcur;

			y = m_wave[idx].pcur;
			plx_vert_indm3(&dr, PLX_VERT, x, y, zmid+linewid/2.0f, color);
			plx_vert_indm3(&dr, PLX_VERT, x, y, zmid-linewid/2.0f, color);
		}
		plx_vert_indm3(&dr, PLX_VERT, xsize/2.0f, ymid, zmid+linewid/2.0f, midclr);
		plx_vert_indm3(&dr, PLX_VERT_EOS, xsize/2.0f, ymid, zmid-linewid/2.0f, midclr);
	}
}

void TopScope::nextFrame(uint64 tm) {
	if (OggVorbis::isPlaying()) {
		m_rot += 1.0f/4.0f;

		for (int i=0; i<SCOPE_WAVECNT; i++) {
			m_wave[i].pcur += m_wave[i].pdelta;
		}
	}
}

