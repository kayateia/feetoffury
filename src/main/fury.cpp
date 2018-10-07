/*
   Feet of Fury

   fury.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "fury.h"

/* - (C)onfusion is: drunken arrows, vortex, turbo, phasing arrows
- (R)escue is: reprieve(eraser), dispel, reverse
- (H)ealth is: health
- (D)amage is: fireball, bomb */

int Fury::furyAttacks[8][AttackCount] = {
	// C / Yellow
	{ AttackDrunkenArrow, AttackVortex, AttackTurbo, AttackPhasingArrows },
	// R / Green
	{ AttackEraser, AttackDispel, AttackReverse },
	// H / Blue
	{ AttackHealth },
	// D / Red
	{ AttackFireball, AttackBomb },
	// CR / Lime
	{ AttackDrunkenArrow, AttackVortex, AttackTurbo, AttackPhasingArrows,
	  AttackEraser, AttackDispel, AttackReverse },
	// CD / Orange
	{ AttackDrunkenArrow, AttackVortex, AttackTurbo, AttackPhasingArrows,
	  AttackFireball, AttackBomb },
	// RH / Cyan
	{ AttackEraser, AttackDispel, AttackReverse,
	  AttackHealth },
	// DH / Magenta
	{ AttackFireball, AttackBomb,
	  AttackHealth },
};

int Fury::furyAttackCnt[8] = {
	4,
	3,
	1,
	2,
	7,
	6,
	4,
	3
};
