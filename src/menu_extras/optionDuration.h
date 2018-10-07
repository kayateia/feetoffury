/*
   Feet of Fury

   menu_options/optionDuration.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionDuration.h,v 1.1 2003/04/26 21:33:24 bard Exp $
*/

#include "oldStyleOption.h"

// Class for attack duration settings
class OptDuration : public OldStyleOption {
public:
	OptDuration(MenuExtras * parent) : OldStyleOption(parent) {
		m_name->setText("Attack Length");
		updateValue();
	}

	void updateValue() {
		int sk = game_options.attack_duration;
		
		if (sk == 2)
			m_value->setText("Short");
		else if (sk == 4)
			m_value->setText("Moderate");
		else if (sk == 6)
			m_value->setText("Long");
		else if (sk == 10)
			m_value->setText("Ludicrous");
		else
			m_value->setText("??");
	}

	virtual bool cursorLeft() {
		int sk = game_options.attack_duration;

		if (sk == 2)
			return false;
		else if (sk == 4)
			sk = 2;
		else if (sk == 6)
			sk = 4;
		else if (sk == 10)
			sk = 6;

		game_options.attack_duration = sk;
		game_options.dirty = true;
		
		updateValue();

		return true;
	}
	virtual bool cursorRight() {
		int sk = game_options.attack_duration;

		if (sk == 2)
			sk = 4;
		else if (sk == 4)
			sk = 6;
		else if (sk == 6)
			sk = 10;
		else if (sk == 10)
			return false;

		game_options.attack_duration = sk;
		game_options.dirty = true;
		
		updateValue();

		return true;
	}
};


