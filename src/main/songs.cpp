/*
   Feet of Fury

   songs.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "songs.h"

int Songs::count = 22;

char * Songs::names[32] = {
	"omm_allusions",
	"djg_bhedu",
	"kud_mitm",
	"omm_house",
	"djg_bufflog",
	"kud_menu",
	"omm_pyro",
	"djg_bufflsr",
	"kud_dreams2",
	"omm_ramp",
	"djg_gotnoclue",
	"omm_run",
	"djg_higher",
	"cho_catchup",
	"djg_noodlesln",
	"cho_laptop",
	"djg_tisonits",
	"cho_quickly",
	"djg_trickster",
	"kud_nrg",
	"djg_loungin",
	"kud_watoo"
};

int Songs::unlock[32] = {
	-1,	// allusions
	18,	// bhedu
	-1,	// mitm
	33,	// house
	-1,	// bufflog
	26,	// menu
	-1,	// pyro
	24,	// bufflsr
	2,	// dreams2
	14,	// ramp
	-1,	// gotnoclue
	-1,	// run
	-1,	// higher
	-1,	// catchup
	29,	// noodlesln
	-1,	// laptop
	-1,	// tisonits
	-1,	// quickly
	-1,	// trickster
	-1,	// nrg
	-1,	// loungin'
	-1,	// watoo
};

