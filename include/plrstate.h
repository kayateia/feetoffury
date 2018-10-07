/*      
   Feet of Fury
        
   plrstate.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __PLRSTATE_H
#define __PLRSTATE_H

#include "stepdata.h"

// The player "state" for each player
class PlayerState : public RefCnt {
public:
	PlayerState();
	virtual ~PlayerState();

	// Returns the length of one second in milliseconds for the
	// requested player (for attack duration timing)
	int secLength() {
		switch (diff) {
		case StepData::Easy:
			return 800;
		case StepData::Medium:
			return 1000;
		case StepData::Hard:
			return 2000;
		}
		
		// Shouldn't be possible, but hey...
		return 1000;
	}

	// Which character has the player selected?
	int	charidx;

	// Character thumbnail (for in-game mug shot)
	RefPtr<Texture>	char_thumb;

	// Full-sized character pic (for win and VS screens)
	RefPtr<Texture> char_full;

	// Innate character sound
	RefPtr<Sound>	char_sound;

	// How many wins does the player have?
	int	wins;

	// Is this player an AI?
	bool	ai;

	// What difficulty level has the player selected?
	StepData::Difficulty	diff;

	// Automated attacks?
	bool	auto_attack;

	// What round is the player playing? (yes, this is duplicated, but...)
	int	round;

	// Fury type selection
	// int	fury;
};

#endif	/* __PLRSTATE_H */

