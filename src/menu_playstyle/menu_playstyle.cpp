/*
   Feet of Fury

   menu_playstyle/menu_playstyle.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_playstyle.h"
#include "res.h"
#include "pak.h"
#include "plrstate.h"
#include "character.h"

#include "scrollybg.h"	// #include "starfield.h"
#include "cubecube.h"
#include "menubox.h"
#include "selbar.h"
#include "textdisplay.h"
#include "attack_mode_popup.h"
#include "gridselector.h"
#include "popup.h"
#include "soundfader.h"

MenuPlayStyle::MPSRes::MPSRes() {
	/* tech_txr[AttackBomb] = res.getTexture(Res::MenuStage, "fury_icons/bomb");
	tech_txr[AttackDrunkenArrow] = res.getTexture(Res::MenuStage, "fury_icons/drunken");
	tech_txr[AttackVortex] = res.getTexture(Res::MenuStage, "fury_icons/vortex");
	tech_txr[AttackTurbo] = res.getTexture(Res::MenuStage, "fury_icons/turbo");
	tech_txr[AttackPhasingArrows] = res.getTexture(Res::MenuStage, "fury_icons/phasing");
	tech_txr[AttackFireball] = res.getTexture(Res::MenuStage, "fury_icons/fireball");
	tech_txr[AttackHealth] = res.getTexture(Res::MenuStage, "fury_icons/health");
	tech_txr[AttackEraser] = res.getTexture(Res::MenuStage, "fury_icons/eraser");
	tech_txr[AttackDispel] = res.getTexture(Res::MenuStage, "fury_icons/dispel");
	tech_txr[AttackReverse] = res.getTexture(Res::MenuStage, "fury_icons/reverse"); */

	attack_modes = res.getTexture(Res::MenuStage, "attack_modes", true);

	diff[0] = res.getTexture(Res::MenuStage, "easy");
	diff[1] = res.getTexture(Res::MenuStage, "medium");
	diff[2] = res.getTexture(Res::MenuStage, "hard");

	font = res.getFont("axaxax");

	sfx_cursor = res.getSound(Res::Preload, "menu_cursor");
	sfx_select = res.getSound(Res::Preload, "menu_accept");
}

MenuPlayStyle::MPSRes::~MPSRes() {
}

///////////////////////////////////////////////////////////////////////

MenuPlayStyle::MenuPlayStyle(PlayerState * p1, PlayerState * p2, StepData * song, bool typing, bool practice) {
	// What's our input source?
// XXX
#if 0
	setDeviceType(typing ? Event::TypeKeyboard : Event::TypeController);
	setPlayerCount(2);
#endif

	m_practice = practice;

	// Get our resources
	m_res = new MPSRes();

	m_song = song;

	m_subscene = new Scene();
	m_scene->subAdd(m_subscene);

	// Create our scene-global visuals
	/* m_sf = new StarField(512);
	m_sf->setTranslate(Vector(0,0,0.001));
	m_scene->subAdd(m_sf); */
	m_sf = new ScrollyBG(res.getTexture(Res::Preload, "scrolly_logo2"));
	m_sf->setTranslate(Vector(0,0,0.001));
	m_subscene->subAdd(m_sf);

	// And two player widgets
	m_players[0] = new Plr(this, p1, m_res, -2.5f, 0);
	m_players[0]->setTranslate(Vector(180,240,10.0f));
	m_subscene->subAdd(m_players[0]);
	m_players[1] = new Plr(this, p2, m_res, 2.5f, 1);
	if (!practice || !p2->ai) {
		m_players[1]->setTranslate(Vector(320+140,240, 10.0f));
		m_subscene->subAdd(m_players[1]);
	}

	// Default result
	m_result = Main::Ok;

	setBgm("bgm/fury.ogg");

	setTimeout(60);
}

MenuPlayStyle::~MenuPlayStyle() {
}

void MenuPlayStyle::startExit() {
	PlayerGenericMenu::startExit();

	Color d(0, -1.0f/(hz/2), -1.0f/(hz/2), -1.0f/(hz/2));
	TintFader * tf = new TintFader(Color(0,0,0), d);
	tf->triggerAdd(new Death());
	m_scene->animAdd(tf);
}

const char * MenuPlayStyle::m_cancelOpt = "Cancel";
const char * MenuPlayStyle::m_mainMenuOpt = "Back to Main Menu";
const char * MenuPlayStyle::m_stageMenuOpt = "Back to Song Menu";
const char * MenuPlayStyle::m_charMenuOpt = "Back to Character Select";
const char * MenuPlayStyle::m_swapMenuOpt = "Back to Swap CD Menu";

void MenuPlayStyle::sysMenuCreate() {
	m_sysmenu = new Popup("System Menu");
	m_sysmenu->setTranslate(Vector(320.0f, 240.0f, 900.0f));
#ifndef KIOSK_MODE
	m_sysmenu->addOption(m_mainMenuOpt);
#endif
	m_sysmenu->addOption(m_stageMenuOpt);
	if (!m_practice) {
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

void MenuPlayStyle::sysMenuDestroy() {
	m_scene->animRemoveAll();
	m_scene->animAdd( new SoundFader(1.0f, 0.6f/(hz/2)) );
	m_subscene->animRemoveAll();
	m_subscene->animAdd(
		new TintFader(Color(1,1,1), Color(0.5f/(hz/2),0.5f/(hz/2),0.5f/(hz/2))) );
	m_sysmenu = NULL;
}

void MenuPlayStyle::sysMenuKey(Event::KeyConstant key) {
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
	} else if (!strcmp(optname, m_stageMenuOpt)) {
		m_result = Main::MenuStage;
	} else {
		Debug::printf("sysmenu returned unknown result %d\n", rv);
		m_result = Main::MenuMain;
	}

	m_sysmenu = NULL;
	startExit();
}

void MenuPlayStyle::inputEvent(const Event & evt) {
	if (evt.type == Event::EvtTimeout) {
		if (m_sysmenu) {
			m_sysmenu->exitNow();
			sysMenuDestroy();
		}
		m_res->sfx_select->play();
		startExit();
	} else
		PlayerGenericMenu::inputEvent(evt);
}

void MenuPlayStyle::processKeypress(int player, Event::KeyConstant key) {
	resetTimeout();

	if (m_sysmenu) {
		sysMenuKey(key);
		return;
	}

	if (key == Event::KeyCancel) {
		sysMenuCreate();
		return;
	}

	if (player < 2) {
		Event evt;
		evt.type = Event::EvtKeypress;
		evt.key = key;
		m_players[player]->inputEvent(evt);
	}

	if (m_players[0]->finished() && m_players[1]->finished())
		startExit();
}

///////////////////////////////////////////////////////////////////////
static const char * txt_am_auto[] = {
	"Auto attacks launch",
	"an item as soon as",
	"it enters your",
	"inventory."
};
static const int txt_am_auto_cnt = 4;

static const char * txt_am_manual[] = {
	"Manual attacks will",
	"require you to trigger",
	"the usage of an",
	"obtained item."
};
static const int txt_am_manual_cnt = 4;

/*
#define BOILER \
	"Fury Selection", \
	"This controls the mix", \
	"of items you receive.",
#define BOILER_CNT 3

// C, R, H, D, CR, CD, DH, RH

static const char * txt_cc_0[] = {
	BOILER
	"Confusion:",
	"Drunken Arrows,",
	"Vortex, Turbo,",
	"Phasing Arrows"
};
static const int txt_cc_0_cnt = 4+BOILER_CNT;

static const char * txt_cc_1[] = {
	BOILER
	"Rescue:",
	"Reprieve, Dispel,",
	"Reverse"
};
static const int txt_cc_1_cnt = 3+BOILER_CNT;

static const char * txt_cc_2[] = {
	BOILER
	"Damage:",
	"Fireball, Bomb"
};
static const int txt_cc_2_cnt = 2+BOILER_CNT;

static const char * txt_cc_3[] = {
	BOILER
	"Health:",
	"Elixir"
};
static const int txt_cc_3_cnt = 2+BOILER_CNT;

static const char * txt_cc_4[] = {
	BOILER
	"Conf/Resc:",
	"All items from",
	"Confusion and Rescue"
};
static const int txt_cc_4_cnt = 3+BOILER_CNT;

static const char * txt_cc_5[] = {
	BOILER
	"Conf/Damage:",
	"All items from",
	"Confusion and Damage"
};
static const int txt_cc_5_cnt = 3+BOILER_CNT;

static const char * txt_cc_6[] = {
	BOILER
	"Resc/Health:",
	"All items from Rescue",
	"and Health"
};
static const int txt_cc_6_cnt = 3+BOILER_CNT;

static const char * txt_cc_7[] = {
	BOILER
	"Damage/Health:",
	"All items from Damage",
	"and Health"
};
static const int txt_cc_7_cnt = 3+BOILER_CNT;

struct txt_cc_t {
	const char	** text;
	int		cnt;
} txt_cc[8] = {
	{ txt_cc_0, txt_cc_0_cnt },
	{ txt_cc_1, txt_cc_1_cnt },
	{ txt_cc_3, txt_cc_3_cnt },
	{ txt_cc_2, txt_cc_2_cnt },
	{ txt_cc_4, txt_cc_4_cnt },
	{ txt_cc_5, txt_cc_5_cnt },
	{ txt_cc_7, txt_cc_7_cnt },
	{ txt_cc_6, txt_cc_6_cnt }
};
*/

static char txt_diff_bufs[2][64] = { {0}, {0} };
static const char * txt_diff_0[] = {
	"Difficulty Selection",
	txt_diff_bufs[0]
};
static const char * txt_diff_1[] = {
	"Difficulty Selection",
	txt_diff_bufs[1]
};
static const char ** txt_diffs[] = {
	txt_diff_0,
	txt_diff_1
};
static const int txt_diff_cnt = 2;


MenuPlayStyle::Plr::Plr(MenuPlayStyle * parent, PlayerState * ps, MPSRes * res, float tdxoffs, int idx) {
	m_parent = parent;
	m_ps = ps;
	m_res = res;
	m_idx = idx;

	// Character banner
	if (!m_parent->m_practice) {
		m_char = new Banner(Drawable::Trans, m_ps->char_full);
		m_char->setTint(Color(0.3f,0.3f,0.3f));
		subAdd(m_char);
	}

	if (!m_ps->ai) {
		// Auto/Manual attack mode
		m_am = new AttackModePopup(m_idx, m_res->font, m_res->attack_modes, 
			m_res->sfx_cursor, m_res->sfx_select, m_ps);
		m_am->setTranslate(Vector(0,-80,5));
		m_am->setActive(AttackModePopup::ActEnabled);

		// Fury selector
		/* m_cc = new CubeCube(m_res, m_ps->fury);
		m_cc->setTranslate(Vector(tdxoffs, +1.0f, -10.0f));
		m_cc->setScale(Vector(0.4f,0.4f,0.4f)); */

		// Difficulty selector
		m_gs = new GridSelector(3, 1, 64, 64, 1);
		if (idx == 0)
			m_gs->setSelAttrs(0, 1.0f, 0.3f, 0.3f, NULL);
		else
			m_gs->setSelAttrs(0, 0.3f, 1.0f, 0.3f, NULL);
		m_gs->setSelection(0, 0,0);
		m_gs->setNumSels(1);
		m_gs->setTextureAt(0, 0, m_res->diff[0], m_res->diff[0]);
		m_gs->setTextureAt(1, 0, m_res->diff[1], m_res->diff[1]);
		m_gs->setTextureAt(2, 0, m_res->diff[2], m_res->diff[2]);
		m_gs->setTranslate(Vector(0,0,5));

		// Start out on easy
		m_ps->diff = StepData::Easy;
		updateDiff(false);

		// "Decided" label
		m_decided = new Label(m_res->font, "DECIDED", 36, true, true);
		m_decided->setTranslate(Vector(0,0,10));

		// Help display
		m_td = new TextDisplay(m_res->font, 18, true);
		m_td->setTranslate(Vector(0,120, 3));
		m_td->setText(txt_am_auto, txt_am_auto_cnt);
		m_td->setBGAlpha(0.3f);
		m_td->setTint(Color(0.8f,0.8f,0.8f));
		subAdd(m_td);

		// Start out in attack select mode, unless we're in practice
		// mode.
		if (!m_parent->m_practice) {
			m_state = StateAttackMode;
			subAdd(m_am);
		} else {
			m_state = StateDifficulty;
			subAdd(m_gs);
			updateDiff(true);
		}
	} else {
		m_state = StateDecided;
	}
}

MenuPlayStyle::Plr::~Plr() {
}

void MenuPlayStyle::Plr::draw(ObjType list) {
	Drawable::draw(list);
}

void MenuPlayStyle::Plr::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
}

void MenuPlayStyle::Plr::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type != Event::EvtKeypress)
		return;

	switch (m_state) {
	case StateAttackMode: {
		AttackModePopup::Response rsp;
		rsp = m_am->processKeypress((Event::KeyConstant)evt.key);
		if (rsp == AttackModePopup::RespSelect) {
			subRemove(m_am);
			/* subAdd(m_cc);
			m_state = StateFury;
			m_td->setText(txt_cc[m_ps->fury].text, txt_cc[m_ps->fury].cnt); */
			subAdd(m_gs);
			m_state = StateDifficulty;
			updateDiff(true);
		} else {
			if (m_ps->auto_attack)
				m_td->setText(txt_am_auto, txt_am_auto_cnt);
			else
				m_td->setText(txt_am_manual, txt_am_manual_cnt);
		}
		break;
	}

	/* case StateFury:
		switch (evt.key) {
		case Event::KeyStart:
			m_parent->startExit();
			break;

		case Event::KeySelect:
			m_res->sfx_select->play();
			subRemove(m_cc);
			subAdd(m_gs);
			m_state = StateDifficulty;
			updateDiff(true);
			break;

		case Event::KeyCancel:
			m_res->sfx_cursor->play();
			subRemove(m_cc);
			subAdd(m_am);
			m_state = StateAttackMode;
			if (m_ps->auto_attack)
				m_td->setText(txt_am_auto, txt_am_auto_cnt);
			else
				m_td->setText(txt_am_manual, txt_am_manual_cnt);
			break;
			
		case Event::KeyLeft:
			moveRight();
			break;

		case Event::KeyRight:
			moveLeft();
			break;

		case Event::KeyUp:
			moveDown();
			break;

		case Event::KeyDown:
			moveUp();
			break;
		}

		if (evt.key != Event::KeySelect && evt.key != Event::KeyCancel)
			m_td->setText(txt_cc[m_ps->fury].text, txt_cc[m_ps->fury].cnt);
		break; */

	case StateDifficulty:
		switch (evt.key) {
		case Event::KeySelect:
			m_res->sfx_select->play();
			subRemove(m_gs);
			subAdd(m_decided);
			m_state = StateDecided;
			subRemove(m_td);
			break;

		case Event::KeyCancel:
			if (!m_parent->m_practice) {
				m_res->sfx_cursor->play();
				subRemove(m_gs);
				/* subAdd(m_cc);
				m_td->setText(txt_cc[m_ps->fury].text, txt_cc[m_ps->fury].cnt);
				m_state = StateFury; */
				subAdd(m_am);
				m_state = StateAttackMode;
				if (m_ps->auto_attack)
					m_td->setText(txt_am_auto, txt_am_auto_cnt);
				else
					m_td->setText(txt_am_manual, txt_am_manual_cnt);
			}
			break;

		case Event::KeyLeft:
			switch (m_ps->diff) {
			case StepData::Easy:
				m_ps->diff = StepData::Hard;
				break;
			case StepData::Medium:
				m_ps->diff = StepData::Easy;
				break;
			case StepData::Hard:
				m_ps->diff = StepData::Medium;
				break;
			}
			break;

		case Event::KeyRight:
			switch (m_ps->diff) {
			case StepData::Easy:
				m_ps->diff = StepData::Medium;
				break;
			case StepData::Medium:
				m_ps->diff = StepData::Hard;
				break;
			case StepData::Hard:
				m_ps->diff = StepData::Easy;
				break;
			}
			break;
		}

		if (evt.key == Event::KeyLeft || evt.key == Event::KeyRight) {
			m_res->sfx_cursor->play();
			updateDiff(true);
		}
		break;

	case StateDecided:
		if (evt.key == Event::KeyCancel) {
			m_res->sfx_select->play();
			subRemove(m_decided);
			subAdd(m_gs);
			subAdd(m_td);
			m_state = StateDifficulty;
		}
			
		break;

	}
}

void MenuPlayStyle::Plr::updateDiff(bool tdalso) {
	int lvl = m_parent->m_song->step_level[m_ps->diff];
	switch (m_ps->diff) {
	case StepData::Easy:
		m_gs->setSelection(0, 0,0);
		sprintf(txt_diff_bufs[m_idx], "Easy: %d (", lvl);
		break;
	case StepData::Medium:
		m_gs->setSelection(0, 1,0);
		sprintf(txt_diff_bufs[m_idx], "Medium: %d (", lvl);
		break;
	case StepData::Hard:
		m_gs->setSelection(0, 2,0);
		sprintf(txt_diff_bufs[m_idx], "Hard: %d (", lvl);
		break;
	}

	char * o = txt_diff_bufs[m_idx] + strlen(txt_diff_bufs[m_idx]);
	for (int i=0; i<lvl; i++) {
		*o++ = '*';
	}
	*o++ = ')';
	*o++ = '\0';

	if (tdalso)
		m_td->setText(txt_diffs[m_idx], txt_diff_cnt);
}

/* void MenuPlayStyle::Plr::moveUp() {
	moveDown();
}

void MenuPlayStyle::Plr::moveDown() {
	int p = m_cc->getSelect();
	p = (p + 4) % 8;
	m_cc->switchSelect(p);
	m_ps->fury = p;

	m_res->sfx_cursor->play();
}

void MenuPlayStyle::Plr::moveLeft() {
	int p = m_cc->getSelect();
	if (p < 4) {
		p = (p - 1) & 3;
	} else {
		p -= 4;
		p = (p - 1) & 3;
		p += 4;
	}
	m_cc->switchSelect(p);
	m_ps->fury = p;

	m_res->sfx_cursor->play();
}

void MenuPlayStyle::Plr::moveRight() {
	int p = m_cc->getSelect();
	if (p < 4) {
		p = (p + 1) & 3;
	} else {
		p -= 4;
		p = (p + 1) & 3;
		p += 4;
	}
	m_cc->switchSelect(p);
	m_ps->fury = p;

	m_res->sfx_cursor->play();
} */

///////////////////////////////////////////////////////////////////////

void testMenuPlayStyle() {
	//pakMount("menu_stage");
	//pakMount("menu_char");
	RefPtr<PlayerState> p1 = new PlayerState(), p2 = new PlayerState();
	p1->charidx = randnum(CHAR_COUNT-1);
	p2->charidx = randnum(CHAR_COUNT-1);
	p1->char_full = res.getTexture(Res::MenuChar, Characters::info[p1->charidx].txr_large, true);
	p2->char_full = res.getTexture(Res::MenuChar, Characters::info[p2->charidx].txr_large, true);
	//pakUnmount("menu_char");
	RefPtr<MenuPlayStyle> mps = new MenuPlayStyle(p1, p2, loadSteps("omm_ramp"), false, false);
	//pakUnmount("menu_stage");
	mps->doMenu();
}

