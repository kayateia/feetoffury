/*
   Feet of Fury

   menu_options/optionVolume.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionVolume.h,v 1.2 2003/05/04 00:12:43 bard Exp $
*/

#include "oldStyleOption.h"
#include "ogg.h"

// Class for volume settings
class OptVolume : public OldStyleOption {
public:
	OptVolume(MenuExtras * parent, int knob) : OldStyleOption(parent) {
		m_idx = knob;
		switch (m_idx) {
		case 0:
			m_name->setText("Music Volume");
			m_valptr = &game_options.vol_music;
			break;
		case 1:
			m_name->setText("Effect Volume");
			m_valptr = &game_options.vol_sounds;
			break;
		default:
			assert( false );
		}

		updateValue();
	}

	void updateValue() {
		const char * values[] = {
			"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"
		};

		m_value->setText(values[*m_valptr / 10]);

		switch (m_idx) {
		case 0:
			oggVolume(1.0f);
			break;
		case 1:
			Sound::setDefaultVolume(*m_valptr * 255 / 100);
			break;
		}
	}

	virtual bool cursorLeft() {
		if (*m_valptr <= 0)
			return false;
			
		*m_valptr -= 10;
		game_options.dirty = true;
		updateValue();

		return true;
	}
	virtual bool cursorRight() {
		if (*m_valptr >= 100)
			return false;

		*m_valptr += 10;
		game_options.dirty = true;
		updateValue();

		return true;
	}

private:
	int	m_idx;
	int	* m_valptr;
};

