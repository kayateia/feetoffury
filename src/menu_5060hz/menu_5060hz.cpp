/*
   Feet of Fury

   menu_5060hz/menu_5060hz.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_5060hz.h"
#include "genmenu.h"
#include "res.h"

#include "cube.h"
#include "rotator.h"
#include "log3dmover.h"
#include "exp3dmover.h"
#include "hsvcycler.h"
#include "starfield.h"
#include "textdisplay.h"
#include "gridselector.h"

CVSID("$Id: menu_5060hz.cc,v 1.3 2003/04/07 06:59:15 bard Exp $");

static const char *msgs_text1[] = {
	"Select this option to stay with",
	"the 50Hz display setting."
};
static const char *msgs_text2[] = {
	"Select this option to switch to the",
	"the 60Hz display setting. If you are",
	"not sure your TV can do this, use 'Test'."
};
static const char *msgs_text3[] = {
	"Switches to 60Hz for a few seconds so",
	"you can make sure your TV works with it.",
	"This menu will return in 50Hz after a few",
	"moments."
};
static const char *msgs_text4[] = {
	"This display is now in 60Hz mode. If you",
	"can see this, then your TV is capable of",
	"displaying 60Hz PAL and you should select",
	"'60' on the menu for a higher quality",
	"display during the game."
};

static struct msg {
	const char 	**str;
	int		len;
} msgs[] = {
	{ msgs_text1, 2 },
	{ msgs_text2, 3 },
	{ msgs_text3, 4 },
	{ msgs_text4, 5 }
};


// Put everything in a class wrapper to make the ref counting easier
class Menu5060Hz : public GenericMenu {
public:
	Menu5060Hz();
	virtual ~Menu5060Hz();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	// What's the outcome of the menu?
	enum {
		Use50, Use60
	}	outcome;

private:
	void ditchTextures();
	void reloadTextures(bool initial);

	void switchToTest();
	void switchToNormal();

	virtual void visualPerFrame();

	// Starfield in the background
	RefPtr<StarField> sf;

	// Cube in the background
	RefPtr<Cube> cube;

	// The grid selector for the option selection
	RefPtr<GridSelector> gs;

	// Our font
	RefPtr<Font> font;

	// Menu label
	RefPtr<Label> label;

	// Text display for option descriptions
	RefPtr<TextDisplay> td;

	// Sound effects
	RefPtr<Sound> sfx_cursor, sfx_select;

	// Are we displaying the 60Hz test? If so, this is a 
	// frame countdown until we switch back. Otherwise it's -1.
	int	test60;
};

void Menu5060Hz::ditchTextures() {
	cube->setTexture(NULL);
	gs->setTextureAt(0, 0, NULL, NULL);
	gs->setTextureAt(1, 0, NULL, NULL);
	gs->setTextureAt(2, 0, NULL, NULL);
	td->setFont(NULL);
	label->setFont(NULL);
	font = NULL;
}

void Menu5060Hz::reloadTextures(bool initial) {
	cube->setTexture( new Texture("preload/gfx_misc/scrolly_logo2.png", false) );

	Texture * btns[3] = {
		new Texture("preload/5060hz/btn_50.png", false),
		new Texture("preload/5060hz/btn_60.png", false),
		new Texture("preload/5060hz/btn_test.png", false) };
	gs->setTextureAt(0, 0, btns[0], btns[0]);
	gs->setTextureAt(1, 0, btns[1], btns[1]);
	gs->setTextureAt(2, 0, btns[2], btns[2]);

	if (!initial) {
		font = new Font("preload/fonts/axaxax.txf");
		td->setFont(font);
		label->setFont(font);
	}
}

void Menu5060Hz::switchToTest() {
	m_scene->subRemove(gs);
	m_scene->subRemove(label);
	
	td->setTranslate(Vector(320.0f, 240.0f, 15.0f));
	td->setText(msgs[3].str, msgs[3].len);

	ditchTextures();
	gfxSwitchTo60();
	reloadTextures(false);
}

void Menu5060Hz::switchToNormal() {
	int x, y;
	gs->getSelection(0, &x, &y);

	m_scene->subAdd(gs);
	m_scene->subAdd(label);

	td->setTranslate(Vector(320.0f, 360.0f, 15.0f));
	td->setText(msgs[x].str, msgs[x].len);

	ditchTextures();
	gfxSwitchTo50();
	reloadTextures(false);
}


////////////////////////////////////////////////////////////////////////////

void Menu5060Hz::startExit() {
	Exp3DMover * mover3d = new Exp3DMover(0.0f, 0.0f, -95.0f/(4*hz), 0.0f, 0.0f, -100.0f);
	AlphaFader * fader = new AlphaFader(0.0f, -1.0f/hz);
	cube->animAdd(mover3d);
	cube->animAdd(fader);
	fader->triggerAdd(new Death());

	ExpXYMover * mover = new ExpXYMover(0.0f, -1.0f, 320.0f, -120.0f);
	label->animAdd(mover);
	mover->triggerAdd(new Death());

	fader = new AlphaFader(0.0f, -1.0f/hz);
	gs->animAdd(fader);
	fader->triggerAdd(new Death());

	mover = new ExpXYMover(0.0f, 1.0f, 320.0f, 480.0f+240.0f);
	td->animAdd(mover);
	mover->triggerAdd(new Death());

	TintFader * tf = new TintFader(Color(1,0,0,0), Color(0,-1.0f/hz,-1.0f/hz,-1.0f/hz));
	sf->animAdd(tf);
	tf->triggerAdd(new Death());

	GenericMenu::startExit();
}

void Menu5060Hz::visualPerFrame() {
	if (test60 != -1) {
		test60--;
		if (test60 == 0) {
			switchToNormal();
			test60 = -1;
		}
	}
	GenericMenu::visualPerFrame();
}

void Menu5060Hz::inputEvent(const Event & evt) {
	if (evt.type != Event::EvtKeypress)
		return;

	int x, y;
	bool updateGrid = false;
	gs->getSelection(0, &x, &y);

	switch (evt.key) {
	case Event::KeyStart:
		startExit();
		break;

	case Event::KeyLeft:
		x--;
		updateGrid = true;
		break;

	case Event::KeyRight:
		x++;
		updateGrid = true;
		break;

	case Event::KeySelect:
		sfx_select->play();

		if (test60 != -1) {
			test60 = 1;
		} else {
			switch (x) {
			case 0:
				outcome = Use50;
				startExit();
				break;
			
			case 1:
				outcome = Use60;
				startExit();
				break;
			
			case 2:
				switchToTest();
				test60 = 10*60;
				break;
			}
		}
		break;

	}

	if (updateGrid) {
		sfx_cursor->play();
		if (x < 0) x += 3;
		if (x >= 3) x -= 3;
		gs->setSelection(0, x, y);
		td->setText(msgs[x].str, msgs[x].len);
	}
}

Menu5060Hz::Menu5060Hz() {
	// Setup the basic gfx objects
	sf = new StarField(512);
	sf->setTranslate(Vector(0.0f, 0.0f, 0.001f));
	m_scene->subAdd(sf);

	cube = new Cube(PLX_LIST_TR_POLY, NULL);
	cube->setTranslate(Vector(0.0f, 0.0f, -100.0f));
	cube->setTint(Color(0.0f,1,1,1));
	cube->animAdd(new Rotator());
	cube->animAdd(new Log3DMover(0.0f, 0.0f, -5.0f));
	cube->animAdd(new AlphaFader(0.7f, 1.0f/hz));
	cube->animAdd(new HsvCycler());
	m_scene->subAdd(cube);

	font = new Font("preload/fonts/axaxax.txf");
	label = new Label(font, "PAL TV Selection", 32, true, true);
	label->setTranslate(Vector(-320.0f, 120.0f, 15.0f));
	label->animAdd(new LogXYMover(320.0f, 120.0f));
	m_scene->subAdd(label);

	gs = new GridSelector(3, 1, 64, 64, 1);
	gs->setSelAttrs(0, 0.3f, 1.0f, 0.3f, NULL);
	gs->setSelection(0, 0,0);
	gs->setNumSels(1);
	gs->setTranslate(Vector(320.0f, 240.0f, 15.0f));
	gs->setTint(Color(0.0f,1,1,1));
	gs->animAdd(new AlphaFader(1.0f, 1.0f/hz));
	m_scene->subAdd(gs);

	td = new TextDisplay(font, 18.0f, false);
	td->setTranslate(Vector(640+320.0f, 360.0f, 15.0f));
	td->animAdd(new LogXYMover(320.0f, 360.0f));
	td->setText(msgs[0].str, msgs[0].len);
	td->setBGAlpha(0.0f);
	m_scene->subAdd(td);

	// Reload all of our textures
	reloadTextures(true);

	// Load sound effects
	sfx_cursor = res.getSound(Res::Preload, "menu_cursor");
	sfx_select = res.getSound(Res::Preload, "options_select");

	test60 = -1;
}

Menu5060Hz::~Menu5060Hz() {
}

////////////////////////////////////////////////////////////////////////////
void doMenu5060Hz() {
	// If we got here at all, we're obviously a European DC. So make
	// sure we're in 50Hz mode.
	if (hz != 50)
		gfxSwitchTo50();

	int ohz = -1;
	{
		Menu5060Hz m;
		m.doMenu();

		if (m.outcome == Menu5060Hz::Use50)
			ohz = 50;
		else
			ohz = 60;
	}

	switch (ohz) {
	case 50:
		if (hz != 50)
			gfxSwitchTo50();
		break;
	case 60:
		if (hz != 60)
			gfxSwitchTo60();
		break;
	}
	pvr_mem_stats();
}

