/*
   Feet of Fury

   menu_char/attack_mode_popup.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "plrstate.h"
#include "hsv.h"

#include "attack_mode_popup.h"

#include "menubox.h"

AttackModePopup::AttackModePopup(int index, Font * fnt, Texture * txr,
	Sound * curs, Sound * sel, PlayerState * pst)
{
	m_font = fnt;
	m_txr = txr;
	m_sfx_curs = curs;
	m_sfx_sel = sel;
	m_pst = pst;
	m_frame = 0; m_phase = 0.0f;
	m_index = index;
	m_act = ActDisabled; setTint(Color(0.0f,1,1,1));
	m_box = new MenuBox(true);
	m_box->setTranslate(Vector(0,0,-1.0f));
	subAdd(m_box);
}

AttackModePopup::~AttackModePopup() {
}

void AttackModePopup::draw(ObjType list) {
	if (list != Trans || getColor().a == 0.0f)
		return;

	// Get our position
	Vector pos = getPosition();

	plx_dr_state_t dr; plx_dr_init(&dr);

	float w = 240.0f, h = 200.0f;

	// Draw a nice background poly
	m_box->setScale(Vector(w, h, 1.0f));
	// m_box->draw(list);

	m_txr->select();

	Color tint = getColor();

	// Draw the header with normal colors
	pos.y -= h/4.0f;
	uint32 color = tint;
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y + 32.0f, pos.z, color, 0.0f, 0.25f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y - 32.0f, pos.z, color, 0.0f, 0.00f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x + 128.0f, pos.y + 32.0f, pos.z, color, 1.0f, 0.25f);
	plx_vert_ifd(&dr, PLX_VERT_EOS, pos.x + 128.0f, pos.y - 32.0f, pos.z, color, 1.0f, 0.00f);

	// Draw the two options
	pos.y += h/4.0f;
	float fade = fsin(m_frame * 2*M_PI / hz) * 0.2f + 0.4f;
	if (m_pst->auto_attack) {
		if (m_index == 0)
			color = tint * Color(1.0f, fade, fade);
		else
			color = tint * Color(fade, 1.0f, fade);
	} else
		color = tint * Color(0.5f, 0.5f, 0.5f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y + 32.0f, pos.z, color, 0.0f, 0.50f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y - 32.0f, pos.z, color, 0.0f, 0.25f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x + 128.0f, pos.y + 32.0f, pos.z, color, 1.0f, 0.50f);
	plx_vert_ifd(&dr, PLX_VERT_EOS, pos.x + 128.0f, pos.y - 32.0f, pos.z, color, 1.0f, 0.25f);

	pos.y += h/4.0f;
	if (!m_pst->auto_attack) {
		if (m_index == 0)
			color = tint * Color(1.0f, fade, fade);
		else
			color = tint * Color(fade, 1.0f, fade);
	} else
		color = tint * Color(0.5f, 0.5f, 0.5f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y + 32.0f, pos.z, color, 0.0f, 0.75f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x - 128.0f, pos.y - 32.0f, pos.z, color, 0.0f, 0.50f);
	plx_vert_ifd(&dr, PLX_VERT, pos.x + 128.0f, pos.y + 32.0f, pos.z, color, 1.0f, 0.75f);
	plx_vert_ifd(&dr, PLX_VERT_EOS, pos.x + 128.0f, pos.y - 32.0f, pos.z, color, 1.0f, 0.50f);
}

void AttackModePopup::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	if (m_act == 1) {
		m_frame++;

		m_phase += 1.0f/480.0f;
		if (m_phase >= 1.0f)
			m_phase -= 1.0f;

		// m_box->nextFrame();
	}
}

AttackModePopup::Response AttackModePopup::processKeypress(GenericMenu::Event::KeyConstant key) {
	if (m_act == ActDieing) return RespNone;

	switch (key) {

	case GenericMenu::Event::KeyCancel:
		//m_sfx_curs->play();
		return RespCancel;

	case GenericMenu::Event::KeyUp:
	case GenericMenu::Event::KeyDown:
		if (m_act != ActEnabled) return RespNone;
		m_sfx_curs->play();
		m_pst->auto_attack = !m_pst->auto_attack;
		return RespNone;

	case GenericMenu::Event::KeySelect:
		m_sfx_sel->play();
		return RespSelect;
	}

	return RespNone;
}

void AttackModePopup::setFadeAnim(float toval) {
	if (m_anim)
		animRemove(m_anim);

	float a = getTint().a;
	
	m_anim = new AlphaFader(toval, (toval - a)/15.0f);
	animAdd(m_anim);
}

void AttackModePopup::setActive(AttackModePopup::Activity act) {
	if (m_act != act) {
		switch (act) {
		case ActDieing:
			if (m_anim)
				animRemove(m_anim);
			m_anim = NULL;
			break;
		case ActDisabled:
			setFadeAnim(0.0f);
			break;
		case ActEnabled:
			setFadeAnim(1.0f);
			break;
		case ActDecided:
			setFadeAnim(0.7f);
			break;
		default: assert(false); return;
		}
	}
	
	m_act = act;
}


