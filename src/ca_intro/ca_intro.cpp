/*
   Feet of Fury

   ca_intro/new_intro.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "ca_intro.h"
#include "res.h"
#include "particlestorm.h"
#include "starfield.h"
#include "ogg.h"
#include "delay.h"

/////////////////////////////////////////////////////////////////////////////

class CALogo;

class NewCAIntro : public GenericMenu {
public:
	NewCAIntro();
	virtual ~NewCAIntro();

	virtual void doMenu();

	void startFades();

private:
	virtual void visualPerFrame();
	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	RefPtr<Drawable>	m_bgcol;
	RefPtr<CALogo>		m_logo_wf;
	RefPtr<Banner>		m_logo_txr;
	RefPtr<Banner>		m_name;
	RefPtr<ParticleStorm>	m_ps;
	RefPtr<StarField>	m_sf;

	int r;
};


class CALogo : public Drawable {
public:
	CALogo(ParticleStorm * ps, NewCAIntro * menu);
	virtual ~CALogo();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

private:
	float	m_frame;
	bool	m_fading;

	RefPtr<ParticleStorm>	m_ps;
	NewCAIntro		* m_menu;
};

CALogo::CALogo(ParticleStorm * ps, NewCAIntro * menu) {
	m_frame = 0;
	m_ps = ps;
	m_fading = false;
	m_menu = menu;
}

CALogo::~CALogo() {
}

void CALogo::draw(ObjType list) {
	if (list != Trans)
		return;

	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	Vector pos = getPosition();
	Vector scale = getScale();
	Color tint = getColor();

	float vis = 0.2f + 0.8f * (m_frame / 112.0f);
	if (vis > 1.0f)
		vis = 1.0f;
	uint32 color = plx_pack_color(1.0f, tint.a*vis, tint.a*vis, tint.a*vis);

	int rc = 0;

	// Part 1: Half-circle from top to bottom, counter-clockwise
	float start = (270-90/8.0f) * 2*M_PI/360;
	float end = (90-90/10.0f)*2*M_PI/360;
	float r, x1, y1, x2, y2;
	for (int i=0; i<32; i++) {
		r = start + (end - start) * i / 32.0f;
		x1 = fcos(r) * scale.x + pos.x;
		y1 = fsin(r) * scale.y + pos.y;
		if (i == 0) {
			x2 = x1;
			y2 = y1 + scale.y*1/7;
		} else {
			x2 = fcos(r) * scale.x*6/7 + pos.x;
			y2 = fsin(r) * scale.y*6/7 + pos.y;
		}
		
		plx_vert_ind(&dr, PLX_VERT, x1, y1, pos.z, color);
		plx_vert_ind(&dr, i == 31 ? PLX_VERT_EOS : PLX_VERT, x2, y2, pos.z, color);

		rc++;
		if (rc >= m_frame) {
			plx_vert_ind(&dr, PLX_VERT_EOS, x2, y2, pos.z, color);
			goto doPart;
		}
	}

	// Part 2: Column from bottom to center
	for (int i=0; i<16; i++) {
		x1 = scale.x * 0.5f/7.0f + pos.x;
		y1 = scale.y - i*(scale.y+1.0f*scale.x/7.0f)/16.0f + pos.y;
		x2 = -scale.x * 0.5f/7.0f + pos.x;
		y2 = y1;

		plx_vert_ind(&dr, PLX_VERT, x1, y1, pos.z, color);
		plx_vert_ind(&dr, i == 15 ? PLX_VERT_EOS : PLX_VERT, x2, y2, pos.z, color);

		rc++;
		if (rc >= m_frame) {
			plx_vert_ind(&dr, PLX_VERT_EOS, x2, y2, pos.z, color);
			goto doPart;
		}
	}

	// Part 3: Row from center to right
	for (int i=15; i>=0; i--) {
		if (i != 15)
			x1 = (scale.x-2.0f*scale.x/7.0f) - i*(scale.x-1.0f*scale.x/7.0f)/16.0f + pos.x;
		else
			x1 = -scale.x * 0.5f/7.0f + pos.x + 1;
		y1 = -scale.y * 0.5f/7.0f + pos.y;
		x2 = x1;
		y2 = scale.y * 0.5f/7.0f + pos.y;

		plx_vert_ind(&dr, PLX_VERT, x1, y1, pos.z, color);
		plx_vert_ind(&dr, i == 0 ? PLX_VERT_EOS : PLX_VERT, x2, y2, pos.z, color);

		rc++;
		if (rc >= m_frame) {
			plx_vert_ind(&dr, PLX_VERT_EOS, x2, y2, pos.z, color);
			goto doPart;
		}
	}

	// Part 4: Column from center to top
	for (int i=-1; i<16; i++) {
		x1 = scale.x * 0.5f/7.0f + pos.x;
		y1 = -i*(scale.y+0.40f*scale.y/7.0f)/16.0f + pos.y;
		x2 = -scale.x * 0.5f/7.0f + pos.x;
		y2 = y1;

		plx_vert_ind(&dr, PLX_VERT, x1, y1, pos.z, color);
		plx_vert_ind(&dr, i == 15 ? PLX_VERT_EOS : PLX_VERT, x2, y2, pos.z, color);

		rc++;
		if (rc >= m_frame) {
			plx_vert_ind(&dr, PLX_VERT_EOS, x2, y2, pos.z, color);
			goto doPart;
		}
	}

	// Part 5: Half-circle from top to bottom, clockwise
	start = 270 * 2*M_PI/360;
	end = 445*2*M_PI/360;
	for (int i=0; i<32; i++) {
		r = start + (end - start) * i / 32.0f;
		x1 = fcos(r) * scale.x + pos.x;
		y1 = fsin(r) * scale.y + pos.y;
		if (i == 31) {
			x2 = x1;
			y2 = y1 - scale.y*1/7;
		} else {
			x2 = fcos(r) * scale.x*6/7 + pos.x;
			y2 = fsin(r) * scale.y*6/7 + pos.y;
		}
		
		plx_vert_ind(&dr, PLX_VERT, x1, y1, pos.z, color);
		plx_vert_ind(&dr, i == 31 ? PLX_VERT_EOS : PLX_VERT, x2, y2, pos.z, color);

		rc++;
		if (rc >= m_frame) {
			plx_vert_ind(&dr, PLX_VERT_EOS, x2, y2, pos.z, color);
			goto doPart;
		}
	}

	m_ps->setTranslate(Vector(1000,1000,12));
	return;

doPart:
	m_ps->setTranslate(Vector(x1, y1, 12));
}

void CALogo::nextFrame(uint64 t) {
	Drawable::nextFrame(t);

	m_frame += 2.0f * 60.0f/hz;
	if (!m_fading && m_frame >= 120.0f) {
		AlphaFader * af = new AlphaFader(0.0f, -1.0f/(hz/2));
		af->triggerAdd(new Death());
		animAdd(af);
		m_fading = true;

		m_menu->startFades();
	}
}


// This'll be called when the wireframe logo is fading out and the
// texture logo needs to fade in.
void NewCAIntro::startFades() {
	// Start the logo fading in
	AlphaFader * af = new AlphaFader(1.0f, 1.0f/(hz/2));
	m_logo_txr->animAdd(af);

	// Start the background plane fading white
	// TintFader * tf = new TintFader(Color(1.0f,0.6f,0.6f,0.6f), Color(0.0f, 0.6f/(hz/2), 0.6f/(hz/2), 0.6f/(hz/2)));
	// m_bgcol->animAdd(tf);

	// Move both logos up a bit
	LogXYMover *mv = new LogXYMover(320.0f, 200.0f);
	m_logo_txr->animAdd(mv);
	m_logo_wf->animAdd(mv);

	// Start the name banner fading in
	af = new AlphaFader(1.0f, 1.0f/(hz/2));
	m_name->animAdd(af);
	m_scene->subAdd(m_name);
}

void NewCAIntro::visualPerFrame() {
	Color t = m_bgcol->getColor();
	setBg(t.r, t.g, t.b);

	// Cheat a bit :)
	if (t.r >= 0.9f && m_ps) {
		m_scene->subRemove(m_ps);
		m_ps = NULL;
	}

	GenericMenu::visualPerFrame();

	m_bgcol->nextFrame(0);
	
	r++;
	if (r == 5*hz)
		startExit();
}

void NewCAIntro::startExit() {
	Drawable * d[] = {
		m_logo_txr, m_ps, m_name, m_sf
	};
	for (int i=0; i<4; i++) {
		TintFader * af = new TintFader(Color(1,0,0,0), Color(0.0f, -1/(hz/2.0f), -1/(hz/2.0f),-1/(hz/2.0f)));
		af->triggerAdd(new Death());
		d[i]->animAdd(af);
	}
	
	m_usebgm = true;
	GenericMenu::startExit();
}

NewCAIntro::NewCAIntro() {
	r = 0;

	m_ps = new ParticleStorm(res.getTexture(Res::CaIntro, "twinkle", true), 64);

	m_logo_wf = new CALogo(m_ps, this);
	m_logo_wf->setTranslate(Vector(320,240,10));
	m_logo_wf->setScale(Vector(123,123,1));
	m_scene->subAdd(m_logo_wf);

	m_logo_txr = new Banner(Drawable::Trans, res.getTexture(Res::CaIntro, "logo", true));
	m_logo_txr->setTranslate(Vector(320,240,11));
	m_logo_txr->setTint(Color(0.0f, 1.0f, 1.0f, 1.0f));
	m_scene->subAdd(m_logo_txr);

	m_ps->setTranslate(Vector(1000, 1000, 12));
	m_scene->subAdd(m_ps);

	m_name = new Banner(Drawable::Trans, res.getTexture(Res::CaIntro, "name", true));
	m_name->setTranslate(Vector(320,380,20));

	m_sf = new StarField(1024);
	m_sf->setTranslate(Vector(0,0,5));
	m_sf->setTint(Color(1,0,0,0));
	m_sf->animAdd(new TintFader(Color(1,1,1,1), Color(0, 1.0f/(hz/2), 1.0f/(hz/2), 1.0f/(hz/2))));
	m_scene->subAdd(m_sf);

	m_bgcol = new Drawable();
	m_bgcol->setTint(Color(0,0,0));
}

NewCAIntro::~NewCAIntro() { }

void NewCAIntro::doMenu() {
	// We piggyback here to start the music exactly on queue. startExit
	// will set the usebgm flag so we get fade-out.
	oggStart("titles/ca_intro/logo.ogg", false, false, true);

	/* sndoggvorbis_queue_wait();
	sndoggvorbis_queue_go(); */

	GenericMenu::doMenu();
}

void NewCAIntro::inputEvent(const Event & evt) {
	// Any key press quits
	if (evt.type == Event::EvtKeypress)
		startExit();
}

/////////////////////////////////////////////////////////////////////////////

class GSPIntro : public GenericMenu {
public:
	GSPIntro();
	virtual ~GSPIntro();
	virtual void doMenu();

private:
	virtual void visualPerFrame();
	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	RefPtr<Banner>		m_logo_txr;

	int r;
};

GSPIntro::GSPIntro() {
	// Fade in the GSP logo
	m_logo_txr = new Banner(Drawable::Opaque, res.getTexture(Res::CaIntro, "copper", true));
	m_logo_txr->setTranslate(Vector(320,240,10));
	m_logo_txr->setTint(Color(1,0,0,0));
	m_scene->subAdd(m_logo_txr);

	float s = 1.0f/(hz/2);
	TintFader * tf = new TintFader(Color(1,1,1,1), Color(0,s,s,s));
	m_logo_txr->animAdd(tf);

	r = 0;
}

GSPIntro::~GSPIntro() {
}

void GSPIntro::doMenu() {
	// Let's get ol' Copper bleating in the background
	oggStart("titles/ca_intro/copper.ogg", false, false, true);

	/* sndoggvorbis_queue_wait();
	sndoggvorbis_queue_go(); */

	GenericMenu::doMenu();
}

void GSPIntro::visualPerFrame() {
	GenericMenu::visualPerFrame();
	r++;
	if (r > 3*hz) {
		startExit();
		r = -1000;
	}
}

void GSPIntro::startExit() {
	float s = 1.0f/(hz/2);
	TintFader * tf = new TintFader(Color(1,0,0,0), Color(0,-s,-s,-s));
	m_logo_txr->animRemoveAll();
	m_logo_txr->animAdd(tf);

	GenericMenu::startExit();
}

void GSPIntro::inputEvent(const Event & evt) {
	if (evt.type == Event::EvtKeypress)
		startExit();
}


/////////////////////////////////////////////////////////////////////////////


void doCaIntro() {
	{
		NewCAIntro m;
		m.doMenu();
	}
#ifndef _OSX_
	{
		GSPIntro m;
		m.doMenu();
	}
#endif
}
