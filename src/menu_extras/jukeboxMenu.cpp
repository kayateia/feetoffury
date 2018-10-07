/*
   Feet of Fury

   menu_extras/jukeboxMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"
#include "ogg.h"
#include "menu_extras.h"
#include "unlock.h"
#include "options.h"

#include "jukeboxMenu.h"

#include "gridselector.h"
#include "menubox.h"
#include "soundfader.h"
#include "messagebox.h"

static const int jukeW = 6, jukeH = 6;

struct JukeItem {
	enum Type {
		Sound,
		Song,
		None
	};
	JukeItem(const char *t = NULL, const char *a = NULL, const char *f = NULL, bool decr = false, Type ty = None) {
		title = t;
		artist = a;
		fn = f;
		decrypt = decr;
		type = ty;
	}
	const char	* title;
	const char	* artist;
	const char	* fn;
	bool		decrypt;
	Type		type;
};


// XXX ... EVIL!

#define RODDY "Roddy Toomim"
#define KUDOS "KUDOS"
#define MARKS "Aaron Marks"
#define CA "Cryptic Allusion"
#define CHOJIN "CHOJIN"

static JukeItem items[36] = {
	JukeItem("Allusions", MARKS, "songs/omm_allusions/song.ogg", true, JukeItem::Song),
	JukeItem("BHedU", RODDY, "songs/djg_bhedu/song.ogg", true, JukeItem::Song),
	JukeItem("Man in the Moon", KUDOS, "songs/kud_mitm/song.ogg", true, JukeItem::Song),
	JukeItem("House", MARKS, "songs/omm_house/song.ogg", true, JukeItem::Song),
	JukeItem("Decrystalized Buff Log", RODDY, "songs/djg_bufflog/song.ogg", true, JukeItem::Song),
	JukeItem("Me N U", KUDOS, "songs/kud_menu/song.ogg", true, JukeItem::Song),

	JukeItem("Pyro", MARKS, "songs/omm_pyro/song.ogg", true, JukeItem::Song),
	JukeItem("Buff Log (Log Sine Remix)", RODDY, "songs/djg_bufflsr/song.ogg", true, JukeItem::Song),
	JukeItem("Dreams2", KUDOS, "songs/kud_dreams2/song.ogg", true, JukeItem::Song),
	JukeItem("Ramp", MARKS, "songs/omm_ramp/song.ogg", true, JukeItem::Song),
	JukeItem("Got No Clue\n  (What I Should Do)", RODDY, "songs/djg_gotnoclue/song.ogg", true, JukeItem::Song),
	JukeItem("Run the Ridge", MARKS, "songs/omm_run/song.ogg", true, JukeItem::Song),

	JukeItem("Higher", RODDY, "songs/djg_higher/song.ogg", true, JukeItem::Song),
	JukeItem("Catchup", CHOJIN, "songs/cho_catchup/song.ogg", true, JukeItem::Song),
	JukeItem("Those Damn Noodles", RODDY, "songs/djg_noodlesln/song.ogg", true, JukeItem::Song),
	JukeItem("Laptop", CHOJIN, "songs/cho_laptop/song.ogg", true, JukeItem::Song),
	JukeItem("Tisonits", RODDY, "songs/djg_tisonits/song.ogg", true, JukeItem::Song),
	JukeItem("Quickly", CHOJIN, "songs/cho_quickly/song.ogg", true, JukeItem::Song),

	JukeItem("Trickster", RODDY, "songs/djg_trickster/song.ogg", true, JukeItem::Song),
	JukeItem("NRG", KUDOS, "songs/kud_nrg/song.ogg", true, JukeItem::Song),
	JukeItem("Loungin'", RODDY, "songs/djg_loungin/song.ogg", true, JukeItem::Song),
	JukeItem("Watoo", KUDOS, "songs/kud_watoo/song.ogg", true, JukeItem::Song),
	JukeItem("Aquatic", RODDY, "extras/jb_12.dat", true, JukeItem::Song),
	JukeItem("Cereal Experiments (Plain)", RODDY, "extras/jb_13.dat", true, JukeItem::Song),

	JukeItem("Fonky Scramble", RODDY, "extras/jb_14.dat", true, JukeItem::Song),
	JukeItem("Got No Clue A-Kon Mix", RODDY, "extras/jb_15.dat", true, JukeItem::Song),
	JukeItem("Radio For Help (old mix)", RODDY, "extras/jb_16.dat", true, JukeItem::Song),
	JukeItem("Movie Guy Roddy", "Roddy and Alan", "extras/jb_17.dat", true, JukeItem::Sound),
	JukeItem("Random Voice Funnies", "Bard and Geki", "extras/jb_18.dat", true, JukeItem::Sound),
	JukeItem("Funny Audio Bug", CA, "extras/jb_19.dat", true, JukeItem::Sound),

	JukeItem("Come On Everybody", CA, "extras/jb_20.dat", true, JukeItem::Song),
	JukeItem("DJ Geki Thanks", CA, "extras/jb_31.dat", true, JukeItem::Sound),
	JukeItem("2ndMix Extended", RODDY, "extras/jb_32.dat", true, JukeItem::Song),
	JukeItem("Train Announcer", CA, "extras/jb_33.dat", true, JukeItem::Sound),
	JukeItem("Deserted Town", RODDY, "extras/jb_34.dat", true, JukeItem::Song),
	JukeItem("My Heart", RODDY, "extras/jb_35.dat", true, JukeItem::Song)
};

JukeboxMenu::JukeboxMenu(MenuExtras * parent, ExtrasMenu * back)
	: ExtrasMenu(parent)
{
	m_back = back;

	m_txrLocked = res.getTexture(Res::Unlock, "unknown_gray");
	m_txrSong = res.getTexture(Res::Unlock, "song");
	m_txrSound = res.getTexture(Res::Unlock, "sound");

	m_title = new Label(m_parent->getFont(), "Sound Check", 36, true, false);
	m_title->setTranslate(Vector(320.0f, 60.0f, 15.0f));
	subAdd(m_title);

	float x = 300.0f;
	float y = 140;

	m_artistLbl = new Label(m_parent->getFont(), "Artist", 24, false, false);
	m_artistLbl->setTranslate(Vector(x, y, 15.0f));
	m_artistLbl->setTint(Color(0.85f,0.85f,0));
	subAdd(m_artistLbl); y += 28.0f;

	m_artistBuf[0] = 0;
	m_artist = new Label(m_parent->getFont(), m_artistBuf, 18, false, false);
	m_artist->setTranslate(Vector(x, y, 15.0f));
	m_artist->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_artist); y += 28.0f;

	y += 16.0f;

	m_nameLbl = new Label(m_parent->getFont(), "Title", 24, false, false);
	m_nameLbl->setTranslate(Vector(x, y, 15.0f));
	m_nameLbl->setTint(Color(0.85f,0.85f,0));
	subAdd(m_nameLbl); y += 28.0f;

	m_nameBuf[0] = 0;
	m_name = new Label(m_parent->getFont(), m_nameBuf, 18, false, false);
	m_name->setTranslate(Vector(x, y, 15.0f));
	m_name->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_name); y += 28.0f;

	y += 16.0f;

	m_info = new Label(m_parent->getFont(), "Press Y for more info\non most tracks.", 18, false, false);
	m_info->setTranslate(Vector(x, y, 15.0f));
	m_info->setTint(Color(0.85f, 0.85f, 0.85f));
	subAdd(m_info); y += 18*2 + 10;

	y += 32.0f;
	m_new_x = x;
	m_new_y = y;

	m_gs = new GridSelector(jukeW, jukeH, 32, 32, 1);
	rescanUnlock();
	m_gs->setSelAttrs(0, 0.3f, 1.0f, 0.3f, NULL);
	m_gs->setSelection(0, 0, 0);
	m_gs->setTranslate(Vector(140,240,13));
	m_gs->setSpacing(2.0f);
	m_gs->setSelectorOutside(false);
	subAdd(m_gs);

	MenuBox * mb = new MenuBox(true);
	mb->setTranslate(Vector(140,240,10));
	mb->setScale(Vector(jukeW*34+2+8, jukeH*34+2+8, 1));
	subAdd(mb);

	m_x = m_y = 0;
	updateBuffers();

	m_fader = new Drawable();
	subAdd(m_fader);
}

JukeboxMenu::~JukeboxMenu() {
}

void JukeboxMenu::updateBuffers() {
	int sq = m_y * jukeW + m_x;
	bool isNew = game_options.isSongPlayed(sq);

	if (items[sq].type == JukeItem::None) {
		strcpy(m_artistBuf, "(None)");
		strcpy(m_nameBuf, "(None)");
		isNew = false;
	} else if (isLocked(sq)) {
		strcpy(m_artistBuf, "?? LOCKED ??");
		strcpy(m_nameBuf, "?? LOCKED ??");
		isNew = false;
	} else {
		strcpy(m_artistBuf, items[sq].artist);
		strcpy(m_nameBuf, items[sq].title);
	}

	if (m_new && !isNew) {
		subRemove(m_new);
		m_new = NULL;
	} else if (!m_new && isNew) {
		m_new = new Label(m_parent->getFont(), "(NEW)", 24, false, false);
		m_new->setTranslate(Vector(m_new_x, m_new_y, 15.0f));
		subAdd(m_new);
	}
}

void JukeboxMenu::startSong(int x, int y) {
	int sq = m_y * jukeW + m_x;

	if (items[sq].type == JukeItem::None) {
		m_parent->sfxInvalid();
	} else {
		m_fader->animRemoveAll();
		oggStop(false);
		if (items[sq].decrypt) {
			char tmp[1024];
			assert( false );
			// XXX
			//sprintf(tmp, "/pd%s/%s", fs_getwd(), items[sq].fn);
			oggStart(tmp, false, false, false);
		} else {
			oggStart(items[sq].fn, false, false, false);
		}

		game_options.setSongPlayed(sq);
	}
}

void JukeboxMenu::FinishTrigger::trigger(Drawable * d, Animation * a) {
	oggStop(false);
}

void JukeboxMenu::gainFocus() {
	m_fader->animRemoveAll();

	SoundFader * sf = new SoundFader(0.0f, -1.0f/(hz/4));
	sf->triggerAdd(new FinishTrigger());
	m_fader->animAdd(sf);
}

bool JukeboxMenu::isLocked(int sq) {
	int ulidx = Unlock::sounds[sq];
	bool locked = !(ulidx == -2) && (ulidx == -1 || game_options.isLocked(ulidx));
	return locked || items[sq].type == JukeItem::None;
}

void JukeboxMenu::rescanUnlock() {
	for (int y=0; y<jukeH; y++)
		for (int x=0; x<jukeW; x++) {
			int sq = y * jukeW + x;
			if (isLocked(sq))
				m_gs->setTextureAt(x, y, m_txrLocked, m_txrLocked);
			else if (items[sq].type == JukeItem::Sound)
				m_gs->setTextureAt(x, y, m_txrSound, m_txrSound);
			else
				m_gs->setTextureAt(x, y, m_txrSong, m_txrSong);
		}
}

void JukeboxMenu::inputEvent(const GenericMenu::Event & evt) {
	bool moved = false;

	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

	if (m_msgbox) {
		m_msgbox->startExit();
		m_msgbox = NULL;
		return;
	}

	switch (evt.key) {
	case GenericMenu::Event::KeySelect:
		if (isLocked(m_y * jukeW + m_x))
			m_parent->sfxInvalid();
		else
			startSong(m_x, m_y);
		break;
	case GenericMenu::Event::KeyCancel:
		m_fader->animRemoveAll();
		oggStop(false);
		oggStart("bgm/options.ogg", true, false, false);
		m_parent->setSubMenu(m_back, false);
		return;
	case GenericMenu::Event::KeyUp:
		m_y--;
		moved = true;
		break;
	case  GenericMenu::Event::KeyDown:
		m_y++;
		moved = true;
		break;
	case GenericMenu::Event::KeyLeft:
		m_x--;
		moved = true;
		break;
	case GenericMenu::Event::KeyRight:
		m_x++;
		moved = true;
		break;

// XXX
#if 0
	case GenericMenu::Event::KeyMiscY: {
		int sq = m_y * jukeW + m_x;
		int ulidx = Unlock::sounds[sq];

		if (game_options.isUnlocked(ulidx) && Unlock::grid[ulidx].artMessage) {
			m_msgbox = new MessageBox(20, m_parent->getFont());
			m_msgbox->setTranslate(Vector(320,240,30));
			m_msgbox->setText(Unlock::grid[ulidx].artMessage, Unlock::grid[ulidx].artMessageCnt);
			subAdd(m_msgbox);
		} else {
			m_parent->sfxInvalid();
		}
		break;
	}
#endif

	}

	if (moved) {
		if (m_x < 0) m_x += jukeW;
		if (m_x >= jukeW) m_x -= jukeW;
		if (m_y < 0) m_y += jukeH;
		if (m_y >= jukeH) m_y -= jukeH;
		m_gs->setSelection(0, m_x, m_y);
		m_parent->sfxCursor();
	}
	updateBuffers();
}
