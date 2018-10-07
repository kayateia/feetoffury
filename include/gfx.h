/*      
   Feet of Fury
        
   gfx.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __GFX_H
#define __GFX_H

// main/gfx.cc

extern int hz;
bool gfxInit();
void gfxShutdown();

// Warning! Using either of these will destroy any screen contents,
// PVR buffers, and allocated textures.
bool gfxSwitchTo50();
bool gfxSwitchTo60();

// Check to see if we're on a VGA box
bool gfxIsVga();


#endif	/* __GFX_H */

