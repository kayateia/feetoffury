/*
   Feet of Fury

   menu_options/optionGenericBool.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: optionGenericBool.h,v 1.1 2003/05/03 19:29:44 bard Exp $
*/

#include "oldStyleOption.h"

// Class for mono/stereo settings
class OptGenericBool : public OldStyleOption {
public:
	OptGenericBool(MenuExtras * parent, const char * desc, bool * set) : OldStyleOption(parent) {
		m_set = set;
		m_name->setText(desc);
		updateValue();
	}

	void updateValue() {
		if (*m_set)
			m_value->setText("On");
		else
			m_value->setText("Off");
	}

	virtual bool cursorLeft() {
		*m_set = !*m_set;
		game_options.dirty = true;
		updateValue();
		return true;
	}
	virtual bool cursorRight() {
		return cursorLeft();
	}

	bool	* m_set;
};


