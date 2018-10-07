/*
   Feet of Fury

   menu_char/attack_mode_popup.h

   Copyright (c)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __ATTACK_MODE_POPUP_H
#define __ATTACK_MODE_POPUP_H

#include "plrstate.h"
#include "menubox.h"

class AttackModePopup : public Drawable {
public:
	AttackModePopup(int index, Font * fnt, Texture * txr, Sound * curs, Sound * sel,
		PlayerState * pst);
	virtual ~AttackModePopup();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	enum Response {
		RespNone,
		RespSelect,
		RespCancel
	};
	Response processKeypress(GenericMenu::Event::KeyConstant);

	enum Activity {
		ActDisabled,
		ActEnabled,
		ActDecided,
		ActDieing
	};

	void setActive(Activity act);

private:
	void setFadeAnim(float toval);

	RefPtr<Font>		m_font;
	RefPtr<Texture>		m_txr;
	RefPtr<PlayerState>	m_pst;
	RefPtr<Sound>		m_sfx_curs, m_sfx_sel;
	RefPtr<AlphaFader>	m_anim;

	RefPtr<MenuBox>		m_box;

	int			m_frame;
	int			m_index;
	float			m_phase;

	Activity		m_act;
};


#endif
