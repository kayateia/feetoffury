/*
   Feet of Fury

   gameplay/plrstate.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "plrstate.h"

// Constructor / Destructor
PlayerState::PlayerState() {
	charidx = -1;
	wins = 0;
	ai = false;
	diff = StepData::Invalid;
	round = 0;
	auto_attack = true;
	// fury = 0;
}

PlayerState::~PlayerState() {
}
