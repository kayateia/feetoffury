/*      
   Feet of Fury
        
   options.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include "character.h"

#include "scrollybg.h"

class Options {
public:
	Options();
	virtual ~Options();

	// Call to save the options. This should be done in between screens
	// since it will take over the 3D output for a bit. The oldbg param
	// is for segueing from the initial "do you want a save" screen.
	bool	save(ScrollyBG * oldbg = NULL);

	// Look for an FoF system file and load it
	bool	load();

	// Check to see if a usable VMU is present. If one is found, its
	// name will be written in two chars at outname.
	// bool	saveableVmuPresent(char * outname, maple_device_t ** dev);

	// True if we should try to autosave (e.g., this may be false
	// if the user doesn't have a save file already and declines one
	// at boot time).
	bool	auto_save;

	// Controller mode: joypad == 0, mat = 1
	int	cont_mode[2];

	// Volume settings 0..100
	int	vol_music, vol_sounds;

	// Announcer voice on/off
	bool	announcer;

	// Mono == true, stereo = false
	bool	snd_mono;

	// Computer's playing skill 0..100
	int	ai_skill;

	// End of round style: 0 == out of health, 1 == end of round
	int	eor_style;

	// Timed-attack duration in seconds (1..10)
	int	attack_duration;

	// Unlocked characters
	bool	char_unlocked[CHAR_COUNT];

	// Total number of unlock points accumulated
	uint32	unlock_pts;

	// Other unlockables. Note that although there are dups
	// between this grid and the char_unlocked array, the char_unlocked
	// array is still the definitive source for unlocked character
	// info (though these should match!!). This thing is a bitmask,
	// so use the methods below to tinker with it. The indeces for
	// these are in unlock.h.
	uint32	unlocked[64 / 32];

	bool isLocked(int idx);
	bool isUnlocked(int idx);
	void unlock(int idx);

	// Song played flags. These bits get set when the matching
	// song/audio file is played in either the jukebox menu or
	// during gameplay.
	uint32	song_played[64 / 32];

	bool isSongPlayed(int idx);
	void setSongPlayed(int idx);

	// Practice mode options
	bool	practiceBG;
	bool	practiceBeatBars;
	bool	practiceClapTrack;

	// Button settings
	enum ButtonSetting {
		Up1 = 0, Up2,
		Left1, Left2,
		Right1, Right2,
		Down1, Down2,
		UseItem,
		Count
	};
	uint32	buttonSettings[Count];

	// Do we allow the system menu during gameplay?
	bool	allowSystemMenu;

	// Set to true if you modify anything in here
	bool	dirty;

	// Set to the version code of the file we loaded at startup
	uint32	version;
};

// Exactly one of these exists in main/options.cc
extern Options game_options;

// Synthetic button masks for LTRIG/RTRIG so we can set those as
// button settings.
#define CONT_LTRIG	(1<<29)
#define CONT_RTRIG	(1<<30)

#endif	/* __OPTIONS_H */

