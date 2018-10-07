/*      
   Feet of Fury
        
   fury.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: fury.h,v 1.1 2003/05/03 19:29:44 bard Exp $
*/

#ifndef __FURY_H
#define __FURY_H

#include "gpstate.h"

namespace Fury {
	// For all 8 "furies", keep an array of which attacks
	// are available.
	extern int furyAttacks[8][AttackCount];

	// And for each of those, count the number of attacks.
	extern int furyAttackCnt[8];
};

#endif	/* __FURY_H */

