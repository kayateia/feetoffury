/*
   Feet of Fury

   menu_options/optionMorePoints.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionMorePoints.h,v 1.2 2003/04/28 02:43:11 bard Exp $
*/

#include "textOption.h"

class OptMorePoints : public TextOption {
public:
	OptMorePoints(MenuExtras * parent) : TextOption(parent, "", "") {
		m_name->setText("Gimme unlock points! (dbg)");
		m_value->setText("");
	}

	virtual void inputEvent(const GenericMenu::Event & evt) {
		if (evt.key == GenericMenu::Event::KeySelect) {
			game_options.unlock_pts += 1000;
			game_options.dirty = true;
			m_parent->sfxSelect();
			m_parent->rescanUnlock();
		}
	}
};


