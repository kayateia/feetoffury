/*      
   Feet of Fury
        
   songs.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: songs.h,v 1.1 2003/05/03 19:29:44 bard Exp $
*/

#ifndef __SONGS_H
#define __SONGS_H

#include "gpstate.h"

namespace Songs {
	// The number of songs available in standard, non-SwapCD mode.
	extern int count;

	// For each song, its filename on disk (under song_meta).
	extern char * names[32];

	// And then for each song, -1 if it's always unlocked; otherwise
	// the index of the unlock slot for it.
	extern int unlock[32];
};

#endif	/* __SONGS_H */

