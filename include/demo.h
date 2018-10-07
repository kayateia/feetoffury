/*      
   Feet of Fury
        
   demo.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __DEMO_H
#define __DEMO_H

void do_demo();

// Lua hooks. Return true if we need to quit

// Put up some text
bool demo_text(const char ** strs, int cnt);

// Sleep for the given number of frames
bool demo_sleep(int framecnt);

// Set the number of textures we'll be loading (in init)
void demo_txr_cnt(int cnt);

// Load a texture (in init)
void demo_txr_load(int slot, const char * fn);

// Fade in / Fade out the PlayerVisuals object
bool demo_pv_fadein();
bool demo_pv_fadeout();

// Start the PV playing
bool demo_pv_start();

// Pause the PV
bool demo_pv_pause();

// Set whether PV arrows will be hit
bool demo_pv_sethit(bool set);

// Switch textures; if fade is true, then a fade effect
// will be used. Otherwise the switch will be immidiate.
bool demo_txr_switch(int idx, bool fade);


#endif	/* __DEMO_H */

