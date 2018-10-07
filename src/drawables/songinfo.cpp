/* 
   Feet of Fury
   
   drawables/songinfo.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "songinfo.h"
#include "stepdata.h"
#include "hsv.h"
#include "res.h"

// Screen locations: dependent on the bitmap
const float SongInfo::m_y_artist = 231.0f,
	SongInfo::m_y_name = 265.0f,
	SongInfo::m_y_name_2 = 265.0f + 37.0f,
	SongInfo::m_y_bpm = 302.0f + 37.0f,
	SongInfo::m_y_new = 302.0f + 37.0f,
	SongInfo::m_y_diff[3] = { 356.0f, 387.0f, 422.0f };
const float SongInfo::m_x_right = 640.0f - 32.0f,
	SongInfo::m_x_new = 274.0f,
	SongInfo::m_x_difflbl = 210.0f,
	SongInfo::m_y_difflbl = 406.0f;

SongInfo::SongInfo(Font * fnt) {
	assert( fnt != NULL );
	m_fnt = fnt;

	m_frame = 0;
	m_song_info = NULL;

	m_txr_star = res.getTexture(Res::MenuStage, "star", true);
	m_rippleidx = 0;
}

SongInfo::~SongInfo() {
}

void SongInfo::drawOne(float size, float y, float r, float g, float b, const char * text, float x) {
	float height, width;

	m_fnt->setSize(size);
	m_fnt->getTextSize(text, width, height);

	if (x == 0.0f)
		x = m_x_right - width;

	m_fnt->setColor(Color(0.0f, 0.0f, 0.0f));
	m_fnt->draw(Vector(x + 3.0f, y + 3.0f, 127.0f), text);

	m_fnt->setColor(Color(r, g, b));
	m_fnt->draw(Vector(x, y, 128.0f), text);
}

void SongInfo::drawStars(float x, float y, int cnt, float scale) {
	int	i;
	float	ya, size = 32.0f * scale;

	x -= size;

	m_txr_star->select();

	Color col = getColor();
	
	if (cnt == 0) cnt++;
	for (i=0; i<cnt; i++) {
		float cs1, cs2;
		float r1, g1, b1;
		float r2, g2, b2;
		cs1 = (float)i/(cnt+1) + (m_frame % hz) * 1.0f / hz;
		cs2 = (float)(i+1)/(cnt+1) + (m_frame % hz) * 1.0f / hz;
		if (cs1 > 1.0f) cs1 -= 1.0f;
		if (cs2 > 1.0f) cs2 -= 1.0f;
		hsvSpectrum(cs1, r1, g1, b1);
		hsvSpectrum(cs2, r2, g2, b2);

		r1 *= col.r; g1 *= col.g; b1 *= col.b;
		r2 *= col.r; g2 *= col.g; b2 *= col.b;
	
		if (Math::fabs(i - m_rippleidx) <= 2.0f) {
			ya = -8.0f * sqrt(2.0f - Math::fabs(i - m_rippleidx));
		} else {
			ya = 0.0f;
		}

		plx_vert_ffp(PLX_VERT, x, y+ya+size, 128, col.a, r1, g1, b1, 0, 1);
		plx_vert_ffp(PLX_VERT, x, y+ya, 128, col.a, r1, g1, b1, 0, 0);
		plx_vert_ffp(PLX_VERT, x+size, y+ya+size, 128, col.a, r2, g2, b2, 1, 1);
		plx_vert_ffp(PLX_VERT_EOS, x+size, y+ya, 128, col.a, r2, g2, b2, 1, 0);

		x -= size;
	}
}
	
void SongInfo::draw(ObjType list) {
	if (list != Trans)
		return;

	if (m_song_info == NULL)
		return;

	Color col = getColor();
	m_fnt->setAlpha(col.a);

	drawOne(18.0f, m_y_artist, col.r*0.8f, col.g*0.8f, 0.0f, m_song_info->song_artist);
	
	char * sn = strstr(m_song_info->song_name, "\\n");
	if (sn) {
		// XXX
		// char str1[1 + (sn - m_song_info->song_name)];
		char str1[1024];
		strncpy(str1, m_song_info->song_name,
			sn - m_song_info->song_name);
		str1[sn - m_song_info->song_name] = 0;

		drawOne(18.0f, m_y_name, 0.0f, col.g*0.8f, col.b*0.8f, str1);
		drawOne(18.0f, m_y_name_2, 0.0f, col.g*0.8f, col.b*0.8f, sn + 2);
	} else {
		drawOne(18.0f, m_y_name, 0.0f, col.g*0.8f, col.b*0.8f, m_song_info->song_name);
	}

	// KOS dtoa is somewhat broken, it seems...
	float fbpm = m_song_info->bpm - ((int)m_song_info->bpm);
	int ibpm = (int)m_song_info->bpm;
	if (fbpm >= 0.5f)
		ibpm++;
	char tmp[64];
	sprintf(tmp, "%d BPM", ibpm);
	drawOne(18.0f, m_y_bpm, col.r, col.g, col.b, tmp);

	if (m_new) {
		Color c;
		if ((m_frame % (hz/2)) < (hz/4))
			c = Color(1,0.85,0.85,0.85);
		else
			c = Color(1,1,0.3,0.3);
		c *= col;
		
		drawOne(18.0f, m_y_new, c.r, c.g, c.b, "NEW", m_x_new);
	}

	m_fnt->setColor(col);
	m_fnt->setSize(18);
	m_fnt->draw(Vector(m_x_difflbl, m_y_difflbl, 128.0f), "Difficulty");
	
	for (int d=0; d<(int)StepData::Count; d++) {
		drawStars(m_x_right, m_y_diff[d], m_song_info->step_level[d], 1.0f);
	}
}

void SongInfo::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;

	// Set the star ripple, if appropriate
	int n = m_frame % (hz*2);
	if (n < hz) {
		m_rippleidx = n * 12.0f / hz;
	}
}

void SongInfo::newSong(StepData * si, bool newsong) {
	m_song_info = si;
	m_new = newsong;
	Debug::printf("Loaded up song info for '%s'\n", si->song_name);
}
