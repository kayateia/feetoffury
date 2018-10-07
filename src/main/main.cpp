/*
   Feet of Fury

   main/main.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

// Ugh... lots of goto's in this code. Sometimes that's just the
// easiest way to handle it though.

#include "global.h"

#define BUILD_FULL

#include "pak.h"
#include "player.h"
#include "plrstate.h"
#include "gameplay.h"
#include "character.h"
#include "options.h"
#include "res.h"

#include "menu_char.h"
#include "menu_stage.h"
#include "menu_win.h"
#include "menu_swapcd.h"
#include "menu_5060hz.h"
#include "main_cdtray.h"
#include "menu_main.h"
#include "menu_extras.h"
#include "title.h"
#include "about.h"
#include "ca_intro.h"
#include "menu_playstyle.h"
#include "menu_score.h"
#include "menu_vmu.h"
#include "saving.h"

void loadForBattle(bool stopThrobber) {
}

void unloadForBattle() {
}

enum Mode { ModeItemBattle, ModeTyping, ModePractice };

// Returns 0 if the user completed a game normally, -1 if
// the user aborted, or -2 if the user timed out.
Main::Result doBattle(Mode mode) {
	RefPtr<PlayerState> p1, p2;
	Main::Result rv;

	// Do the character selection. Aborting here will send you
	// back to the main menu. If we're in practice mode, we don't
	// care about character selection.
doMenuCharAgain:
	rv = Main::Ok;
//	pakMount("menu_char");
	if (mode != ModePractice) {
		rv = doMenuChar(new MenuChar(mode == ModeTyping), p1, p2);
	} else {
		p1 = new PlayerState();
		p2 = new PlayerState();
	
		p1->charidx = p2->charidx = 0;

		// XXX Shouldn't be necessary
		p1->char_thumb = p2->char_thumb = res.getTexture(Res::MenuChar, "dv_select_thumb_color", true);
		p1->char_full = p2->char_full = res.getTexture(Res::MenuChar, "dv_select_large", true);
	}
//	pakUnmount("menu_char");
	if (rv != Main::Ok)
		return rv;

	// Now do the rounds; again if we're in practice, we just never
	// stop.
	int initialSong = -1;
	for (int i=0; i<3; i++) {
		int winner = -1;
		RefPtr<StepData> song;
		RefPtr<PlayerState> p2pass;

		if (mode == ModePractice)
			i = 0;
		assert( p1->charidx >= 0 && p1->charidx < Characters::count );
		assert( p2->charidx >= 0 && p2->charidx < Characters::count );
		p1->round = p2->round = i;

		// Ok, we now have two ref'd PlayerState objects with the character
		// selections filled in. Do the stage menu
	doMenuStageAgain:
		rv = doMenuStage(NULL, song, p1, p2, mode == ModeTyping, mode == ModePractice, initialSong);
		if (rv == Main::MenuChar)
			goto doMenuCharAgain;
		else if (rv == Main::MenuStage) {
			song = NULL;
			goto doMenuStageAgain;
		} else if (rv != Main::Ok)
			return rv;

		// If we're in practice mode and p2 is invalidated, don't use it.
		if (mode != ModePractice || p2->diff != StepData::Invalid)
			p2pass = p2;

		// Ok, we now have two fully setup players and a song.. do the gameplay!
	doGameplayAgain:
		rv = doGameplay(song, p1, p2pass, false, mode == ModeTyping,
			mode == ModePractice ? GameplayState::ModePractice : GameplayState::ModePlay, winner);
		switch(winner) {
		case -1:
			if (rv != Main::Gameplay)
				song = NULL;
			if (rv == Main::Gameplay)
				goto doGameplayAgain;
			else if (rv == Main::MenuStage)
				goto doMenuStageAgain;
			else if (rv == Main::MenuChar)
				goto doMenuCharAgain;
			else
				return rv;
		case 0:
			i--; break;	// rematch!
		case 1:			// Both of these are handled in gameplay
		case 2:
			break;
		}

		// No winners in practice mode either
		if (mode != ModePractice) {
			// Best 2 out of 3 wins
			if (p1->wins >= 2 || p2->wins >= 2) {
				// Who won, if anyone?
				if (p1->wins > p2->wins) {
					doMenuWin(0, p1);
				} else {
					doMenuWin(1, p2);
				}

				break;
			}
		}
	}

	return Main::Ok;
}

// Returns a return-to code
Main::Result battleLoop(bool swapcd, Mode mode) {
	Main::Result rv = Main::MenuMain;

	if (swapcd)
		swapcd_enabled = true;

	loadForBattle(true);
	pakUnload("titles");

swapCDAgain:
	if (swapcd) {
		// Reclaim a bit of ram
	
		char name[256];
		int modeIdx;
		if (doMenuSwapCD(name, false, modeIdx) >= 0) {
			Debug::printf("USER SELECTED '%s'\n", name);

			// XXX
			bool altpath = false;
			/* file_t f = fs_open("/cd/SwapCD/Version.1e", O_RDONLY);
			bool altpath = false;
			if (f != FILEHND_INVALID) {
				fs_close(f);
				altpath = true;
			} */

			if (altpath)
				sprintf(swapcd_path, "/pd/cd/%s/song_meta.pak", name);
			else
				sprintf(swapcd_path, "/cd/%s/song_meta.pak", name);
			pakLMOldSkool(swapcd_path, "song_meta");

			if (altpath)
				sprintf(swapcd_path, "/pd/cd/%s/songs", name);
			else
				sprintf(swapcd_path, "/cd/%s/songs", name);

			switch (modeIdx) {
			case 0:		// Item battle
				mode = ModeItemBattle;
				break;
			case 1:		// Typing
				mode = ModeTyping;
				break;
			case 2:		// Practice
				mode = ModePractice;
				break;
			}
		} else {
			goto exitBattleLoop;
		}
	}

	for ( ;; ) {
		rv = doBattle(mode);
		if (rv != Main::Ok)
			break;
	}
	if (rv == Main::MenuSwapCD)
		goto swapCDAgain;

exitBattleLoop:
//#ifndef RUN_FROM_PC
	if (swapcd) {
		int mode;
		doMenuSwapCD(NULL, true, mode);
	}
//#endif

	unloadForBattle();
	swapcd_enabled = false;

//	loadingStart();
	pakLoad("titles");
//	loadingFinish();

	return rv;
}

/////////////////////////////////////////////////////////////////////////////////
// "Front end" processing -- title loop and menus

void doDemoPlay(const char * song) {
//	loadForBattle(false);	// Gameplay itself stops this in demo mode

	RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
	RefPtr<StepData> steps = loadSteps(song);
	p1->ai = p2->ai = true;

//	pakMount("menu_char");
	p1->charidx = randnum(CHAR_COUNT-1);
	p2->charidx = randnum(CHAR_COUNT-1);
	p1->char_thumb = res.getTexture(Res::MenuChar, Characters::info[p1->charidx].txr_thumb_color, true);
	p2->char_thumb = res.getTexture(Res::MenuChar, Characters::info[p2->charidx].txr_thumb_color, true);
	p1->char_full = res.getTexture(Res::MenuChar, Characters::info[p1->charidx].txr_large, true);
	p2->char_full = res.getTexture(Res::MenuChar, Characters::info[p2->charidx].txr_large, true);
//	pakUnmount("menu_char");

	int typing = randnum(100);

	int eor_style = game_options.eor_style;
	game_options.eor_style = 1;
	int winner;
// XXX
	doGameplay(steps, p1, p2, true, typing > 50, GameplayState::ModePlay, winner);
	game_options.eor_style = eor_style;

//	unloadForBattle();
}

void loadFrontend() {
//	pakMount("titles");
}

void unloadFrontend() {
//	pakUnmount("titles");
}

// Title loop -- loops until the user does something to indicate that
// they want to play.
void doTitleLoop(bool initial) {
	if (initial)
		doCaIntro();

	// If this is our first time through, make sure our background
	// load completed before going on.
//	if (initial)
//		loadFullComplete();

	int cycle = 0;
	for ( ; ; ) {
		Title::Result tr = Title::doTitle();
		switch (tr) {

		case Title::Start:
			return;

		/* case Title::Reset:
			arch_menu();
			break; */

		case Title::Timeout:
			break;

		}

		// XXX These song names should be pulled from the dirs
		switch (cycle) {
		case 0:
//			pakUnmount("titles");
			doDemoPlay("omm_house");
//			pakMount("titles");
			break;
		case 1:
//			pakUnmount("titles");
			doDemoPlay("djg_tisonits");
//			pakMount("titles");
			break;
		case 2:
			doAbout(true);
			break;
		case 3:
//			pakUnmount("titles");
			doDemoPlay("djg_gotnoclue");
//			pakMount("titles");
			break;
		case 4:
			doCaIntro();
			break;
		}

		cycle = (cycle + 1) % 5;
	}
}

// We will enter this function immediately at game startup, and we won't exit
// it until we're ready to do a non-frontend task (like play the game). -1 is
// used to indicate a program exit in debug mode.
int doFrontend(bool initial, bool doTitles) {
//	loadFrontend();

	int rv = -1;
	for ( ; ; ) {
		if (doTitles) {
			// Wait for user activity
			doTitleLoop(initial);
		}
		initial = false;
		doTitles = true;

	menu_again:
		// Ok, put up the main menu and see what the user wants to do
		rv = doMenuMain(NULL);

		// Non-titles tasks
		if (rv >= 0 && rv <= 3)
			break;

		// Extras menu?
		if (rv == 4) {
			if (doMenuExtras() == Main::Ok)
				goto menu_again;
		}

		// About menu?
		if (rv == 5) {
			doAbout(false);
			goto menu_again;
		}

		// Debug quit?
		if (rv == -1)
			break;
	}

//	unloadFrontend();

	return rv;
}

/////////////////////////////////////////////////////////////////////////////////

#include "manualviewer.h"
#if 0
void test() {
	RefPtr<ManualViewer> mv = new ManualViewer("test.mnl");

	while(true) {
		plx_wait_ready();
		plx_scene_begin();
		plx_list_begin(PLX_LIST_OP_POLY);
		plx_list_begin(PLX_LIST_TR_POLY);

		mv->draw(PLX_LIST_TR_POLY);
		
		plx_scene_finish();

		mv->nextFrame();
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////////

extern "C" int tiki_main() {
	if (globalSetup() < 0)
		return -1;

#ifdef BUILD_FULL
	bool done = false;
	bool initial = true;
	Main::Result blresult = Main::Title;
	while (!done) {
		bool bDoTitle;
		// Are we doing the title screen?
		switch (blresult) {
		case Main::MenuMain:
			bDoTitle = false;
			break;

		default:
			bDoTitle = true;
			break;

		}

		// Do titles, main menus, etc
		int result = doFrontend(initial, bDoTitle);

		initial = false;

		switch (result) {
		case 0: // Item battle
			blresult = battleLoop(false, ModeItemBattle);
			break;

		case 1: // Typing of Fury
			blresult = battleLoop(false, ModeTyping);
			break;

		case 2: // Practice mode
			blresult = battleLoop(false, ModePractice);
			break;

		case 3: // Swap CD
			blresult = battleLoop(true, ModeItemBattle);
			break;

		case -1: // Debug quit
			done = true;
			break;

		default: // ???
			assert( false );
		}
	}
#endif

#if defined(BUILD_VMU)
	Saving::start(NULL);
	Time::usleep(5*1000*1000);
	Saving::finish();
#endif

#if defined(BUILD_BATTLE) || defined(BUILD_STAGE) \
	|| defined(BUILD_CHAR) || defined(BUILD_GAMEPLAY) || defined(BUILD_PRACTICE) \
	|| defined(BUILD_GAMEPLAY_TEST) || defined(BUILD_WIN) || defined(BUILD_PLAYSTYLE) \
	|| defined(BUILD_SCORE)
//	loadFullComplete();
	loadForBattle(true);
#endif

#ifdef BUILD_INTRO
	loadFrontend();
	doCaIntro();
	unloadFrontend();
#endif

#ifdef BUILD_LOADING
	loadingStart();
	thd_sleep(5*1000);
	loadingFinish();
#endif

#ifdef BUILD_TITLE
	pakMount("titles");
	Title::doTitle();
	pakUnmount("titles");
#endif

#ifdef BUILD_MAIN
	doMenuMain(NULL);
#endif

#ifdef BUILD_EXTRAS
	pakMount("titles");
	doMenuExtras();
	pakUnmount("titles");
#endif

#ifdef BUILD_BATTLE
	doBattle();
#endif

#ifdef BUILD_EDITOR
	doEditor();
#endif

#ifdef BUILD_STAGE
	RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
	RefPtr<StepData> steps;

	p1->ai = false;
	p2->ai = false;
	p1->charidx = 0; p2->charidx = 1;
	pakMount("menu_char");
	p1->char_thumb = res.getTexture(Res::MenuChar, "dv_select_thumb_color", true);
	p1->char_full = res.getTexture(Res::MenuChar, "dv_select_large", true);
	p2->char_thumb = res.getTexture(Res::MenuChar, "sv_select_thumb_color", true);
	p2->char_full = res.getTexture(Res::MenuChar, "sv_select_large", true);
	pakUnmount("menu_char");

	int foo = -1;
	doMenuStage(NULL, steps, p1, p2, /* typing */true, /* practice */true, foo);
#endif

#ifdef BUILD_DEMO
	doDemoPlay("omm_house");
#endif

#ifdef BUILD_CHAR
	RefPtr<PlayerState> p1, p2;
	int rv;

	// Do the character selection. Aborting here will send you
	// back to the main menu.
	pakMount("menu_char");
	rv = doMenuChar(new MenuChar(true), p1, p2);
	pakUnmount("menu_char");
#endif

#ifdef BUILD_GAMEPLAY
	RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
	p1->diff = StepData::Medium;
	p2->diff = StepData::Medium;
	p1->charidx = 0; p2->charidx = 1;
	p1->ai = false; p2->ai = true;
	pakMount("menu_char");
	p1->char_thumb = res.getTexture(Res::MenuChar, "dv_select_thumb_color", true);
	p1->char_full = res.getTexture(Res::MenuChar, "dv_select_large", true);
	p2->char_thumb = res.getTexture(Res::MenuChar, "sv_select_thumb_color", true);
	p2->char_full = res.getTexture(Res::MenuChar, "sv_select_large", true);
	pakUnmount("menu_char");
	RefPtr<StepData> steps = loadSteps("djg_gotnoclue");

	int winner;
	doGameplay(steps, p1, p2, /* demo */false, /* typing */false, GameplayState::ModePlay, winner);
#endif

#ifdef BUILD_PRACTICE
	/* swapcd_enabled = true;
	pakUnmount("song_meta");
	pakMountUnencr("/cd/Rage/song_meta.pak", "song_meta");
	strcpy(swapcd_path, "/cd/Rage/songs"); */

	RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
	p1->diff = StepData::Hard;
	p2->diff = StepData::Medium;
	p1->charidx = 0; p2->charidx = 1;
	p1->ai = false; p2->ai = true;
	p1->char_thumb = res.getTexture(Res::MenuChar, "dv_select_thumb_color", true);
	p2->char_thumb = res.getTexture(Res::MenuChar, "sv_select_thumb_color", true);
	RefPtr<StepData> steps = loadSteps("kud_nrg");

	int winner;
	doGameplay(steps, p1, p2, false, false, GameplayState::ModePractice, winner);
#endif

#ifdef BUILD_GAMEPLAY_TEST
	for (int i=0; i<20; i++) {
		doGameplay("ddr_lupin", StepData::Invalid, StepData::Invalid, false);
	}
#endif

#ifdef BUILD_WIN
	RefPtr<PlayerState> p1 = new PlayerState();

	for (int i=0; i<Characters::count; i++) {
		// int i = 5;
		pakMount("menu_char");
		p1->charidx = i;
		p1->char_full = res.getTexture(Res::MenuChar, Characters::info[i].txr_large, true);
		pakUnmount("menu_char");
		doMenuWin(0, p1);
	}
#endif

#ifdef BUILD_PLAYSTYLE
	testMenuPlayStyle();
#endif

#ifdef BUILD_SCORE
	testMenuScore();
#endif

#ifdef BUILD_ABOUTLOOP
	for (int i=0; i<30; i++)
		doAbout(true);
#endif

#ifdef BUILD_ABOUT
	doAbout(true);
#endif

#ifdef BUILD_SWAPCD
	char name[256];
	
	swapcd_enabled = true;
	loadForBattle();

	int mode;
	if (doMenuSwapCD(name, false, mode) >= 0) {
		Debug::printf("USER SELECTED '%s'\n", name);

		sprintf(swapcd_path, "/cd/%s/song_meta.pak", name);
		pakLMOldSkool(swapcd_path, "song_meta");
		
		sprintf(swapcd_path, "/cd/%s/songs", name);
		
		RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
		RefPtr<StepData> steps;

		p1->ai = false;
		p2->ai = true;

		int initialSong = -1;
		doMenuStage(NULL, steps, p1, p2, false, true, initialSong);

		pakUnmount("song_meta");
		swapcd_enabled = false;

		doMenuSwapCD(name, true, mode);
	}
#endif

#ifdef BUILD_TEST
	test();
#endif

	globalShutdown();

	return 0;
}

