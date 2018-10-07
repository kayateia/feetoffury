/*      
   Feet of Fury
        
   character.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: character.h,v 1.7 2003/05/09 00:07:18 bard Exp $
*/

#ifndef __CHARACTER_H
#define __CHARACTER_H

#include "gpstate.h"

// All the info on an individual character
struct Character {
	Character(const char * n, bool en, const char * scr,
		  int gx, int gy,
		  const char ** wt, int wtc,
		  const char * tn, Attack tt,
		  const char * tl, const char * ttc, const char * ttg,
		  const char * tb,
		  const char * wav)
		: name(n), enabled(en), scroller(scr),
		  grid_x(gx), grid_y(gy),
		  win_text(wt), win_text_cnt(wtc),
		  tech_name(tn), tech_type(tt),
		  txr_large(tl), txr_thumb_color(ttc), txr_thumb_gray(ttg),
		  txr_banner(tb), sound_fn(wav) { }

	// Character name
	const char * name;

	// Is it enabled?
	bool enabled;

	// Scroller text about character
	const char * scroller;

	// Grid position
	int grid_x, grid_y;

	// Winning text message
	const char ** win_text;
	int win_text_cnt;

	// Technique
	const char * tech_name;
	Attack tech_type;

	// Texture names
	const char * txr_large;
	const char * txr_thumb_color;
	const char * txr_thumb_gray;
	const char * txr_banner;

	// Filename of their sound effect
	const char * sound_fn;
};

#define CHAR_COUNT 10

// A static struct containing a list of all the characters
// and the number of characters, etc.
struct Characters {
	// Number of characters
	static const int count;

	// Character info
	static const Character info[];

	// Character select grid size
	static const int grid_w, grid_h;

	// Scroll text for locked character
	static const char * locked;

	// Locate a character profile by grid x,y position
	static int charAt(int w, int h);

	// Choose a random character
	static int pickRandom();
};

#endif	/* __CHARACTER_H */

