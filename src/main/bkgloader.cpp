/*
   Feet of Fury

   bkgLoader.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "pak.h"

/* static void loadInitialRomdisks() {
	pakMount("fonts", "gfx/fonts");
	pakMount("gameplay", "gfx/gameplay");
	pakMount("gfx_misc", "gfx/misc");
	pakMount("menu_char", "gfx/menu_char");
	pakMount("menu_stage", "gfx/menu_stage");
	pakMount("sfx_misc", "sfx");
}

static volatile bool thdProcFinished = false;

static void thdProc(void * p) {
	printf("bkgLoader thdProc: started loading\n");
	loadInitialRomdisks();
	thdProcFinished = true;
	printf("bkgLoader thdProc: finished loading\n");
}

void bkgLoaderBegin() {
	thdProcFinished = false;
	void * t = thd_create(thdProc, NULL);
	assert( t != NULL );
}

void bkgLoaderWait() {
	while (!thdProcFinished)
		thd_pass();
}

bool bkgLoaderDone() {
	return thdProcFinished;
} */
