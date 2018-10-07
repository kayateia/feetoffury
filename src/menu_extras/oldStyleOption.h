/*
   Feet of Fury

   menu_extras/oldStyleOption.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: oldStyleOption.h,v 1.2 2003/05/04 00:12:43 bard Exp $
*/

#ifndef __OLD_STYLE_OPTION_H
#define __OLD_STYLE_OPTION_H

#include "options.h"
#include "textOption.h"

// A simple option class that displays a label with the name
// of the option to the left, and a value to the right. Subclass
// to modify behavior and display.
class OldStyleOption : public TextOption {
public:
	OldStyleOption(MenuExtras * parent)
		: TextOption(parent, "", "")
	{
	}
	virtual ~OldStyleOption() {
	}

	virtual bool cursorLeft() { return false; }
	virtual bool cursorRight() { return false; }

	virtual void inputEvent(const GenericMenu::Event & evt) {
		bool snd = false;
		if (evt.key == GenericMenu::Event::KeyLeft) {
			snd = cursorLeft();
		} else if (evt.key == GenericMenu::Event::KeyRight) {
			snd = cursorRight();
		}

		if (snd)
			m_parent->sfxSelect();
		else
			m_parent->sfxInvalid();
	}
};

#endif	// __OLD_STYLE_OPTION_H

