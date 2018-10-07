/*      
   Feet of Fury
        
   unlock.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __UNLOCK_H
#define __UNLOCK_H

#include "character.h"

namespace Unlock {
	// Named unlockable indeces
	enum UnlockableIndex {
		CharMetalJunky = 0,
		CharCharlotte = 5,
		CharVengeance = 12,
		Casino = 30,
		CharPico = 38,
		CharSedary = 46
	};

	static const int count = 49;

	// Map of various in-game indeces to unlock indeces
	extern int chars[CHAR_COUNT];
	extern int sounds[36];

	struct Unlockable {
		Unlockable(const char **txt, int cnt, int p, const char * txrName, const char * txrT = NULL,
			const char **artmsg = NULL, int artmsgcnt = 0)
		{
			text = txt; count = cnt; price = p;
			txr = txrName;
			txrTarget = txrT;
			artMessage = artmsg;
			artMessageCnt = artmsgcnt;
		}

		const char ** text;
		int count;
		int price;
		const char * txr;
		const char * txrTarget;
		const char ** artMessage;
		int artMessageCnt;
	};

	extern Unlockable locked;
	extern Unlockable grid[count];
};

#endif	/* __UNLOCK_H */

