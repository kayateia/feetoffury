/* 
   Feet of Fury
   
   drawables/vs.cc
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "vs.h"
#include "res.h"

#include "plrstate.h"
#include "gpskin.h"
#include "starfield.h"
#include "topbar.h"

VSDisplay::VSDisplay(PlayerState * p1, PlayerState * p2) {
	m_plr[0] = p1;
	m_plr[1] = p2;

	m_sf = new StarField(512);
	m_sf->setTranslate(Vector(0,0,10));
	m_sf->setTint(Color(0,0,0));
	m_sf->animAdd(new TintFader(Color(1,1,1,1), Color(0, 1.0f/(hz/2), 1.0f/(hz/2), 1.0f/(hz/2))));
	subAdd(m_sf);

	m_banner[0] = new Banner(Trans, m_plr[0]->char_full);
	m_banner[0]->setTranslate(Vector(160, -240, 15));
	m_banner[0]->animAdd(new LogXYMover(160, 240));
	subAdd(m_banner[0]);

	m_banner[1] = new Banner(Trans, m_plr[1]->char_full);
	m_banner[1]->setTranslate(Vector(320+160, 480+240, 15));
	m_banner[1]->animAdd(new LogXYMover(320+160, 240));
	subAdd(m_banner[1]);

	RefPtr<Font> f = res.getFont("axaxax");
	m_v = new Label(f, "V", 36, true, true);
	m_v->setTranslate(Vector(640+320,240,20));
	m_v->animAdd(new LogXYMover(320-15, 240));
	subAdd(m_v);

	m_s = new Label(f, "S", 36, true, true);
	m_s->setTranslate(Vector(-320,240,20));
	m_s->animAdd(new LogXYMover(320+15, 240));
	subAdd(m_s);

	m_bar = new Topbar(32.0f, 2.0f, Topbar::Horizontal, Topbar::Barcode);
	m_bar->setTranslate(Vector(0, 240, 17));
	m_bar->setAlpha(0);
	m_bar->animAdd(new AlphaFader(1.0f, 1.0f/(hz/2)));
	subAdd(m_bar);

	m_frame = 0;
	m_needExit = m_exiting = false;
}

VSDisplay::~VSDisplay() {
}

void VSDisplay::draw(ObjType list) {
	Drawable::draw(list);
	if (list != Trans)
		return;
}

void VSDisplay::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;
	if (m_needExit && m_frame >= (hz*4)) {
		Color z(1,0,0,0), zd(0, -1.0f/(hz/2), -1.0f/(hz/2), -1.0f/(hz/2));
		
		TintFader * tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_sf->animAdd(tf);

		tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_banner[0]->animAdd(tf);

		tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_banner[1]->animAdd(tf);

		tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_v->animAdd(tf);

		tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_s->animAdd(tf);

		tf = new TintFader(z, zd);
		tf->triggerAdd(new Death());
		m_bar->animAdd(tf);

		m_needExit = false;
		m_exiting = true;
		m_frame = hz*4;
	}
	if (m_exiting && m_frame >= (hz*4 + hz/2 + 3))
		setFinished();
}

void VSDisplay::startExit() {
	m_needExit = true;
}


////////////////////////////////////////////////////////////////////////////

RefPtr<VSDisplay> VS::vsd;
RefPtr<Thread::Thread> VS::thd;
static bool nowVsing = false;
volatile bool VS::thdquit = false;

void * VS::thdproc(void *p) {
	Debug::printf("VS: thread started\n");
	while (!vsd->isFinished()) {
		vsd->createSceneList();
		Frame::begin();
		vsd->drawScene(Drawable::Opaque);
		Frame::transEnable();
		vsd->drawScene(Drawable::Trans);
		Frame::finish();
		vsd->destroySceneList();

		vsd->nextFrame(0);

		if (thdquit) {
			Debug::printf("VS: starting exit\n");
			vsd->startExit();
			thdquit = false;
		}
	}
	Debug::printf("VS: thread finished\n");
	
	return NULL;
}

bool VS::start(PlayerState * p1, PlayerState * p2) {
	// Make sure we're not already running
	assert( !nowVsing );
	assert( vsd == NULL );

	vsd = new VSDisplay(p1, p2);
	thdquit = false;
	nowVsing = true;
	thd = new Thread::Thread(thdproc, NULL);
	
	return true;
}

bool VS::finish() {
	assert( nowVsing );
	assert( vsd != NULL );

	thdquit = true;
	thd->join(NULL); thd = NULL;

	nowVsing = false;
	vsd = NULL;

	return true;
}
