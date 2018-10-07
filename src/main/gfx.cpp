/*
   Feet of Fury

   gfx.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"

int hz = 60;

bool gfxSwitchTo50() {
	return true;
}

bool gfxSwitchTo60() {
	return true;
}

bool gfxIsVga() {
	return true;
}

bool gfxInit() {
	// gfxSwitchTo50();

	/* Initialize the Parallax as well */
	plx_mat3d_mode(PLX_MAT_PROJECTION);
	plx_mat3d_identity();
	plx_mat3d_perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	plx_mat3d_mode(PLX_MAT_MODELVIEW);

	return true;
}

void gfxShutdown() {
#if 0
	pvr_stats_t	stats;

	pvr_get_stats(&stats);
	dbglog(DBG_DEBUG, "3D Stats: %ld vblanks, frame rate ~%f fps, max vertex used %d bytes\n",
		stats.vbl_count, (double)stats.frame_rate, stats.vtx_buffer_used_max);
#endif
}

