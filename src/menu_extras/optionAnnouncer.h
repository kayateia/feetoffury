/*
   Feet of Fury

   menu_options/optionAnnouncer.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionAnnouncer.h,v 1.1 2003/04/26 21:33:23 bard Exp $
*/

#include "oldStyleOption.h"

// Class for announcer on/off settings
class OptAnnouncer : public OldStyleOption {
public:
	OptAnnouncer(MenuExtras * parent) : OldStyleOption(parent) {
		m_name->setText("Announcer");
		updateValue();
	}

	void updateValue() {
		if (game_options.announcer == false)
			m_value->setText("Off");
		else
			m_value->setText("On");
	}

	virtual bool cursorLeft() {
		game_options.announcer = !game_options.announcer;
		game_options.dirty = true;
		updateValue();
		return true;
	}
	virtual bool cursorRight() {
		return cursorLeft();
	}
};


