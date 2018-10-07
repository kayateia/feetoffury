/*
   Feet of Fury

   global.h

   Copyright (C)2002, 2005 Cryptic Allusion, LLC
*/

#ifndef __GLOBAL_H
#define __GLOBAL_H

// I18N stuff
#define _(String) String
#define N_(String) String

#include "platglobal.h"

#include <Tiki/tiki.h>

// Tiki
#include <Tiki/TikiAll.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::Math;
using namespace Tiki::Audio;
using namespace Tiki::GL::Plxcompat;

// Some STL stuff we'll use periodically.
#include <vector>
#include <list>

// ANSI C stuff
#include <stdlib.h>
#include <math.h>

// This is used in a lot of places thanks to 'hz'
#include "gfx.h"

// Ditto on this for swapcd_enabled
#include "main.h"

static inline int randnum(int lim) {
	return rand() % lim;
}

#endif	/* __GLOBAL_H */
