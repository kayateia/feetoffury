/*
   Feet of Fury

   menu_stage/menu_stage.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_stage.h"
#include "plrgenmenu.h"
#include "plrstate.h"
#include "stepdata.h"
#include "ogg.h"
#include "options.h"
#include "res.h"
#include "pak.h"
#include "songs.h"
#include "options.h"
#include "cdtray.h"

#include "gamemode.h"
#include "songinfo.h"
#include "popup.h"
#include "soundfader.h"
#include "screenblinds.h"
#include "finishall.h"

#include "menu_playstyle.h"

// XXX
// #include <sys/types.h>
// #include <dirent.h>


// Constructor / Destructor
MenuStage::MenuStage() {
	// We don't want any bgm
	setBgm("");

	m_subscene = new Scene();
	m_scene->subAdd(m_subscene);

	ScreenBlinds * sb = new ScreenBlinds(false);
	sb->setTint(Color(1,0,0,0));
	m_scene->subAdd(sb);

	// Set some defaults to keep things happy for now
	m_pcnt = 1;
	
	// Load our textures
	m_txr_framework = res.getTexture(Res::MenuStage, "framework");
	m_bkg = new Banner(Drawable::Opaque, m_txr_framework);
	m_bkg->setUV(0.0f, 480.0f/512.0f, 0.0f, 0.0f, 640.0f/1024.0f, 480.0f/512.0f, 640.0f/1024.0f, 0.0f);
	m_bkg->setSize(640, 480);
	m_bkg->setTranslate(Vector(320.0f, 240.0f, 1.0f));
	m_subscene->subAdd(m_bkg);

	// Sound effects
	m_sfx_select = res.getSound(Res::MenuStage, "music_accept");
	m_sfx_unselect = res.getSound(Res::MenuStage, "music_unaccept");
	m_sfx_cursor = res.getSound(Res::MenuStage, "music_cursor");
	m_sfx_announce = res.getSound(Res::MenuStage, "select_song");

	// Load up song info
	getSongList();

	// Load up all the song textures and make a CDTray
	RefPtr<Texture> txrunk;
	m_song_txrs = new RefPtr<Texture>[m_song_cnt];
	for (int i=0; i<m_song_cnt; i++) {
		// XXX Convert to Res
		char tmp[64];
		sprintf(tmp, "song_meta/%s/thumb.png", m_song_names[i]);
		m_song_txrs[i] = new Texture();
		if (!m_song_txrs[i]->loadFromFile(tmp, true)) {
			Debug::printf("warning: couldn't load texture '%s'\n", tmp);
			if (!txrunk)
				txrunk = res.getTexture(Res::MenuStage, "unknown", true);
			m_song_txrs[i] = txrunk;
		}
	}
	txrunk = NULL;
	Texture * shiny[2] = {
		res.getTexture(Res::MenuStage, "shiny1", true),
		res.getTexture(Res::MenuStage, "shiny2", true) };
	m_cdtray = new CDTray(m_song_cnt, m_song_txrs,
		res.getTexture(Res::MenuStage, "cd_shadow", true), shiny);
	m_subscene->subAdd(m_cdtray);

	// Game mode
	m_gamemode = new GameMode();
	m_gamemode->setMode(m_pcnt - 1);
	m_subscene->subAdd(m_gamemode);

	m_smp_playing = false;
	m_selected = 0;

	m_frame = 0;
	m_last_moved = -(hz/2);

	// Our font renderer
	m_fnt = res.getFont("axaxax");

	// Load up all of the step data and make a SongInfo
	m_song_data = new RefPtr<StepData>[m_song_cnt];
	for (int i=0; i<m_song_cnt; i++) {
		// XXX Convert to Res
		char tmp[64];
		Debug::printf("Loading song '%s':\n", m_song_names[i]);
		sprintf(tmp, "song_meta/%s/steps.dwi", m_song_names[i]);
		m_song_data[i] = new StepData(tmp, m_song_names[i]);
	}
	m_songinfo = new SongInfo(m_fnt);
	setSong(0);
	m_subscene->subAdd(m_songinfo);

	m_ogg_thd_running = NULL;

	m_result = Main::Ok;

	setTimeout(60);
}

void MenuStage::setPlayers(PlayerState * p1, PlayerState * p2, bool typing, bool checkPlayerCnt, int initialSong) {
	// Setup the genmenu stuff
// XXX
#if 0
	setDeviceType(typing ? Event::TypeKeyboard : Event::TypeController);
	setPlayerCount(2);
#endif
	m_checkPlayerCnt = checkPlayerCnt;

	m_selected = initialSong;
	if (m_selected == -1)
		m_selected = 0;
	m_cdtray->setSelection(m_selected);
	setSong(m_selected);

// XXX
#if 0
	// Auto-repeat on left, right, pgup and pgdn
	setAutoRepeat(Event::KeyLeft, true);
	setAutoRepeat(Event::KeyRight, true);
	setAutoRepeat(Event::KeyPgup, true);
	setAutoRepeat(Event::KeyPgdn, true);
#endif

	// Infer our player count from the PlayerState objects for now, and
	// ignore how many controllers are plugged in. If we're in practice
	// mode then we'll recalculate it later.
	assert( p1 != NULL );
	assert( p2 != NULL );
	if (p2->ai)
		m_pcnt = 1;
	else
		m_pcnt = 2;
	m_plr[0] = p1;
	m_plr[1] = p2;

}

void MenuStage::finishUp(RefPtr<StepData> & sdout) {
	if (m_ogg_thd) {
		m_ogg_thd->join();
		m_ogg_thd = NULL;
	}

	// Stop the music
	songSampleStop();

	if (m_selected < 0) {
		sdout = NULL;
	} else {
		sdout = m_song_data[m_selected];
		Debug::printf("Selected song at %p, name '%s'\n", (void *)sdout, (char *)(sdout->song_fn));
	}

	// Kill off song info
	delete[] m_song_data;
	delete[] m_song_txrs;

	for (int i=0; i<m_song_cnt; i++) {
		delete m_song_names[i];
	}
	delete[] m_song_names;
	delete[] m_song_idxs;
}

MenuStage::~MenuStage() {
}

void MenuStage::setSong(int idx) {
	if (!swapcd_enabled)
		m_songinfo->newSong(m_song_data[idx], game_options.isSongPlayed(m_song_idxs[idx]));
	else
		m_songinfo->newSong(m_song_data[idx], false);
}

// Called to fill out our song list
bool MenuStage::getSongList() {
	m_song_idxs = NULL;

	// If we're in Swap CD mode, then read the directory.
/*	if (swapcd_enabled) {
		// Open the dir for reading
		DIR * f;
		f = opendir(swapcd_path);
		assert( f );

		// Read through once to figure out the space to allocate
		struct dirent *d;
		int cnt = 0;
		while ( (d = readdir(f)) ) {
			if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
				continue;
			if (d->d_type != DT_DIR)
				continue;
			if (strlen(d->d_name) > 64) continue;
			cnt++;
			if (cnt >= 50) break;
		}

		closedir(f);

		// Now do it again and actually read the names
		m_song_cnt = cnt;
		m_song_names = new char * [m_song_cnt];
		f = opendir(swapcd_path);
		assert( f );

		cnt = 0;
		while ( (d = readdir(f)) ) {
			if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
				continue;
			if (d->d_type != DT_DIR)
				continue;
			if (strlen(d->d_name) > 64) continue;
			m_song_names[cnt] = new char[strlen(d->d_name)+1];
			strcpy(m_song_names[cnt], d->d_name);
			cnt++;
			if (cnt >= 50) break;
		}

		closedir(f);
	} else */{
		// Go through the list of songs and see what's unlocked.
		// We'll build an allocation count from that.
		int cnt = 0;
		for (int i=0; i<Songs::count; i++) {
			if (Songs::unlock[i] < 0 || game_options.isUnlocked(Songs::unlock[i]))
				cnt++;
		}

		// Now do it again and read the names
		m_song_cnt = cnt;
		m_song_idxs = new int[m_song_cnt];
		m_song_names = new char * [m_song_cnt];
		cnt = 0;
		for (int i=0; i<Songs::count; i++) {
			if (Songs::unlock[i] < 0 || game_options.isUnlocked(Songs::unlock[i])) {
				m_song_names[cnt] = new char[strlen(Songs::names[i])+1];
				m_song_idxs[cnt] = i;
				strcpy(m_song_names[cnt], Songs::names[i]);
				cnt++;
			}
		}
	}

	return true;
}
        

void * MenuStage::thdStart(void *ptr) {
	assert( ptr != NULL );
	((MenuStage*)ptr)->thdMethod();
	return NULL;
}
void MenuStage::thdMethod() {
	oggStart(m_ogg_thd_fn, true, false, false);
	m_ogg_thd_running = false;
}

// Start a song sample playing
void MenuStage::songSample() {
	// XXX Convert to Res

	if (m_ogg_thd_running) {
		m_ogg_thd->join();
		m_ogg_thd = NULL;
	}
	songSampleStop();

	// char tmp[64];
	char * tmp = m_ogg_thd_fn;
	//if (swapcd_enabled)
	//	sprintf(tmp, "%s/%s/sample.ogg", swapcd_path, m_song_names[m_selected]);
	//else
		sprintf(tmp, "songs/%s/sample.ogg", m_song_names[m_selected]);
#if TIKI_PLAT == TIKI_WIN32
	// DirectX is thread-stoopid
	oggStart(tmp, true, false, false);
#else
	m_ogg_thd_running = true;
	m_ogg_thd = new Thread::Thread(thdStart, this);
#endif

	m_smp_playing = true;
}

void MenuStage::songSampleStop() {
	// Make sure it actually started before trying to stop
	if (m_smp_playing) {
		if (m_ogg_thd_running) {
			m_ogg_thd->join();
			m_ogg_thd = NULL;
		}
		oggStop(false);
	}
	m_smp_playing = false;
}

void MenuStage::startExit() {
	FinishAll * fa = new FinishAll();
	fa->addTarget(m_cdtray);
	fa->addTarget(m_songinfo);
	fa->addTarget(m_gamemode);
	fa->addTarget(m_bkg);

	ScreenBlinds * sb = new ScreenBlinds(true);
	sb->setTint(Color(1,0,0,0));
	sb->triggerAdd(fa);
	m_scene->subAdd(sb);

	m_scene->animRemoveAll();
	m_scene->animAdd( new SoundFader(0.0f, -1.0f/(hz/2)) );

	PlayerGenericMenu::startExit();
}

void MenuStage::inputEvent(const Event & evt) {
	switch (evt.type) {

	case Event::EvtTimeout:
		if (m_sysmenu) {
			m_sysmenu->exitNow();
			sysMenuDestroy();
		}
		m_sfx_select->play();
		startExit();
		break;

	case Event::EvtKeypress:
		resetTimeout();
// XXX
		if (evt.key == Event::KeyReset) {
			m_result = Main::Title;
			startExit();
		}
		break;
	
	}

	PlayerGenericMenu::inputEvent(evt);

	if (m_checkPlayerCnt) {
		if (m_plrCnt != m_pcnt) {
			m_pcnt = m_plrCnt;

			if (m_pcnt == 1) {
				m_plr[1]->ai = true;
				m_gamemode->setMode(m_pcnt - 1);
			}
		}
	}
}

const char * MenuStage::m_cancelOpt = "Cancel";
const char * MenuStage::m_mainMenuOpt = "Back to Main Menu";
const char * MenuStage::m_charMenuOpt = "Back to Character Select";
const char * MenuStage::m_swapMenuOpt = "Back to Swap CD Menu";


void MenuStage::sysMenuCreate() {
	m_sysmenu = new Popup("System Menu");
	m_sysmenu->setTranslate(Vector(320.0f, 240.0f, 1000.0f));
#ifndef KIOSK_MODE
	m_sysmenu->addOption(m_mainMenuOpt);
#endif
	if (!m_checkPlayerCnt) {
		m_sysmenu->addOption(m_charMenuOpt);
	}
	if (swapcd_enabled) {
		m_sysmenu->addOption(m_swapMenuOpt);
	}
	m_sysmenu->addOption(m_cancelOpt);
	m_scene->subAdd(m_sysmenu);

	m_scene->animAdd( new SoundFader(0.4f, -0.6f/(hz/2)) );
	m_subscene->animAdd(
		new TintFader(Color(0.5f, 0.5f, 0.5f), Color(-0.5f/(hz/2),-0.5f/(hz/2),-0.5f/(hz/2))));
}

void MenuStage::sysMenuDestroy() {
	m_scene->animRemoveAll();
	m_scene->animAdd( new SoundFader(1.0f, 0.6f/(hz/2)) );
	m_subscene->animRemoveAll();
	m_subscene->animAdd(
		new TintFader(Color(1,1,1), Color(0.5f/(hz/2),0.5f/(hz/2),0.5f/(hz/2))) );
	m_sysmenu = NULL;
}

void MenuStage::sysMenuKey(Event::KeyConstant key) {
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
	} else if (!strcmp(optname, m_charMenuOpt)) {
		m_result = Main::MenuChar;
	} else if (!strcmp(optname, m_swapMenuOpt)) {
		m_result = Main::MenuSwapCD;
	} else if (!strcmp(optname, m_mainMenuOpt)) {
		m_result = Main::MenuMain;
	} else {
		Debug::printf("sysmenu returned unknown result %d\n", rv);
		m_result = Main::MenuMain;
	}

	m_sysmenu = NULL;
	startExit();
}

void MenuStage::processKeypress(int player, Event::KeyConstant key) {
	if (m_sysmenu) {
		sysMenuKey(key);
		return;
	}

// XXX
	if (player == 1 && m_plr[1]->ai && key != Event::KeyCancel)
		return;

	switch (key) {

	case Event::KeyLeft:
		if (!m_cdtray->isMoving()) {
			m_sfx_cursor->play();
			m_cdtray->move(-1, (hz/4));
			m_selected--;
			while (m_selected < 0)
				m_selected += m_song_cnt;
			setSong(m_selected);
			m_last_moved = m_frame + (hz/4);
			songSampleStop();
		}
		break;

	case Event::KeyRight:
		if (!m_cdtray->isMoving()) {
			m_sfx_cursor->play();
			m_cdtray->move(1, (hz/4));
			m_selected++;
			while (m_selected >= m_song_cnt)
				m_selected -= m_song_cnt;
			setSong(m_selected);
			m_last_moved = m_frame + (hz/4);
			songSampleStop();
		}
		break;

	case Event::KeyPgup:
		if (!m_cdtray->isMoving()) {
			m_sfx_cursor->play();
			m_cdtray->move(-8, hz/2);
			m_selected -= 8;
			while (m_selected < 0)
				m_selected += m_song_cnt;
			setSong(m_selected);
			m_last_moved = m_frame + hz/2;
			songSampleStop();
		}
		break;

	case Event::KeyPgdn:
		if (!m_cdtray->isMoving()) {
			m_sfx_cursor->play();
			m_cdtray->move(+8, hz/2);
			m_selected += 8;
			while (m_selected >= m_song_cnt)
				m_selected -= m_song_cnt;
			setSong(m_selected);
			m_last_moved = m_frame + hz/2;
			songSampleStop();
		}
		break;

	case Event::KeySelect:
		m_sfx_select->play();
		startExit();
		break;

// XXX
	case Event::KeyCancel:
		if (player == 1 && m_plr[1]->ai) {
			m_plr[1]->ai = false;
			m_gamemode->setMode(m_pcnt - 1);
			m_sfx_select->play();
		} else
			sysMenuCreate();
		break;
	}
}

void MenuStage::visualTransList() {
	if (!m_exiting) {
		// Handle frame triggers
		if (m_frame == 15*hz/60 && game_options.announcer)
			m_sfx_announce->play();
		if (m_frame == (m_last_moved + hz/2))
			songSample();
		m_frame++;
	}

	PlayerGenericMenu::visualTransList();
}

////////////////////////////////////////////////////////////////////////

// Call this from main to do the stage menu
Main::Result doMenuStage(MenuStage * menuPtr, RefPtr<StepData> & sdout, PlayerState * p1, PlayerState * p2, bool typing, bool practice, int & initialSong) {
	// pakMount("menu_stage");

	RefPtr<MenuStage> menu = menuPtr;
	if (!menu) {
		menu = new MenuStage();
	}
	menu->setPlayers(p1, p2, typing, practice, initialSong);

	menu->doMenu();
	menu->finishUp(sdout);
	Main::Result rv = menu->getResult();
	initialSong = menu->getSelectedSong();

	if (rv == Main::Ok) {
		RefPtr<MenuPlayStyle> mps = new MenuPlayStyle(p1, p2, sdout, typing, practice);
		mps->doMenu();
		rv = mps->getResult();
	}

	// pakUnmount("menu_stage");

	return rv;
}
	
