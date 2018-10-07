/*
   Feet of Fury

   menu_extras/menu_extras.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"
#include "menu_extras.h"
#include "unlock.h"
#include "options.h"
#include "pak.h"

#include "scrollybg.h"

#include "extrasMenu.h"
#include "multiOptionMenu.h"
#include "subMenuOption.h"
#include "disabledOption.h"
#include "placeholderMenu.h"
#include "unlockMenu.h"
#include "jukeboxMenu.h"
#include "slotsMenu.h"
#include "manualviewMenu.h"

// #include "optionController.h"
#include "optionVolume.h"
#include "optionAnnouncer.h"
#include "optionMono.h"
#include "optionAiSkill.h"
#include "optionEor.h"
#include "optionDuration.h"
#include "optionMorePoints.h"
#include "optionGenericBool.h"
#include "optionBtnMap.h"

MenuExtras::MenuExtras() {
	/* pakLoad("manual");
	pakMount("manual");
	pakUnload("manual"); */

	m_font = res.getFont("axaxax");

	m_sfx_select = res.getSound(Res::MenuOptions, "options_select");
	m_sfx_unselect = res.getSound(Res::MenuOptions, "options_back");
	m_sfx_cursor = res.getSound(Res::MenuOptions, "menu_cursor");
	m_sfx_invalid = res.getSound(Res::MenuOptions, "invalid");

	m_sb = new ScrollyBG(res.getTexture(Res::MenuOptions, "scrolly_logo2"), true, 0.75f);
	m_sb->setTranslate(Vector(0,0,5.0f));
	m_scene->subAdd(m_sb);

	// Make all of our menus now ////////////////////////////////////////

	// Create all the menus up front so we can cross-link them
	m_mom = new MultiOptionMenu(this);
		m_settings = new MultiOptionMenu(this);
			m_audioSet = new MultiOptionMenu(this);
			m_gameSet = new MultiOptionMenu(this);
			m_practiceSet = new MultiOptionMenu(this);
			m_keySet = new MultiOptionMenu(this);
		m_jukebox = new JukeboxMenu(this, m_mom);
		m_unlock = new UnlockMenu(this, m_mom);
		m_manual = new MultiOptionMenu(this);
			m_manLegal = new ManualviewMenu(this, m_manual, "legal");
			m_manIntro = new ManualviewMenu(this, m_manual, "genintro");
			m_manGameplay = new MultiOptionMenu(this);
				m_manGameplayIntro = new ManualviewMenu(this, m_manGameplay, "gpintro");
				m_manGameplayItemBattles = new ManualviewMenu(this, m_manGameplay, "gpitembattle");
				m_manGameplayTyping = new ManualviewMenu(this, m_manGameplay, "gptyping");
				m_manGameplayPractice = new ManualviewMenu(this, m_manGameplay, "gppractice");
			m_manSwapCD = new ManualviewMenu(this, m_manual, "swapcds");
			m_manExtras = new MultiOptionMenu(this);
				m_manExtrasOptions = new ManualviewMenu(this, m_manExtras, "options");
				m_manExtrasUnlock = new ManualviewMenu(this, m_manExtras, "unlock");
				m_manExtrasJukebox = new ManualviewMenu(this, m_manExtras, "jukebox");
			m_manAboutMusic = new ManualviewMenu(this, m_manual, "aboutmusic");
			m_manAboutCA = new ManualviewMenu(this, m_manual, "aboutca");
			m_manAboutGSP = new ManualviewMenu(this, m_manual, "aboutgsp");
		m_slots = new SlotsMenu(this, m_mom);

	// The main menu
	m_mom->setTitle("Extras/Options");
	m_mom->setOptionCount(6);
	m_mom->setOption(0, new SubMenuOption(this, "Options >>", m_settings, true));
	m_mom->setOption(1, new SubMenuOption(this, "Sound Check >>", m_jukebox, true));
	m_mom->setOption(2, new SubMenuOption(this, "Unlock Store >>", m_unlock, true));
	m_mom->setOption(3, new SubMenuOption(this, "Online Manual >>", m_manual, true));
	if (game_options.isUnlocked(Unlock::Casino)) {
		m_mom->setOption(4, new SubMenuOption(this, "Casino >>", m_slots, true));
		m_casinoOpen = true;
	} else {
		m_mom->setOption(4, new DisabledOption(this, "? <Locked> ?"));
		m_casinoOpen = false;
	}
	// m_mom->setOption(5, new OptMorePoints(this));
	m_mom->setOption(5, new SubMenuOption(this, "<< Exit", NULL, false), true);

		// Settings menu
		m_settings->setTitle("Options");
		m_settings->setOptionCount(5);
		m_settings->setOption(0, new SubMenuOption(this, "Button Config >>", m_keySet, true));
		m_settings->setOption(1, new SubMenuOption(this, "Audio Settings >>", m_audioSet, true));
		m_settings->setOption(2, new SubMenuOption(this, "Game Settings >>", m_gameSet, true));
		m_settings->setOption(3, new SubMenuOption(this, "Practice Settings >>", m_practiceSet, true));
		m_settings->setOption(4, new SubMenuOption(this, "<< Back", m_mom, false), true);
			// Key config menu
			m_keySet->setTitle("Button Config");
			m_keySet->setOptionCount(11);
			m_keySet->setOption(0, new OptBtnMap(this, m_keySet, "Up #1", &game_options.buttonSettings[Options::Up1]));
			m_keySet->setOption(1, new OptBtnMap(this, m_keySet, "Up #2", &game_options.buttonSettings[Options::Up2]));
			m_keySet->setOption(2, new OptBtnMap(this, m_keySet, "Left #1", &game_options.buttonSettings[Options::Left1]));
			m_keySet->setOption(3, new OptBtnMap(this, m_keySet, "Left #2", &game_options.buttonSettings[Options::Left2]));
			m_keySet->setOption(4, new OptBtnMap(this, m_keySet, "Right #1", &game_options.buttonSettings[Options::Right1]));
			m_keySet->setOption(5, new OptBtnMap(this, m_keySet, "Right #2", &game_options.buttonSettings[Options::Right2]));
			m_keySet->setOption(6, new OptBtnMap(this, m_keySet, "Down #1", &game_options.buttonSettings[Options::Down1]));
			m_keySet->setOption(7, new OptBtnMap(this, m_keySet, "Down #2", &game_options.buttonSettings[Options::Down2]));
			m_keySet->setOption(8, new OptBtnMap(this, m_keySet, "Use Item", &game_options.buttonSettings[Options::UseItem]));
			m_keySet->setOption(9, new OptGenericBool(this, "System Menu", &game_options.allowSystemMenu));
			m_keySet->setOption(10, new SubMenuOption(this, "<< Back", m_settings, false), true);

			// Audio settings menu
			m_audioSet->setTitle("Audio Settings");
			m_audioSet->setOptionCount(5);
			m_audioSet->setOption(0, new OptVolume(this, 0));
			m_audioSet->setOption(1, new OptVolume(this, 1));
			m_audioSet->setOption(2, new OptAnnouncer(this));
			m_audioSet->setOption(3, new OptMono(this));
			m_audioSet->setOption(4, new SubMenuOption(this, "<< Back", m_settings, false), true);

			// Game settings menu
			m_gameSet->setTitle("Game Settings");
			m_gameSet->setOptionCount(4);
			m_gameSet->setOption(0, new OptAISkill(this));
			m_gameSet->setOption(1, new OptEOR(this));
			m_gameSet->setOption(2, new OptDuration(this));
			m_gameSet->setOption(3, new SubMenuOption(this, "<< Back", m_settings, false), true);

			// Practice settings menu
			m_practiceSet->setTitle("Practice Mode Settings");
			m_practiceSet->setOptionCount(4);
			m_practiceSet->setOption(0, new OptGenericBool(this, "Background", &game_options.practiceBG));
			m_practiceSet->setOption(1, new OptGenericBool(this, "Beat Bars", &game_options.practiceBeatBars));
			m_practiceSet->setOption(2, new OptGenericBool(this, "Clap Track", &game_options.practiceClapTrack));
			m_practiceSet->setOption(3, new SubMenuOption(this, "<< Back", m_settings, false), true);

		// Online manual menu
		m_manual->setTitle("Online Manual");
		m_manual->setOptionCount(9);
		m_manual->setOption(0, new SubMenuOption(this, "Disclaimer / Legal >>", m_manLegal, true));
		m_manual->setOption(1, new SubMenuOption(this, "General Intro >>", m_manIntro, true));
		m_manual->setOption(2, new SubMenuOption(this, "Gameplay >>", m_manGameplay, true));
		m_manual->setOption(3, new SubMenuOption(this, "Swap CDs >>", m_manSwapCD, true));
		m_manual->setOption(4, new SubMenuOption(this, "Options / Extras >>", m_manExtras, true));
		m_manual->setOption(5, new SubMenuOption(this, "Musician Bios >>", m_manAboutMusic, true));
		m_manual->setOption(6, new SubMenuOption(this, "Cryptic Allusion >>", m_manAboutCA, true));
		m_manual->setOption(7, new SubMenuOption(this, "GOAT Store Publishing >>", m_manAboutGSP, true));
		m_manual->setOption(8, new SubMenuOption(this, "<< Back", m_mom, false), true);

			// Gameplay
			m_manGameplay->setTitle("Gameplay");
			m_manGameplay->setOptionCount(5);
			m_manGameplay->setOption(0, new SubMenuOption(this, "Introduction >>", m_manGameplayIntro, true));
			m_manGameplay->setOption(1, new SubMenuOption(this, "Item Battle >>", m_manGameplayItemBattles, true));
			m_manGameplay->setOption(2, new SubMenuOption(this, "Typing of Fury >>", m_manGameplayTyping, true));
			m_manGameplay->setOption(3, new SubMenuOption(this, "Practice Mode >>", m_manGameplayPractice, true));
			m_manGameplay->setOption(4, new SubMenuOption(this, "<< Back", m_manual, false), true);

			// Options/Extras
			m_manExtras->setTitle("Extras and Options");
			m_manExtras->setOptionCount(4);
			m_manExtras->setOption(0, new SubMenuOption(this, "Gameplay Options >>", m_manExtrasOptions, true));
			m_manExtras->setOption(1, new SubMenuOption(this, "Unlock System >>", m_manExtrasUnlock, true));
			m_manExtras->setOption(2, new SubMenuOption(this, "Sound Check >>", m_manExtrasJukebox, true));
			m_manExtras->setOption(3, new SubMenuOption(this, "<< Back", m_manual, false), true);


	m_root = m_mom;
	m_curMenu = m_root;

	m_curMenu->setTranslate(Vector(0,0,10));
	m_scene->subAdd(m_curMenu);

	// cacheLoad("bgm/options.ogg", "options.ogg");
	// setBgm(CACHE_PREFIX "options.ogg");
	setBgm("bgm/options.ogg");

	m_result = Main::Ok;

	m_picPopupDirs = false;
}

MenuExtras::~MenuExtras() {
	/* pakUnmount("manual");
	cacheUnload("options.ogg"); */
}

void MenuExtras::startExit() {
	TintFader * fader = new TintFader(Color(1,0,0,0), Color(0,-1.0f/(hz/2),-1.0f/(hz/2),-1.0f/(hz/2)));
	fader->triggerAdd(new Death());
	m_scene->animAdd(fader);

	GenericMenu::startExit();
}

void MenuExtras::inputEvent(const Event & evt) {
	m_curMenu->inputEvent(evt);
}

void MenuExtras::setSubMenu(ExtrasMenu * menu, bool moveRight, bool playSound) {
	// If we were already in a transition, then kill it off immediately
	if (m_lastMenu) {
		m_curMenu->setTranslate(Vector(0,0,10));
		m_curMenu->animRemoveAll();
		m_lastMenu->animRemoveAll();
		finishedMoving();
	}

	if (menu == NULL) {
		if (playSound)
			m_sfx_unselect->play();
		startExit();
		return;
	}

	m_lastMenu = m_curMenu;
	m_curMenu = menu;
	m_scene->subAdd(m_curMenu);

	if (moveRight) {	// For sub-menus
		m_curMenu->setTranslate(Vector(640,0,10));
		m_curMenu->animAdd(new LogXYMover(0,0));

		LogXYMover * lm = new LogXYMover(-640, 0);
		lm->triggerAdd(new FinishTrigger(this));
		m_lastMenu->animAdd(lm);

		if (playSound)
			m_sfx_select->play();
	} else {		// For parent-menus
		m_curMenu->setTranslate(Vector(-640,0,10));
		m_curMenu->animAdd(new LogXYMover(0,0));

		LogXYMover * lm = new LogXYMover(640, 0);
		lm->triggerAdd(new FinishTrigger(this));
		m_lastMenu->animAdd(lm);

		if (playSound)
			m_sfx_unselect->play();
	}

	m_curMenu->gainFocus();
}

void MenuExtras::finishedMoving() {
	// Get rid of our "lastMenu"
	m_scene->subRemove(m_lastMenu);
	m_lastMenu = NULL;
}

void MenuExtras::rescanUnlock() {
	m_root->rescanUnlock();

	if (!m_casinoOpen && game_options.isUnlocked(Unlock::Casino)) {
		MultiOptionMenu * mom = (MultiOptionMenu *)(void *)m_root;
		mom->setOption(4, new SubMenuOption(this, "Casino >>", m_slots, true));
		m_casinoOpen = true;
	}
}

Main::Result doMenuExtras() {
	Main::Result opt;

	// loadingStart();
	MenuExtras m;
	// loadingFinish();
	m.doMenu();
	opt = m.getResult();

	game_options.save(NULL);

	return opt;
}
