/*
   Feet of Fury

   menu_options/optionAiSkill.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionAiSkill.h,v 1.1 2003/04/26 21:33:23 bard Exp $
*/

#include "oldStyleOption.h"

// Class for AI skill settings
class OptAISkill : public OldStyleOption {
public:
	OptAISkill(MenuExtras * parent) : OldStyleOption(parent) {
		m_name->setText("AI Skill");
		updateValue();
	}

	void updateValue() {
		int sk = game_options.ai_skill;
		
		if (sk == 80)
			m_value->setText("Wimpy");
		else if (sk == 85)
			m_value->setText("Moderate");
		else if (sk == 90)
			m_value->setText("Really Good");
		else if (sk == 95)
			m_value->setText("Ludicrous");
		else
			m_value->setText("??");
	}

	virtual bool cursorLeft() {
		int sk = game_options.ai_skill;

		if (sk == 80)
			return false;
		else if (sk == 85)
			sk = 80;
		else if (sk == 90)
			sk = 85;
		else if (sk == 95)
			sk = 90;

		game_options.ai_skill = sk;
		game_options.dirty = true;
		
		updateValue();

		return true;
	}
	virtual bool cursorRight() {
		int sk = game_options.ai_skill;

		if (sk == 80)
			sk = 85;
		else if (sk == 85)
			sk = 90;
		else if (sk == 90)
			sk = 95;
		else if (sk == 95)
			return false;

		game_options.ai_skill = sk;
		game_options.dirty = true;
		
		updateValue();

		return true;
	}
};

