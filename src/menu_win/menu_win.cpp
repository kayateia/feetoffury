/*
   Feet of Fury

   menu_win/menu_win.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_win.h"
#include "plrstate.h"
#include "character.h"
#include "res.h"

#include "bubblefield.h"
#include "textdisplay.h"
#include "wavetxr.h"

////////////////////////////////////////////////////////////////////////////

class MenuWin : public GenericMenu {
public:
	MenuWin(int index, PlayerState * pst);
	virtual ~MenuWin();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	// Labels for winning player
	RefPtr<Label> lbl_winner[2];

	// WaveTexture for character pic
	// RefPtr<WaveTexture> char_pic;
	RefPtr<Banner> char_pic;

	// Our font
	RefPtr<Font> font;

	// Text display for character saying
	RefPtr<TextDisplay> td;

	// Bubble background
	RefPtr<BubbleField> bf;
};

////////////////////////////////////////////////////////////////////////////

// Begin the drawable exodus
void MenuWin::startExit() {
	// Put a mover on all the objects with a death trigger
	AlphaFader * fader = new AlphaFader(0.0f, -1.0f/(hz/2));
	fader->triggerAdd(new Death());
	bf->animAdd(fader);

	fader = new AlphaFader(0.0f, -1.0f/(hz/2));
	fader->triggerAdd(new Death());
	td->animAdd(fader);

	fader = new AlphaFader(0.0f, -1.0f/(hz/2));
	fader->triggerAdd(new Death());
	lbl_winner[0]->animAdd(fader);

	fader = new AlphaFader(0.0f, -1.0f/(hz/2));
	fader->triggerAdd(new Death());
	lbl_winner[1]->animAdd(fader);

	/* WaveTextureAnim * wta = new WaveTextureAnim(16.0f, 16.0f/(hz*2), 0.0f, -1.0f/(hz*2));
	wta->triggerAdd(new Death());
	char_pic->animAdd(wta); */
	fader = new AlphaFader(0.0f, -1.0f/(hz/2));
	fader->triggerAdd(new Death());
	char_pic->animAdd(fader);

	GenericMenu::startExit();
}

void MenuWin::inputEvent(const Event & evt) {
	if (evt.type == Event::EvtTimeout)
		startExit();
	else if (evt.type == Event::EvtKeypress)
		startExit();
}

// Call this from main to do the main menu
MenuWin::MenuWin(int index, PlayerState * pst) {
	setTimeout(10);

	Debug::printf("MenuWin: Character index is %d\n", pst->charidx);
	if (pst->charidx < 0 || pst->charidx >= Characters::count) {
		Debug::printf("INVALID CHARCTER ID!\n");
		pst->charidx = 0;
	}

	// Bubble background
	bf = new BubbleField(res.getTexture(Res::Preload, "bubble", true), 128);
	bf->setTranslate(Vector(0.0f, 0.0f, 0.0002f));
	m_scene->subAdd(bf);

	Texture * char_txr = NULL;
	// char_txr = res.getTexture(Res::MenuChar, Characters::info[pst->charidx].txr_large, true);
	char_txr = pst->char_full;

	// Character pic
	/* char_pic = new WaveTexture(char_txr, PLX_LIST_TR_POLY);
	char_pic->setTranslate(Vector(180.0f, 240.0f, 10.0f));
	char_pic->setTint(Color(0,1,1,1));
	char_pic->setMag(16.0f);
	char_pic->animAdd(new WaveTextureAnim(0.0f, -16.0f/120.0f, 1.0f, 1.0f/120.0f));
	m_scene->subAdd(char_pic); */
	char_pic = new Banner(Drawable::Trans, char_txr);
	char_pic->setTranslate(Vector(180.0f, 240.0f, 10.0f));
	char_pic->setTint(Color(0.0f,1,1,1));
	char_pic->animAdd(new AlphaFader(1.0f, 1.0f/hz));
	m_scene->subAdd(char_pic);

	// Font
	font = res.getFont("axaxax");

	// Winner label
	if (index == 0)
		lbl_winner[0] = new Label(font, "Player 1", 36, true, true);
	else
		lbl_winner[0] = new Label(font, "Player 2", 36, true, true);
	lbl_winner[0]->setTranslate(Vector(640.0f - 200.0f, 100.0f, 10.0f));
	lbl_winner[0]->setTint(Color(0.0f,1,1,1));
	lbl_winner[0]->animAdd(new AlphaFader(1.0f, 1.0f/hz));
	m_scene->subAdd(lbl_winner[0]);

	lbl_winner[1] = new Label(font, "- WIN -", 36, true, true);
	lbl_winner[1]->setTranslate(Vector(640.0f - 200.0f, 140.0f, 10.0f));
	lbl_winner[1]->setTint(Color(0.0f,1,1,1));
	lbl_winner[1]->animAdd(new AlphaFader(1.0f, 1.0f/hz));
	m_scene->subAdd(lbl_winner[1]);

	// Character saying display
	const char ** text = Characters::info[pst->charidx].win_text;
	int linecnt = Characters::info[pst->charidx].win_text_cnt;
	td = new TextDisplay(font, 24.0f, true);
	td->setTranslate(Vector(640.0f - 200.0f, 300.0f, 12.0f));
	td->setText(text, linecnt);
	td->setTint(Color(0.0f,1,1,1));
	td->setBGAlpha(0.3f);
	td->animAdd(new AlphaFader(1.0f, 1.0f/hz));
	m_scene->subAdd(td);
	
	// GL Setup
	setBg(0.05f, 0.0f, 0.20f);

	// Start Ogg playing
	setBgm("bgm/winner.ogg");

	resetTimeout();
}

MenuWin::~MenuWin() {
}


void doMenuWin(int index, PlayerState * pst) {
	MenuWin m(index, pst);
	m.doMenu();
}
