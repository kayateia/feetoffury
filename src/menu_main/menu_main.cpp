/*
   Feet of Fury

   menu_main/menu_main.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_main.h"
#include "res.h"

#include "topbar.h"
#include "slideshow.h"
#include "bubble_selector.h"
#include "textdisplay.h"
#include "messagebox.h"
#include "menubox.h"

////////////////////////////////////////////////////////////////////////////

// Menu option infos
struct MenuOption {
	const char	* icon;
	const char	* sshot;
	const char	* label;
	const char 	* text[20];
	int		textlen;
	bool		enabled;
};

static MenuOption options[] = {
	// 0 -- Item Battle
	{ "ico_play",
	  "opt_gameplay",
	  "Item Battle",
	  {
	    "Play against another",
	    "player or the computer",
	    "in the Feet of Fury",
	    "Item Battle mode."
	  }, 4, true
	},

	// 1 -- Typing of Fury
	{ "ico_typing",
	  "opt_typing",
	  "Typing of Fury",
	  {
	    "Plug in your keyboard",
	    "and let your fingers",
	    "do the dancing! Type",
	    "words in time to the",
	    "music as they scroll",
	    "upwards."
	  }, 6, true
	},

	// 2 -- Practice
	{ "ico_practice",
	  "opt_practice",
	  "Practice Mode",
	  {
	    "If you're having some",
	    "trouble getting into",
	    "the groove, use this",
	    "mode to practice songs",
	    "without any player vs",
	    "player worries."
	  }, 6, true
	},

	// 3 -- Play game with Swap CD
	{ "ico_jukebox",
	  "opt_jukebox",
	  "Swap CD",
	  {
	    "Load up a Swap CD with",
	    "your own songs and step",
	    "data, and use it to",
	    "play any of the normal",
	    "modes. Please do not",
	    "swap out CDs until the",
	    "game instructs you to",
	    "do so."
	  }, 8, true
	},

	// 4 -- Options/Extras
	{ "ico_options",
	  "opt_options",
	  "Options/Extras",
	  {
	    "Set various options",
	    "that affect game play,",
	    "unlock new areas of the",
	    "game, view an online",
	    "manual, and more."
	  }, 5, true
	},

	// 5 -- Credits
	{ "ico_about",
	  "opt_about",
	  "Credits",
	  {
	    "Game credits"
	  }, 1, true
	}

	/* // 5 -- Extras
	{ "ico_edit",
	  "opt_editor",
	  "Extras",
	  {
	    "Extra features, such as",
	    "the art gallery, edit",
	    "data creator, etc. Not",
	    "completed yet!"
	  }, 4, false
	},

	// 5 -- Art Gallery
	{ "ico_art_gallery",
	  "opt_nopic",
	  "Art Gallery",
	  {
	    "View the artwork from",
	    "and inspired by Feet",
	    "of Fury.",
	    "(not completed)",
	  }, 4, false
	} */
};

static int options_cnt = 6;

// Selected menu option
static int cur_option = -1;

////////////////////////////////////////////////////////////////////////////

// Start the exodus for when the user has selected something
void MenuMain::startExit() {
	// Put a mover on all the objects with a death trigger
	ExpXYMover * mover = new ExpXYMover(-1.0f, 0.0f, -128.0f, 80.0f);
	mover->triggerAdd(new Death());
	fof_banner->animAdd(mover);

	for (int i=0; i<3; i++) {
		mover = new ExpXYMover(0.0f, 1.0f, 0.0f, 480.0f + 80.0f);
		mover->triggerAdd(new Death());
		topbar[i]->animAdd(mover);
	}

	for (int i=0; i<2; i++) {
		mover = new ExpXYMover(1.0f, 0.0f, 640.0f+128.0f, 0.0f);
		mover->triggerAdd(new Death());
		sidebar[i]->animAdd(mover);
	}

	AlphaFader * fader = new AlphaFader(0.0f, -1.0f/30.0f);
	fader->triggerAdd(new Death());
	slides->animAdd(fader);

	fader = new AlphaFader(0.0f, -1.0f/30.0f);
	fader->triggerAdd(new Death());
	sshots->animAdd(fader);

	mover = new ExpXYMover(-1.0f, 0.0f, -256.0f, 0.0f);
	mover->triggerAdd(new Death());
	label->animAdd(mover);

	fader = new AlphaFader(0.0f, -1.0f/15.0f);
	fader->triggerAdd(new Death());
	sel->animAdd(fader);
	
	fader = new AlphaFader(0.0f, -1.0f/30.0f);
	fader->triggerAdd(new Death());
	td->animAdd(fader);

	GenericMenu::startExit();
}

void MenuMain::processTach() {
	// Scan to see if we have any keyboards attached. If so,
	// enable the Typing of Fury option. Otherwise, disable it
	// (entering that menu without a keyboard won't do too well).
	bool havekb = false;
	bool havecont = false;
	// XXX
	/* for (int i=0; i<4; i++) {
		if (m_contTypes[i] & MAPLE_FUNC_KEYBOARD)
			havekb = true;
		if (m_contTypes[i] & MAPLE_FUNC_CONTROLLER)
			havecont = true;
	} */

	options[0].enabled = havecont;
	sel->setEnabled(0, havecont);
	options[1].enabled = havekb;
	sel->setEnabled(1, havekb);
	options[2].enabled = havecont;
	sel->setEnabled(2, havecont);
	sel->update();
}

void MenuMain::inputEvent(const Event & evt) {
	switch (evt.type) {

	case Event::EvtTimeout:
		cur_option = -2;
		startExit();
		break;

	case Event::EvtKeypress:
		processKey((Event::KeyConstant)evt.key);
		break;

	case Event::EvtAttach:
	case Event::EvtDetach:
		processTach();
		break;
	}
}

void MenuMain::processKey(Event::KeyConstant key) {
	if (kbmb && key != Event::KeyReset) {
		kbmb->startExit();
		kbmb = NULL;
		return;
	}

	bool updateStuff = false;

	switch (key) {

	case Event::KeyLeft:
		sfx_cursor->play();
		sel->choiceSwitch(-1);
		cur_option = sel->getChoice();
		updateStuff = true;
		break;

	case Event::KeyRight:
		sfx_cursor->play();
		sel->choiceSwitch(+1);
		cur_option = sel->getChoice();
		updateStuff = true;
		break;


	case Event::KeySelect:
		if (options[cur_option].enabled) {
			sfx_select->play();
			startExit();
		} else {
			// XXX
			sfx_invalid->play();
			if (cur_option == 1) {
				static const char *kbmsg[] = {
					"Typing of Fury requires at least one",
					"attached keyboard. We currently do",
					"not detect any keyboards. If you",
					"wish to play this mode, attach a",
					"keyboard and try again."
				};
				kbmb = new MessageBox(18.0f);
				kbmb->setTranslate(Vector(320,240,1000));
				kbmb->setText(kbmsg, 5);
				m_scene->subAdd(kbmb);
			} else if (cur_option == 0 || cur_option == 2) {
				static const char *kbmsg[] = {
					"Item Battle and Practice Mode",
					"require at least one attached",
					"control pad or dance mat. We",
					"currently do not detect any",
					"such devices. If you wish to",
					"play this mode, attach one of",
					"them and try again."
				};
				kbmb = new MessageBox(18.0f);
				kbmb->setTranslate(Vector(320,240,1000));
				kbmb->setText(kbmsg, 7);
				m_scene->subAdd(kbmb);
			}
		}
		break;

	case Event::KeyReset:
		cur_option = -2;
		startExit();
		break;

	case Event::KeyCancel:
#if defined(RUN_FROM_PC) || defined(TEST_FROM_PC)
		cur_option = -1;
		startExit();
#endif
		break;
	}

	if (updateStuff) {
		sshots->setSlide(cur_option);
		label->setText(options[cur_option].label);
		td->setText(options[cur_option].text, options[cur_option].textlen);
	}
}


////////////////////////////////////////////////////////////////////////////

MenuMain::MenuMain() {
	// pvr_mem_stats();

	// Setup
	if (cur_option < 0 || cur_option >= options_cnt)
		cur_option = 0;

	// Add the top parallax bars	
	topbar[0] = new Topbar(28.0f, -4.0f, Topbar::Horizontal, Topbar::Barcode);
	topbar[0]->setTranslate(Vector(0.0f, 480.0f + 80.0f, 2.0f));
	topbar[1] = new Topbar(10.0f, -3.0f, Topbar::Horizontal, Topbar::Barcode);
	topbar[1]->setTranslate(Vector(0.0f, 480.0f + 80.0f - (10.0f + 5.0f), 1.0f));
	topbar[2] = new Topbar(10.0f, -3.0f, Topbar::Horizontal, Topbar::Barcode);
	topbar[2]->setTranslate(Vector(0.0f, 480.0f + 80.0f + (10.0f + 5.0f), 1.0f));
	m_scene->subAdd(topbar[0]);
	m_scene->subAdd(topbar[1]);
	m_scene->subAdd(topbar[2]);

	LogXYMover * topbar_mover = new LogXYMover(0.0f, 80.0f);
	topbar[0]->animAdd(topbar_mover);
	topbar_mover = new LogXYMover(0.0f, 80.0f - (10.0f + 5.0f));
	topbar[1]->animAdd(topbar_mover);
	topbar_mover = new LogXYMover(0.0f, 80.0f + (10.0f + 5.0f));
	topbar[2]->animAdd(topbar_mover);

	// Add the side parallax bars
	sidebar[0] = new Topbar(10.0f, -1.0f, Topbar::Vertical, Topbar::Checker);
	sidebar[0]->setTranslate(Vector(640.0f + 64.0f, 0.0f, 3.0f));
	sidebar[1] = new Topbar(10.0f, -1.0f, Topbar::Vertical, Topbar::Checker, 10.0f);
	sidebar[1]->setTranslate(Vector(640.0f + 64.0f+10.0f + 256.0f, 0.0f, 3.0f));
	m_scene->subAdd(sidebar[0]);
	m_scene->subAdd(sidebar[1]);

	topbar_mover = new LogXYMover(64.0f, 0.0f);
	sidebar[0]->animAdd(topbar_mover);
	topbar_mover = new LogXYMover(64.0f+10.0f, 0.0f);
	sidebar[1]->animAdd(topbar_mover);

	// Add the FoF banner
	fof_banner = new Banner(Drawable::Trans, res.getTexture(Res::MenuMain, "fof_logo", true));
	fof_banner->setTranslate(Vector(-64.0f, 80.0f, 5.0f));
	fof_banner->setSize(128.0f, 128.0f);
	LogXYMover * fof_banner_mover = new LogXYMover(640.0f - 128.0f, 80.0f);
	fof_banner->animAdd(fof_banner_mover);
	m_scene->subAdd(fof_banner);

	// Character Slideshow
	slides = new SlideShow(6, hz*6, 0.5f);
	slides->setPic(0, res.getTexture(Res::MenuMain, "char_slide_1", true));
	slides->setPic(1, res.getTexture(Res::MenuMain, "char_slide_2", true));
	slides->setPic(2, res.getTexture(Res::MenuMain, "char_slide_3", true));
	slides->setPic(3, res.getTexture(Res::MenuMain, "char_slide_4", true));
	slides->setPic(4, res.getTexture(Res::MenuMain, "char_slide_5", true));
	slides->setPic(5, res.getTexture(Res::MenuMain, "char_slide_6", true));
	slides->setTranslate(Vector(640.0f - (128.0f + 64.0f), 270.0f, 0.001f));
	slides->setTint(Color(0.7f, 0.5f, 0.6f, 0.8f));
	m_scene->subAdd(slides);

	// Screen shots
	sshots = new SlideShow(options_cnt, -1, 2.0f);
	for (int i=0; i<options_cnt; i++)
		sshots->setPic(i, res.getTexture(Res::MenuMain, options[i].sshot, true));
	sshots->setTranslate(Vector(128.0f + 32.0f, 200.0f, 10.0f));
	sshots->setSlide(cur_option);
	m_scene->subAdd(sshots);

	// Font
	font = res.getFont("axaxax");

	// Menu option label
	label = new Label(font, options[cur_option].label, 36, false, true);
	label->setTranslate(Vector(-256.0f, 80.0f + 10.0f, 15.0f));
	label->animAdd(new LogXYMover(40.0f, 80.0f + 10.0f));
	m_scene->subAdd(label);

	// Bubble selector
	sel = new BubbleSelector(options_cnt, cur_option);
	for (int i=0; i<options_cnt; i++) {
		sel->setTexture(i, res.getTexture(Res::MenuMain, options[i].icon, false));
		sel->setEnabled(i, options[i].enabled);
	}
	sel->setTranslate(Vector(-2.8f, -2.5f, -10.0f));	// 3D coords!!
	sel->setDist(1.1f);
	sel->finalize();
	// sel->setChoice(cur_option);
	sel->setTint(Color(0,1,1,1));
	sel->animAdd(new AlphaFader(1.0f, 1.0f/30.0f));
	//m_scene->subAdd(sel);

	// Text display
	td = new TextDisplay(font, 20);
	td->setTranslate(Vector(640.0f - (128.0f + 64.0f), 270.0f, 20.0f));
	td->setText(options[cur_option].text, options[cur_option].textlen);
	m_scene->subAdd(td);

	// Sound effects
	sfx_select = res.getSound(Res::MenuMain, "menu_accept");
	sfx_cursor = res.getSound(Res::MenuMain, "menu_cursor");
	sfx_invalid = res.getSound(Res::MenuMain, "invalid");

	setBg(0.08f, 0.0f, 0.35f);
	setBgm("bgm/menu_main.ogg");
	resetTimeout();

	// pvr_mem_stats();

}

MenuMain::~MenuMain() {
}


////////////////////////////////////////////////////////////////////////////
// Call this from main to do the main menu
int doMenuMain(MenuMain * m) {
	RefPtr<MenuMain> menu = m;
	if (!m)
		menu = new MenuMain();
	menu->doMenu();
	return cur_option;
}

