/*
   Feet of Fury

   menu_options/optionMono.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: optionMono.h,v 1.1 2003/04/26 21:33:24 bard Exp $
*/

#include "oldStyleOption.h"

// Class for mono/stereo settings
class OptMono : public OldStyleOption {
public:
	OptMono(MenuExtras * parent) : OldStyleOption(parent) {
		m_name->setText("Audio Mode");
		updateValue();
	}

	void updateValue() {
		if (game_options.snd_mono == false) {
			// XXX
			//spu_master_mixer(15, 1);
			m_value->setText("Stereo");
		} else {
			// XXX
			//spu_master_mixer(15, 0);
			m_value->setText("Mono");
		}
	}

	virtual bool cursorLeft() {
		game_options.snd_mono = !game_options.snd_mono;
		game_options.dirty = true;
		updateValue();
		return true;
	}
	virtual bool cursorRight() {
		return cursorLeft();
	}
};


