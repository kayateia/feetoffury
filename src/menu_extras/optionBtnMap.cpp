/*
   Feet of Fury

   menu_extras/optionBtnMap.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_extras.h"
#include "options.h"

#include "extrasMenu.h"
#include "optionBtnMap.h"
#include "multiOptionMenu.h"


HitBtnMenu::HitBtnMenu(OptBtnMap * parent)
	: ExtrasMenu(parent->getParent())
{
	m_parent = parent;

	Label * lbl = new Label(parent->getParent()->getFont(), "Press the desired key", 26, true, true);
	lbl->setTranslate(Vector(320,240 - 26/2, 15.0));
	subAdd(lbl);

	lbl = new Label(parent->getParent()->getFont(), "on any control pad.", 26, true, true);
	lbl->setTranslate(Vector(320,240 + 26/2, 15.0));
	subAdd(lbl);
}

HitBtnMenu::~HitBtnMenu() {
}

void HitBtnMenu::inputEvent(const GenericMenu::Event & evt) {
	// Not a keypress -> ignore
	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

// XXX
#if 0
	// Not a controller -> ignore
	if (evt.ptype != GenericMenu::Event::TypeController) {
		m_parent->getParent()->sfxInvalid();
		return;
	}

	cont_state_t * st = (cont_state_t *)evt.rawState;

	// Look for ltrig and rtrig first.
	uint32 & poo = *(m_parent->m_setting);
	bool set = false;
	if (st->ltrig > 0) {
		poo = CONT_LTRIG;
		set = true;
	} else if (st->rtrig > 0) {
		poo = CONT_RTRIG;
		set = true;
	} else if (st->buttons & CONT_A) {
		poo = CONT_A;
		set = true;
	} else if (st->buttons & CONT_B) {
		poo = CONT_B;
		set = true;
	} else if (st->buttons & CONT_X) {
		poo = CONT_X;
		set = true;
	} else if (st->buttons & CONT_Y) {
		poo = CONT_Y;
		set = true;
	} else if (st->buttons & CONT_DPAD_UP) {
		poo = CONT_DPAD_UP;
		set = true;
	} else if (st->buttons & CONT_DPAD_DOWN) {
		poo = CONT_DPAD_DOWN;
		set = true;
	} else if (st->buttons & CONT_DPAD_LEFT) {
		poo = CONT_DPAD_LEFT;
		set = true;
	} else if (st->buttons & CONT_DPAD_RIGHT) {
		poo = CONT_DPAD_RIGHT;
		set = true;
	}
	if (!set)
		m_parent->getParent()->sfxInvalid();
	else
		m_parent->finished();
#endif
}




OptBtnMap::OptBtnMap(MenuExtras * parent, ExtrasMenu * mom, const char * desc, uint32 * setting)
	: TextOption(parent, desc, "")
{
	m_mom = mom;
	m_setting = setting;
	updateBuffers();
}

OptBtnMap::~OptBtnMap() {
}

void OptBtnMap::inputEvent(const GenericMenu::Event & evt) {
	if (evt.key == GenericMenu::Event::KeySelect) {
		// Ack it 
		m_parent->sfxSelect();

		// Create a submenu
		m_submenu = new HitBtnMenu(this);

		// Switch to it
		m_parent->setSubMenu(m_submenu, true, false);
	}
}

void OptBtnMap::updateBuffers() {
// XXX
#if 0
	// Update our text label
	switch (*m_setting) {
	case CONT_A:
		m_value->setText("A"); break;
	case CONT_B:
		m_value->setText("B"); break;
	case CONT_X:
		m_value->setText("X"); break;
	case CONT_Y:
		m_value->setText("Y"); break;
	case CONT_DPAD_UP:
		m_value->setText("DPad Up"); break;
	case CONT_DPAD_DOWN:
		m_value->setText("DPad Down"); break;
	case CONT_DPAD_LEFT:
		m_value->setText("DPad Left"); break;
	case CONT_DPAD_RIGHT:
		m_value->setText("DPad Right"); break;
	case CONT_LTRIG:
		m_value->setText("Left Trigger"); break;
	case CONT_RTRIG:
		m_value->setText("Right Trigger"); break;
	default:
		m_value->setText("???"); break;
	}
#endif
}

// Cheat and overload this a bit
void OptBtnMap::rescanUnlock() {
	updateBuffers();
}

void OptBtnMap::finished() {
	// Acknowledge it.
	m_parent->sfxSelect();

	// Get rid of our little sub-menu.
	m_parent->setSubMenu(m_mom, false, false);


	// Look for other keys that are the same and fix them.
	for (int i=0; i<Options::Count; i++) {
		if (game_options.buttonSettings[i] == *m_setting) {
			if (&game_options.buttonSettings[i] != m_setting)
				game_options.buttonSettings[i] = 0;
		}
	}
	m_mom->rescanUnlock();

	game_options.dirty = true;
}

MenuExtras * OptBtnMap::getParent() const { return m_parent; }
