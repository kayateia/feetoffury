/*
   Feet of Fury

   main/misc.cc

   Copyright (C)2003,2005 Cryptic Allusion, LLC
*/

#include "global.h"
#include "pak.h"
#include "stepdata.h"
#include "options.h"

#if TIKI_PLAT == TIKI_WIN32
#include <direct.h>
#endif

bool swapcd_enabled = false;
char swapcd_path[256];

StepData * loadSteps(const char *fn) {
	StepData * sd;

	char tmp[256];
	sprintf(tmp, "song_meta/%s/steps.dwi", fn);
	sd = new StepData(tmp, fn);

	return sd;
}

/////////////////////////////////////////////////////////////////////////////////

int globalSetup() {
	Tiki::init(0, NULL);

	if (!gfxInit())
		return -1;

	/* Print a banner */
	Debug::printf("\n<-- Feet of Fury START ------------------------------------>\n\n");

	srand((unsigned int)Time::gettime());

	// XXX
#if TIKI_PLAT == TIKI_OSX
#elif TIKI_PLAT == TIKI_WIN32
	_chdir("d:\\cygwin\\home\\bard\\prj\\game\\fof\\remix\\data");
#endif

	// Setup OggVorbis thread
	OggVorbis::init();

	// Initialize Lua
	//lua_init();

	return 0;
}

int globalShutdown() {
	// Shut down
	OggVorbis::shutdown();

	// Print a banner
	gfxShutdown();

	Debug::printf("\n<-- Feet of Fury FINISH ----------------------------------->\n\n");

	Tiki::shutdown();

	return 0;
}
