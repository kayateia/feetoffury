/* 
   Feet of Fury
   
   drawables/saving.cc
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "saving.h"
#include "res.h"

#include "scrollybg.h"
#include "messagebox.h"

#if 0

#include <pthread.h>

static const char * savingText[] = {
	"Saving game data.",
	"Do not remove the VMU",
	"or switch off the console",
	"at this time!"
};
static const int savingCnt = 4;

static const char * failedText[] = {
	"We failed to save a file",
	"to the first VMU. Please",
	"check the VMU and try again",
	"later."
};
static const int failedCnt = 4;

SavingDisplay::SavingDisplay(ScrollyBG * oldbg) {
	if (oldbg) {
		m_sb = oldbg;
	} else {
		m_sb = new ScrollyBG(res.getTexture(Res::Preload, "scrolly_logo2"), false);
	}
	m_sb->setTranslate(Vector(0,0,10));
	subAdd(m_sb);

	m_mb = new MessageBox(24);
	m_mb->setText(savingText, savingCnt);
	m_mb->setTranslate(Vector(320,240,15));
	subAdd(m_mb);

	if (!oldbg) {
		setTint(Color(1,0,0,0));
		animAdd(new TintFader(Color(1,1,1,1), Color(0, 1.0f/(hz/2), 1.0f/(hz/2), 1.0f/(hz/2))));
	}
}

SavingDisplay::~SavingDisplay() {
}

void SavingDisplay::startExit() {
	Color z(1,0,0,0), zd(0, -1.0f/(hz/2), -1.0f/(hz/2), -1.0f/(hz/2));
		
	TintFader * tf = new TintFader(z, zd);
	tf->triggerAdd(new Death());
	animAdd(tf);

	m_mb->startExit();
}

void SavingDisplay::setFail() {
	m_mb->setText(failedText, failedCnt);
}


////////////////////////////////////////////////////////////////////////////

RefPtr<SavingDisplay> Saving::sd;
pthread_t Saving::thd;
static bool nowSaving = false;
volatile bool Saving::thdquit = false;

void * Saving::thdproc(void *p) {
	printf("Saving: thread started\n");
	while (!sd->isFinished()) {
		plx_wait_ready();
		plx_scene_begin();
		plx_list_begin(PLX_LIST_OP_POLY);
		sd->draw(PLX_LIST_OP_POLY);
		plx_list_begin(PLX_LIST_TR_POLY);
		sd->draw(PLX_LIST_TR_POLY);
		plx_scene_finish();

		sd->nextFrame();

		if (thdquit) {
			printf("Saving: starting exit\n");
			sd->startExit();
			thdquit = false;
		}
	}
	printf("Saving: thread finished\n");
	
	return NULL;
}

bool Saving::start(ScrollyBG * oldbg) {
	// Make sure we're not already running
	assert( !nowSaving );
	assert( sd == NULL );

	sd = new SavingDisplay(oldbg);
	thdquit = false;
	nowSaving = true;
	pthread_create(&thd, NULL, thdproc, NULL);
	
	return true;
}

bool Saving::fail() {
	assert( sd != NULL );

	sd->setFail();
	usleep(5*1000*1000);
	return finish();
}

bool Saving::finish() {
	assert( thd != NULL );
	assert( sd != NULL );

	thdquit = true;
	pthread_join(thd, NULL);

	nowSaving = false;
	sd = NULL;

	return true;
}

#endif
