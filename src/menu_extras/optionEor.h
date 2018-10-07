/*
   Feet of Fury

   menu_options/optionEor.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionEor.h,v 1.1 2003/04/26 21:33:24 bard Exp $
*/

#include "oldStyleOption.h"

// Class for End of Round settings
class OptEOR : public OldStyleOption {
public:
	OptEOR(MenuExtras * parent) : OldStyleOption(parent) {
		m_name->setText("End of Round");
		updateValue();
	}

	void updateValue() {
		if (game_options.eor_style == 0)
			m_value->setText("Out of Health");
		else
			m_value->setText("End of Song");
	}

	virtual bool cursorLeft() {
		game_options.eor_style ^= 1;
		game_options.dirty = true;
		updateValue();
		return true;
	}
	virtual bool cursorRight() {
		return cursorLeft();
	}
};

