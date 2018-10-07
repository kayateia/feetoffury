/*
   Feet of Fury

   menu_vmu/menu_vmu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_vmu.h"
#include "genmenu.h"
#include "res.h"
#include "options.h"

#include "scrollybg.h"
#include "messagebox.h"

////////////////////////////////////////////////////////////////////////////

class MenuVmu : public GenericMenu {
public:
	MenuVmu();
	virtual ~MenuVmu();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	// Background
	RefPtr<ScrollyBG>	m_sb;

	// Text display
	RefPtr<MessageBox>	m_mb;

	// Do we need a save or not?
	bool			m_canSave, m_doSave;
};

////////////////////////////////////////////////////////////////////////////

// Begin the drawable exodus
void MenuVmu::startExit() {
	// Put a mover on all the objects with a death trigger
	TintFader * fader = new TintFader(Color(1,0,0,0), Color(0,-1.0f/(hz/2),-1.0f/(hz/2),-1.0f/(hz/2)));
	fader->triggerAdd(new Death());
	m_scene->animAdd(fader);

	m_mb->startExit();

	GenericMenu::startExit();
}

void MenuVmu::inputEvent(const Event & evt) {
	if (evt.type != Event::EvtKeypress)
		return;

	switch (evt.key) {
	case Event::KeySelect:
		if (m_canSave) {
			m_scene->subRemove(m_sb);
			m_doSave = true;
			quitNow();
			break;
		}
	case Event::KeyCancel:
		game_options.auto_save = false;
		startExit();
		break;
	}
}

static char txt_buf[80] =
	"VMU XX. If this is OK, then press A or";

static const char * txt_ok[] = {
	"Feet of Fury needs a VMU with 4 blocks",
	"of free space for settings and game",
	"progress storage. After progress is made,",
	"your game will be auto-saved. Currently",
	"we do not detect an existing save on any",
	"attached VMU. We can place a save on",
	txt_buf,
	"Enter. If you do not wish to make a save",
	"file, press B or ESC."
};
static int cnt_ok = 9;

static const char * txt_old[] = {
	"We found a save game for the preview",
	"release of Feet of Fury. The save needs",
	"to be upgraded to work with the new",
	"features of the release version. As a",
	"thanks for being a long-time supporter,",
	"we'll give you 500 free unlock points!",
	"The new file will be saved on",
	txt_buf,
	"Enter. If you do not wish to make a save",
	"file, press B or ESC."
};
static int cnt_old = 10;

static const char * txt_novmu[] = {
	"Feet of Fury needs a VMU with 4 blocks",
	"of free space for settings and game",
	"progress storage. After progress is made,",
	"your game will be auto-saved. Currently",
	"we do not detect any VMUs attached that",
	"have enough free space for a save game.",
	"If you wish to save, please correct",
	"this problem and start the game again."
};
static int cnt_novmu = 8;


// Call this from main to do the main menu
MenuVmu::MenuVmu() {
	// background
	m_sb = new ScrollyBG(res.getTexture(Res::Preload, "scrolly_logo2"), false);
	m_sb->setTranslate(Vector(0.0f, 0.0f, 1.0f));
	m_scene->subAdd(m_sb);

	// Get a font
	RefPtr<Font> font = res.getFont("axaxax");

	// Text display
	m_mb = new MessageBox(18);
	if (game_options.saveableVmuPresent(txt_buf + 4, NULL)) {
		if (game_options.version == 0x01000000) {
			m_mb->setText(txt_old, cnt_old);
		} else {
			m_mb->setText(txt_ok, cnt_ok);
		}
		m_canSave = true;
	} else { 
		m_mb->setText(txt_novmu, cnt_novmu);
		m_canSave = false;
	}
	m_mb->setTranslate(Vector(320.0f, 240.0f, 12.0f));
	m_scene->subAdd(m_mb);
	
	m_scene->animAdd(new TintFader(Color(1,1,1,1), Color(0,1.0f/(hz/2),1.0f/(hz/2),1.0f/(hz/2))));

	m_scene->setTint(Color(1,0,0,0));

	m_doSave = false;
}

MenuVmu::~MenuVmu() {
}


void doMenuVmu() {
	if (game_options.load() == false || game_options.version == 0x01000000) {
		MenuVmu m;
		m.doMenu();

		if (m.m_doSave) {
			game_options.save(m.m_sb);
		}
	}
}
