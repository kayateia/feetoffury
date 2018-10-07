/*
   Feet of Fury

   menu_char/menu_char.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_char.h"
#include "plrgenmenu.h"
#include "plrstate.h"
#include "character.h"
#include "options.h"
#include "res.h"
#include "pak.h"

#include "gridselector.h"
#include "topbar.h"
#include "scroller.h"
#include "slideshow.h"
#include "bubblefield.h"
#include "popup.h"
#include "soundfader.h"

MenuChar::MenuChar(bool t)
	: PlayerGenericMenu()
// XXX
//	: PlayerGenericMenu(t ? Event::TypeKeyboard : Event::TypeController, 2)
{
	m_typing = t;

	m_pstate[0] = new PlayerState();
	m_pstate[1] = new PlayerState();

	m_pstate[0]->charidx = 0;
	m_pstate[1]->charidx = 0;
	m_pstate[0]->ai = false;
	m_pstate[1]->ai = true;

	m_chscene = new Scene();

	// Bubble background
	m_bf = new BubbleField(res.getTexture(Res::Preload, "bubble", true), 32);
	m_bf->setTranslate(Vector(0.0f, 0.0f, 0.0002f));
	m_chscene->subAdd(m_bf);

	for (int i=0; i<4; i++) {
		m_confirmed[i] = false;
	}

	// Topbars to go behind player names
	m_namebars[0] = new Topbar(15.0f, -1.0f, Topbar::Vertical, Topbar::Barcode);
	m_namebars[0]->setTranslate(Vector(680.0f, 0.0f, 2.0f));
	m_namebars[0]->setTint(Color(0.6f,1,1,1));
	m_namebars[1] = new Topbar(15.0f, 1.0f, Topbar::Vertical, Topbar::Barcode);
	m_namebars[1]->setTranslate(Vector(-40.0f, 0.0f, 2.0f));
	m_namebars[1]->setTint(Color(0.6f,1,1,1));
	m_chscene->subAdd(m_namebars[0]);
	m_chscene->subAdd(m_namebars[1]);

	LogXYMover * namebar_mover = new LogXYMover(48.0f, 0.0f);
	m_namebars[0]->animAdd(namebar_mover);
	namebar_mover = new LogXYMover(640.0f - 48.0f, 0.0f);
	m_namebars[1]->animAdd(namebar_mover);

	// Character shots
	for (int i=0; i<Characters::count; i++) {
		m_char_large_txrs[i] = res.getTexture(Res::MenuChar, Characters::info[i].txr_large, true);
	}

	// Character Slideshows
	m_chars[0] = new SlideShow(Characters::count, -1, 2.0f);
	for (int i=0; i<Characters::count; i++) {
		m_chars[0]->setPic(i, m_char_large_txrs[i], i < (Characters::count-1));
	}
	m_chars[0]->setTranslate(Vector(160.0f, 240.0f, 10.0f));
	m_chars[0]->setSlide(0);
	m_chscene->subAdd(m_chars[0]);

	m_chars[1] = new SlideShow(Characters::count, -1, 2.0f);
	for (int i=0; i<Characters::count; i++)
		m_chars[1]->setPic(i, m_char_large_txrs[i]);
	m_chars[1]->setTranslate(Vector(640.0f - 160.0f, 240.0f, 10.0f));
	m_chars[1]->setTint(Color(0.3f,1,1,1));
	m_chars[1]->setSlide(0);
	m_chscene->subAdd(m_chars[1]);

	// Character Names
	for (int i=0; i<Characters::count; i++) {
		m_char_name_txrs[i] = res.getTexture(Res::MenuChar, Characters::info[i].txr_banner, true);
	}

	m_char_names[0] = new Banner(Drawable::Trans, m_char_name_txrs[0]);
	m_char_names[0]->setTranslate(Vector(680.0f, 240.0f, 13.0f));
	m_char_names[0]->setUV(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	m_char_names[0]->setSize(32.0f, 512.0f);
	m_char_names[0]->animAdd(new LogXYMover(48.0f, 240.0f));
	m_char_names[0]->setTint(Color(0.8f,1,1,1));
	m_chscene->subAdd(m_char_names[0]);
	
	m_char_names[1] = new Banner(Drawable::Trans, m_char_name_txrs[0]);
	m_char_names[1]->setTranslate(Vector(-40.0f, 240.0f, 13.0f));
	m_char_names[1]->setUV(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	m_char_names[1]->setSize(32.0f, 512.0f);
	m_char_names[1]->animAdd(new LogXYMover(640.0f - 48.0f, 240.0f));
	m_char_names[1]->setTint(Color(0.3f,1,1,1));
	m_chscene->subAdd(m_char_names[1]);

	// Grid selector
	m_gs = new GridSelector(Characters::grid_w, Characters::grid_h, 64, 64, 2);
	for (int i=0; i<Characters::count; i++) {
		m_char_thumb_txrs[i] = res.getTexture(Res::MenuChar, Characters::info[i].txr_thumb_color, true);
		m_gs->setTextureAt(
			Characters::info[i].grid_x,
			Characters::info[i].grid_y,
			m_char_thumb_txrs[i],
			res.getTexture(Res::MenuChar, Characters::info[i].txr_thumb_gray, true),
			game_options.char_unlocked[i] ? Color(1,1,1,1) : Color(1, 0.5f, 0.5f, 0.5f));
	}
	m_gs_txrs[0] = res.getTexture(Res::MenuChar, "1p", true);
	m_gs_txrs[1] = res.getTexture(Res::MenuChar, "2p", true);
	m_gs->setSelAttrs(0, 1.0f, 0.3f, 0.3f, m_gs_txrs[0]);
	m_gs->setSelAttrs(1, 0.3f, 1.0f, 0.3f, m_gs_txrs[1]);
	m_gs->setSelection(0, 0,0);
	m_gs->setSelection(1, 0,0);
	m_gs->setNumSels(1);
	m_gs->setTranslate(Vector(320.0f, 240.0f, 15.0f));
	m_chscene->subAdd(m_gs);

	// Font
	m_font = res.getFont("axaxax");

	m_char_bg[0] = new Scroller(m_font, 16.0f, Characters::info[Characters::charAt(0,0)].scroller);
	m_char_bg[0]->setTint(Color(0.7f,1,1,1));
	m_char_bg[0]->setTranslate(Vector(0.0f, 480 + 48.0f, 1.0f));
	m_char_bg[0]->animAdd(new LogXYMover(0.0f, 48.0f));
	m_chscene->subAdd(m_char_bg[0]);

	m_char_bg[1] = new Scroller(m_font, 16.0f, "       ");
	m_char_bg[1]->setTint(Color(0.7f,1,1,1));
	m_char_bg[1]->setTranslate(Vector(0.0f, - 64.0f, 1.0f));
	m_chscene->subAdd(m_char_bg[1]);

	// Sound effects
	m_sfx_announce = res.getSound(Res::Preload, "select_char");
	m_sfx_select = res.getSound(Res::Preload, "menu_accept");
	m_sfx_cursor = res.getSound(Res::Preload, "menu_cursor");
	m_sfx_p2in = res.getSound(Res::Preload, "title_accept");
	m_sfx_invalid = res.getSound(Res::Preload, "invalid");

	// Add our sub-scene to the main one
	m_scene->subAdd(m_chscene);

	// XXX Convert to Res
	setBg(0.05f, 0.0f, 0.20f);
	setBgm("bgm/menu_char.ogg");

	m_frame = 0; m_p2start = false;

	setTimeout(60);
	resetTimeout();

	m_retval = Main::Ok;
}

MenuChar::~MenuChar() {
}

Main::Result MenuChar::getResults(RefPtr<PlayerState> & p1out, RefPtr<PlayerState> & p2out) {
	// Shutdown
	if (m_pstate[1]->ai) {
		m_pstate[1]->charidx = randnum(Characters::count-1);
		assert( m_pstate[1]->charidx < Characters::count-1 );
	}
	Debug::printf("PLAYER 1 CHOSE CHAR %d\n", m_pstate[0]->charidx);
	Debug::printf("PLAYER 2 CHOSE CHAR %d\n", m_pstate[1]->charidx);

	for (int i=0; i<2; i++) {
		m_pstate[i]->char_thumb = m_char_thumb_txrs[m_pstate[i]->charidx];
		m_pstate[i]->char_full = m_char_large_txrs[m_pstate[i]->charidx];
	}

	if (m_retval == Main::Ok) {
		p1out = m_pstate[0];
		p2out = m_pstate[1];
		return Main::Ok;
	} else
		return m_retval;
}

// Push out the P2 images and such
void MenuChar::p2Out() {
	AlphaFader * fader = new AlphaFader(0.3f, -1.0f/30.0f);
	m_chars[1]->animRemoveAll();
	m_chars[1]->animAdd(fader);
	m_char_names[1]->animRemoveAll();
	m_char_names[1]->animAdd(fader);
	m_char_bg[1]->animRemoveAll();
	m_char_bg[1]->animAdd(new ExpXYMover(0.0f, -1.0f, 0.0f, -48.0f));

	m_gs->setNumSels(1);

	m_pstate[1]->ai = true;
	m_p2start = false;

	m_confirmed[1] = false;
	m_gs->setSelAttrs(1, 0.3f, 1.0f, 0.3f, m_gs_txrs[1]);
}

// Pull in the P2 images and such
void MenuChar::p2In() {
	int x, y, ci;
	m_gs->getSelection(1, &x, &y);
	ci = Characters::charAt(x, y);

	AlphaFader * fader;
	if (game_options.char_unlocked[ci])
		fader = new AlphaFader(0.8f, 1.0f/30.0f);
	else
		fader = new AlphaFader(0.5f, 1.0f/30.0f);
	m_char_names[1]->animRemoveAll();
	m_char_names[1]->animAdd(fader);

	if (game_options.char_unlocked[ci])
		fader = new AlphaFader(1.0f, 1.0f/30.0f);
	else
		fader = new AlphaFader(0.5f, 1.0f/30.0f);
	m_chars[1]->animRemoveAll();
	m_chars[1]->animAdd(fader);

	m_char_bg[1]->animRemoveAll();
	m_char_bg[1]->animAdd(new LogXYMover(0.0f, 480.0f - 48.0f));

	m_gs->setNumSels(2);

	m_sfx_p2in->play();

	if (game_options.char_unlocked[ci])
		m_char_bg[1]->setText(Characters::info[ci].scroller);
	else
		m_char_bg[1]->setText(Characters::locked);

	m_pstate[1]->ai = false;
}

// Begin the drawable exodus
void MenuChar::startExit() {
	// Put a mover on all the objects with a death trigger
	AlphaFader * fader = new AlphaFader(0.0f, -1.0f/(hz/2.0f));
	fader->triggerAdd(new Death());
	m_bf->animAdd(fader);

	ExpXYMover * mover = new ExpXYMover(1.0f, 0.0f, 640.0f + 128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_namebars[0]->animAdd(mover);

	mover = new ExpXYMover(-1.0f, 0.0f, -128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_namebars[1]->animAdd(mover);

	mover = new ExpXYMover(1.0f, 0.0f, 640.0f + 128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_chars[0]->animAdd(mover);

	mover = new ExpXYMover(-1.0f, 0.0f, - 128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_chars[1]->animAdd(mover);

	mover = new ExpXYMover(1.0f, 0.0f, 640.0f + 128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_char_names[0]->animAdd(mover);

	mover = new ExpXYMover(-1.0f, 0.0f, - 128.0f, 0.0f);
	mover->triggerAdd(new Death());
	m_char_names[1]->animAdd(mover);

	fader = new AlphaFader(0.0f, -1.0f/(hz/2.0f));
	fader->triggerAdd(new Death());
	m_gs->animAdd(fader);

	mover = new ExpXYMover(0.0f, 1.0f, 0.0f, 480.0f+64.0f);
	mover->triggerAdd(new Death());
	m_char_bg[0]->animAdd(mover);

	mover = new ExpXYMover(0.0f, -1.0f, 0.0f, -64.0f);
	mover->triggerAdd(new Death());
	m_char_bg[1]->animAdd(mover);

	PlayerGenericMenu::startExit();
}

// Do one frame of video goodness
void MenuChar::visualTransList() {
	PlayerGenericMenu::visualTransList();

	if (m_plrCnt >= 2 && !m_p2start && !m_exiting) {
		m_font->setSize(30.0f);
		m_font->setAlpha(0.5f + fsin(m_frame * 2*M_PI/hz) * 0.5f);
		if (m_typing)
			m_font->drawCentered(Vector(640.0f - 128.0f, 100.0f, 128.0f), "P2 Hit ENTER!");
		else
			m_font->drawCentered(Vector(640.0f - 128.0f, 100.0f, 128.0f), "P2 Hit START!");
	}

	m_frame++;
	if (m_frame == (15*hz/60) && game_options.announcer)
		m_sfx_announce->play();
}

void MenuChar::inputEvent(const Event & evt) {
	// PlayerGenericMenu::inputEvent(evt);

	if (evt.type == Event::EvtTimeout) {
		// Timeout.. select for them :)
		if (m_sysmenu) {
			m_sysmenu->exitNow();
			sysMenuDestroy();
		}
		m_sfx_select->play();
		startExit();
	}

	// If we reset, then abort
// XXX
	if (evt.type == Event::EvtKeypress && evt.key == Event::KeyReset) {
		m_retval = Main::Title;
		startExit();
		return;
	}

	PlayerGenericMenu::inputEvent(evt);
}


const char * MenuChar::m_cancelOpt = "Cancel";
const char * MenuChar::m_mainMenuOpt = "Back to Main Menu";
const char * MenuChar::m_swapMenuOpt = "Back to Swap CD Menu";


void MenuChar::sysMenuCreate() {
	m_sysmenu = new Popup("System Menu");
	m_sysmenu->setTranslate(Vector(320.0f, 240.0f, 200.0f));
#ifndef KIOSK_MODE
	m_sysmenu->addOption(m_mainMenuOpt);
#endif
	//if (swapcd_enabled) {
	//	m_sysmenu->addOption(m_swapMenuOpt);
	//}
	m_sysmenu->addOption(m_cancelOpt);
	m_scene->subAdd(m_sysmenu);

	m_scene->animAdd( new SoundFader(0.4f, -0.6f/(hz/2)) );
	m_chscene->animAdd(
		new TintFader(Color(0.5f, 0.5f, 0.5f), Color(-0.5f/(hz/2),-0.5f/(hz/2),-0.5f/(hz/2))));
}

void MenuChar::sysMenuDestroy() {
	m_scene->animRemoveAll();
	m_scene->animAdd( new SoundFader(1.0f, 0.6f/(hz/2)) );
	m_chscene->animRemoveAll();
	m_chscene->animAdd(
		new TintFader(Color(1,1,1), Color(0.5f/(hz/2),0.5f/(hz/2),0.5f/(hz/2))) );
	m_sysmenu = NULL;
}

void MenuChar::sysMenuKey(Event::KeyConstant key) {
	Event evt;
	evt.type = Event::EvtKeypress;
	evt.key = key;
	m_sysmenu->inputEvent(evt);

	int rv = m_sysmenu->getResult();
	if (rv == -1)
		return;
	if (rv == -2) {
		sysMenuDestroy();
		return;
	}

	const char * optname = m_sysmenu->getOptionString(rv);
	if (!strcmp(optname, m_cancelOpt)) {
		sysMenuDestroy();
		return;
	} else if (!strcmp(optname, m_swapMenuOpt)) {
		m_retval = Main::MenuSwapCD;
	} else if (!strcmp(optname, m_mainMenuOpt)) {
		m_retval = Main::MenuMain;
	} else {
		Debug::printf("sysmenu returned unknown result %d\n", rv);
		m_retval = Main::MenuMain;
	}

	m_sysmenu = NULL;
	startExit();
}

// Process keypresses from attached peripherals
void MenuChar::processKeypress(int pidx, Event::KeyConstant key) {
	resetTimeout();

	// Handle the system menu if it's up
	if (m_sysmenu) {
		sysMenuKey(key);
		return;
	}

	// Nope ... if this is p2, check for start.
	if (pidx == 1) {
		if ( (/*key == Event::KeyStart
				|| */ (m_typing && key == Event::KeySelect))
			&& !m_p2start)
		{
			p2In();
			m_p2start = true;
			return;
		}

		if (!m_p2start)
			return;
	}

	// Has the player confirmed their character yet? If so, pass this event on to
	// the attack style popup.
	if (m_confirmed[pidx]) {
		if (key == Event::KeyCancel) {
			m_confirmed[pidx] = false;
			if (pidx == 0)
				m_gs->setSelAttrs(0, 1.0f, 0.3f, 0.3f, m_gs_txrs[pidx]);
			else
				m_gs->setSelAttrs(1, 0.3f, 1.0f, 0.3f, m_gs_txrs[pidx]);
			m_sfx_cursor->play();
		}

		return;
	}

	// Ok, so they're still sitting at the character selection. Process
	// the key as normal...
	int moveDir = -1;
	switch (key) {
	case Event::KeySelect: {
		int x, y;

		m_gs->getSelection(pidx, &x, &y);
		if (!Characters::info[Characters::charAt(x,y)].enabled ||
			!game_options.char_unlocked[Characters::charAt(x,y)])
		{
			m_sfx_invalid->play();
		} else {
			if (!m_confirmed[pidx]) {
				if (y == Characters::grid_h-1 && x == Characters::grid_w-1) {
					int j = Characters::pickRandom();
					x = j % Characters::grid_w;
					y = j / Characters::grid_w;
					m_gs->setSelection(pidx, x, y);
					moveDir = 0;
				}
				m_gs->setSelAttrs(pidx, 0.5f, 0.5f, 0.5f, m_gs_txrs[pidx]);
					
				m_sfx_select->play();
				// m_pstate[pidx]->char_sound = res.getSound(Res::MenuChar, Characters::info[Characters::charAt(x,y)].sound_fn);
				// m_pstate[pidx]->char_sound->play();
				m_confirmed[pidx] = true;

				if ((m_confirmed[0] && m_confirmed[1]) || (m_confirmed[0] && (m_plrCnt == 1 || !m_p2start))) {
					startExit();
				}
			}
		}
		break;
	}

	case Event::KeyLeft:
		moveDir = 4; break;
	case Event::KeyUp:
		moveDir = 8; break;
	case Event::KeyRight:
		moveDir = 6; break;
	case Event::KeyDown:
		moveDir = 2; break;

#ifndef KIOSK_MODE
	case Event::KeyCancel:
		sysMenuCreate();
		return;
#endif

	}

	// Process any move requests
	if (moveDir != -1) {
		int x, y;

		if (moveDir != 0)
			m_sfx_cursor->play();

		m_gs->getSelection(pidx, &x, &y);
		switch (moveDir) {
		case 4: x--; break;
		case 8: y--; break;
		case 6: x++; break;
		case 2: y++; break;
		// case 0: user selected a character
		}

		if (x < 0) x += Characters::grid_w;
		if (x >= Characters::grid_w) x -= Characters::grid_w;
		if (y < 0) y += Characters::grid_h;
		if (y >= Characters::grid_h) y -= Characters::grid_h;

		m_gs->setSelection(pidx, x, y);

		m_pstate[pidx]->charidx = y*Characters::grid_w+x;

		m_chars[pidx]->setSlide(y*Characters::grid_w+x);
		m_char_names[pidx]->setTexture(m_char_name_txrs[y*Characters::grid_w+x]);
		if (game_options.char_unlocked[y*Characters::grid_w+x]) {
			m_chars[pidx]->setTint(Color(1,1,1,1));
			m_char_names[pidx]->setTint(Color(0.8f,1,1,1));
			if (moveDir != 0)
				m_char_bg[pidx]->setText(Characters::info[Characters::charAt(x,y)].scroller);
		} else {
			m_chars[pidx]->setTint(Color(0.5f,1,1,1));
			m_char_names[pidx]->setTint(Color(0.5f,1,1,1));
			if (moveDir != 0)
				m_char_bg[pidx]->setText(Characters::locked);
		}
	}
}

// Process peripheral attach/detach messages
void MenuChar::processPeriph() {
	int oldpcnt = m_plrCnt;
	PlayerGenericMenu::processPeriph();

	if (oldpcnt != m_plrCnt)
		Debug::printf("Player count changed from %d to %d\n", oldpcnt, m_plrCnt);

	// If we lost P2, then p2out
	if (oldpcnt == 2 && m_plrCnt == 1)
		p2Out();

	// If we gained P2, then allow p2in
	if (oldpcnt == 1 && m_plrCnt == 2)
		m_p2start = false;
}

////////////////////////////////////////////////////////////////////////////

// Call this from main to do the main menu
Main::Result doMenuChar(MenuChar * mc, RefPtr<PlayerState> & p1out, RefPtr<PlayerState> & p2out) {
	RefPtr<MenuChar> m = mc;
	m->doMenu();
	Main::Result rv = m->getResults(p1out, p2out);
	return rv;
}

