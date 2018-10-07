/*
   Feet of Fury

   ogg.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "ogg.h"
#include "options.h"


// Ogg wrappers (for cache stuff)

int oggStart(const char * fn, bool loop, bool cache, bool queue) {
	// If we're running over dc-load-ip, then automatically cache the
	// song to avoid jerkiness. Otherwise, we'll cache when the
	// client code tells us to (generally during game play).
/* #ifdef RUN_FROM_PC
	if (fn[0] == 'b' && fn[1] == 'g' && fn[2] == 'm' && fn[3] == '/')
		cache = true;
#endif */

	oggVolume(1.0f);

	/* if (queue)
		sndoggvorbis_queue_enable();
	else
		sndoggvorbis_queue_disable(); */
			
	// If we're cacheing, then load into the cache and start from there
	/* if (cache) {
		cacheLoad(fn, "bgm.ogg");
		sndoggvorbis_start(CACHE_PREFIX "bgm.ogg", loop ? 1 : 0);
	} else { */
		OggVorbis::start(fn, loop);
	// }

	return 0;
}

int oggStop(bool cache) {
/* #ifdef RUN_FROM_PC
	cache = true;
#endif */

	OggVorbis::stop();
	/* if (cache)
		cacheUnload("bgm.ogg"); */

	return 0;
}

static float oggVol = 1.0f;

void oggVolume(float v) {
	// Store the _real_ volume for returning to clients
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	oggVol = v;

	// Now adjust the effective volume by the user settings
	// and use that to set the speakers.
	v = v * game_options.vol_music / 100.0f;
	if (v < 0.0f)
		v = 0.0f;
	if (v > 1.0f)
		v = 1.0f;
	OggVorbis::setVolume((int)(v * 255));
}

float oggVolume() { return oggVol; }
