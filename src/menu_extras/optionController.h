/*
   Feet of Fury

   menu_options/optionController.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionController.h,v 1.1 2003/04/26 21:33:23 bard Exp $
*/

#include "oldStyleOption.h"

// Class for controller mode
class OptController : public OldStyleOption {
public:
	OptController(MenuExtras * parent, int idx) : OldStyleOption(parent) {
		if (idx == 0)
			m_name->setText("P1 Control");
		else
			m_name->setText("P2 Control");

		m_idx = idx;
		updateValue();
	}

	void updateValue() {
		if (game_options.cont_mode[m_idx] == 0)
			m_value->setText("JoyPad");
		else
			m_value->setText("Floor Mat");
	}

	virtual bool cursorLeft() {
		game_options.cont_mode[m_idx] ^= 1;
		game_options.dirty = true;
		updateValue();
		return true;
	}
	virtual bool cursorRight() {
		return cursorLeft();
	}

private:
	int	m_idx;
};


