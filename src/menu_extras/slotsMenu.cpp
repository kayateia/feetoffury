/*
   Feet of Fury

   menu_extras/slotsMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"
#include "menu_extras.h"
#include "options.h"

#include "slotsMenu.h"
#include "multiOptionMenu.h"

#include "textdisplay.h"
#include "menubox.h"
#include "scrollybg.h"

Cylinder::Cylinder(Texture * txr, Sound * click) {
	m_txr = txr;
	// m_offs = randnum(100) / 100.0f;
	m_offs = 0.0f;
	m_speed = m_speedTo = 0.0f;
	m_accel = 0.0f;
	m_stopAt = 0;
	m_stop = false;
	m_click = click;
	m_frame = 0;
}

Cylinder::~Cylinder() {
}

void Cylinder::draw(ObjType list) {
	Drawable::draw(list);

	if (list != Opaque)
		return;

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	Vector p = getPosition();
	Color t = getColor();

	// Get our texture
	plx_cxt_texture(m_txr);
	plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(list);

	static const float vs[] = {
		-42.5f/512.0f,
		-29.0f/512.0f,
		-16.5f/512.0f,
		-7.0f/512.0f,
		0.0f,
		+7.0f/512.0f,
		+16.5f/512.0f,
		+29.0f/512.0f,
		+42.5f/512.0f
	};
	static const float ys[] = {
		-32,
		-24,
		-14,
		-6,
		0,
		6,
		14,
		24,
		32
	};
	static const float tint[] = {
		0.60f,
		0.80f,
		0.85f,
		0.80f,
		0.75f,
		0.70f,
		0.60f,
		0.55f,
		0.4f,
	};

	for (int i=0; i<9; i++) {
		Color c = t;
		c.r *= tint[i];
		c.g *= tint[i];
		c.b *= tint[i];
		plx_vert_ifd(&dr, PLX_VERT,
			p.x-32, p.y+ys[i], p.z,
			c,
			0.0f, vs[i] + m_offs + 85/(2*512.0f));
		plx_vert_ifd(&dr, i == 8 ? PLX_VERT_EOS : PLX_VERT,
			p.x+32, p.y+ys[i], p.z,
			c,
			1.0f, vs[i] + m_offs + 85/(2*512.0f));
	}

	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(list);

	uint32 w = Color(1,0,0,0);
	plx_vert_ind(&dr, PLX_VERT,     p.x-34, p.y-1, p.z+1, w);
	plx_vert_ind(&dr, PLX_VERT,     p.x+34, p.y-1, p.z+1, w);
	plx_vert_ind(&dr, PLX_VERT,     p.x-34, p.y+1, p.z+1, w);
	plx_vert_ind(&dr, PLX_VERT_EOS, p.x+34, p.y+1, p.z+1, w);
	
}

void Cylinder::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	if (m_stop) {
		float d = m_offs - m_stopAt;
		if (d < 0) d = -d;
		if (d < 2*m_speed) {
			if (m_speed < 2.0f/512.0f) {
				m_offs = m_stopAt;
				m_stop = false;
				m_speed = 0.0f;
				m_speedTo = 0.0f;
				m_accel = 0.0f;
			} else {
				m_speedTo = m_speed / 4.0f;
				m_accel = (m_speedTo - m_speed) / hz;
			}
		}
	}

	m_speed += m_accel;
	if (m_speed < m_speedTo) {
		m_speed = m_speedTo;
	}
	m_offs += m_speed;
	if (m_offs >= 1.0f)
		m_offs -= (int)m_offs;

	if (m_click) {
		int p = hz;
		if (m_speed >= 16.0f/512.0f) {
			p = 5;
		} else if (m_speed >= 5.0f/512.0f) {
			p = 6;
		} else if (m_speed >= 1.0f/512.0f) {
			p = 7;
		}
		if (p != hz && !(m_frame % p)) {
			m_click->play();
		}
	}

	m_frame++;
}

void Cylinder::stop() {
	m_offs = 0.0f;
	m_speed = 0.0f;
	m_speedTo = 0.0f;
	m_accel = 0.0f;
	m_stop = false;
}


Slot::Slot(SlotsMenu * pr, Texture * txr) {
	m_parent = pr;

	MenuBox * mb = new MenuBox(true);
	mb->setScale(Vector(3*64 + 16, 1*64, 1));
	subAdd(mb);

	m_cyl[0] = new Cylinder(txr, NULL);
	m_cyl[0]->setTranslate(Vector(-70,0,1));
	mb->subAdd(m_cyl[0]);
	m_cyl[1] = new Cylinder(txr, NULL);
	m_cyl[1]->setTranslate(Vector(0,0,1));
	mb->subAdd(m_cyl[1]);
	m_cyl[2] = new Cylinder(txr, res.getSound(Res::Slots, "click"));
	m_cyl[2]->setTranslate(Vector(+70,0,1));
	mb->subAdd(m_cyl[2]);

	m_stop = false;
	m_cylStopped[0] = m_cylStopped[1] = m_cylStopped[2] = true;
}

Slot::~Slot() {
}

void Slot::go() {
	// Are they gonna win? (ahh fate is lovely :)
	int win = randnum(100);
	if (win < 25) {
		m_stopAt[0] = randnum(6);
		m_stopAt[1] = m_stopAt[0];
		m_stopAt[2] = m_stopAt[0];
	} else {
		// They still might win.. just very unlikely ;)
		m_stopAt[0] = randnum(6);
		m_stopAt[1] = randnum(6);
		m_stopAt[2] = randnum(6);
	}
	m_stop = true;
	m_stopped = false;

	m_cyl[0]->m_speed = 16.0f/512.0f;
	m_cyl[1]->m_speed = 16.0f/512.0f;
	m_cyl[2]->m_speed = 16.0f/512.0f;
	m_cyl[0]->m_stop = true;
	m_cyl[0]->m_stopAt = 85.0f * m_stopAt[0] / 512.0f;
	m_cyl[1]->m_stop = m_cyl[2]->m_stop = false;

	m_cylStopped[0] = m_cylStopped[1] = m_cylStopped[2] = false;
}

void Slot::stopAll() {
	m_cyl[0]->stop();
	m_cyl[1]->stop();
	m_cyl[2]->stop();
	m_stop = false;
	m_cylStopped[0] = m_cylStopped[1] = m_cylStopped[2] = true;
}

bool Slot::isStopped() {
	return m_cylStopped[0] && m_cylStopped[1] && m_cylStopped[2];
}

void Slot::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	if (!m_stop) return;

	if (m_cyl[0]->m_stop == false) {
		if (!m_cylStopped[0]) {
			m_cylStopped[0] = true;
			m_cyl[1]->m_stopAt = 85.0f * m_stopAt[1] / 512.0f;
			m_cyl[1]->m_stop = true;
		}

		if (m_cyl[1]->m_stop == false) {
			if (!m_cylStopped[1]) {
				m_cylStopped[1] = true;
				m_cyl[2]->m_stopAt = 85.0f * m_stopAt[2] / 512.0f;
				m_cyl[2]->m_stop = true;
			}

			if (m_cyl[2]->m_stop == false && !m_cylStopped[2]) {
				m_cylStopped[2] = true;
				m_stopped = true;
				m_stop = false;
				m_parent->stopped();
			}
		}
	}
}


static const char *txt_help[] = {
	"Press A or ENTER to bet 50 unlock points.",
	"If a favorable combination is under the win",
	"line when the cylinders stop, you win more",
	"unlock points! Press B to exit."
};

SlotsMenu::SlotsMenu(MenuExtras * parent, ExtrasMenu * back)
	: ExtrasMenu(parent)
{
	m_back = back;

	m_txrSlots = res.getTexture(Res::Slots, "slots");
	m_sfxFail = res.getSound(Res::Slots, "fail");
	m_sfxSucceed = res.getSound(Res::Unlock, "unlock");

	m_title = new Label(m_parent->getFont(), "Casino of Fury", 36, true, false);
	m_title->setTranslate(Vector(320.0f, 60.0f, 15.0f));
	subAdd(m_title);

	m_creditsLbl = new Label(m_parent->getFont(), "Credits", 24, false, false);
	m_creditsLbl->setTranslate(Vector(120.0f, 230.0f, 15.0f));
	m_creditsLbl->setTint(Color(0.85f,0.85f,0));
	subAdd(m_creditsLbl);

	m_creditsBuf[0] = 0;
	m_credits = new Label(m_parent->getFont(), m_creditsBuf, 24, false, false);
	m_credits->setTranslate(Vector(320.0f, 230.0f, 15.0f));
	m_credits->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_credits);

	m_winLbl = new Label(m_parent->getFont(), "Last Result:", 24, false, false);
	m_winLbl->setTranslate(Vector(120.0f, 260.0f, 15.0f));
	m_winLbl->setTint(Color(0.85f,0.85f,0));
	subAdd(m_winLbl);

	strcpy(m_winBuf, "None");
	m_win = new Label(m_parent->getFont(), m_winBuf, 24, false, false);
	m_win->setTranslate(Vector(320.0f, 260.0f, 15.0f));
	m_win->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_win);

	m_td = new TextDisplay(m_parent->getFont(), 20, true);
	m_td->setTranslate(Vector(320.0f, 370.0f, 15.0f));
	m_td->setText(txt_help, sizeof(txt_help)/sizeof(txt_help[0]));
	m_td->setBGAlpha(0.0f);
	subAdd(m_td);

	m_slot = new Slot(this, m_txrSlots);
	m_slot->setTranslate(Vector(320,160,10));
	subAdd(m_slot);

	updateBuffers();

	m_dirty = false;
}

SlotsMenu::~SlotsMenu() {
}

void SlotsMenu::updateBuffers() {
	sprintf(m_creditsBuf, "%lu", game_options.unlock_pts);
}

void SlotsMenu::gainFocus() {
	updateBuffers();
}

void SlotsMenu::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

	switch (evt.key) {
	case GenericMenu::Event::KeySelect:
		if (m_slot->isStopped()) {
			game_options.unlock_pts -= 50;
			game_options.dirty = true;
			m_dirty = true;
			updateBuffers();
			m_slot->go();
		}
		break;
	case GenericMenu::Event::KeyCancel:
		m_slot->stopAll();
		if (m_dirty) {
			m_dirty = false;
			m_parent->rescanUnlock();
		}
		m_parent->setSubMenu(m_back, false);
		return;
	}
}

void SlotsMenu::stopped() {
	if (m_slot->m_stopAt[0] == m_slot->m_stopAt[1] && m_slot->m_stopAt[0] == m_slot->m_stopAt[2]) {
		m_sfxSucceed->play();
		switch (m_slot->m_stopAt[0]) {
		case 0:
			game_options.unlock_pts += 500;
			strcpy(m_winBuf, "Won 500 points!");
			break;
		case 1:
			game_options.unlock_pts += 1000;
			strcpy(m_winBuf, "Won 1000 points!");
			break;
		default:
			game_options.unlock_pts += 250;
			strcpy(m_winBuf, "Won 250 points!");
			break;
		}
		m_dirty = true;
		updateBuffers();
	} else {
		strcpy(m_winBuf, "Lost");
		m_sfxFail->play();
	}
}
