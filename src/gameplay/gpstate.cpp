/*
   Feet of Fury

   gameplay/gpstate.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "gpstate.h"
#include "options.h"
#include "stepdata.h"

// Constructor / Destructor
GameplayState::GameplayState() {
	start_tm = 0;
	game_time = game_beat = 0;
	gap_used = false;
	state = StateNormal;
	frame = 0;
	alpha = 1.0f;
	gap = 0;
	timeunit = 0.0f;
	hitall = false;
	typing = false;
	mode = ModePlay;
	next_bpm_change = NULL;
	scene = new Scene();
	scene->setTint(Color(1,1,1,1));

	for (int i=0; i<2; i++) {
		pp[i].life = 0.8f;
		pp[i].furiosity = 0.0f;
		for (int j=0; j<AttackCount; j++) {
			pp[i].attacks[j].active = false;
			pp[i].attacks[j].end = 0xffffffff;
			pp[i].attacks[j].counter = 0.0f;
		}
		pp[i].qnpixels = 64.0f;
		pp[i].attack_avail = AttackNone;
		pp[i].ai = false;
		pp[i].ai_accuracy = game_options.ai_skill / 100.0f;
		pp[i].unlock_pts = 0;
	}
}

GameplayState::~GameplayState() {
}

// Record the game start time; adjust for a negative gap here if neccessary
void GameplayState::recordStart() {
	uint64 tm;
	int gap2;
	
	tm = Time::gettime() / 1000;

	// Both players should have the same step data...
	if (gap < 0) {
		gap2 = -gap;
		tm -= gap2;
		gap_used = true;
	}

	start_tm = tm;
}

// How many milliseconds have passed since the start time?
uint32 GameplayState::getGameTime() {
	uint64 tm, d;

	tm = Time::gettime() / 1000;
	return tm - start_tm;
	return d;
}

// Update game_time and game_beat
void GameplayState::syncTime() {
	if (mode == ModePlay || mode == ModeEditTest || mode == ModeDemoPlay || mode == ModePractice)
		game_time = getGameTime();

	if (!gap_used && (int)game_time >= gap) {
		gap_used = true;
		start_tm += gap;
		if (mode == ModePlay || mode == ModeEditTest || mode == ModeDemoPlay || mode == ModePractice)
			game_time = getGameTime();
	}

	old_game_beat = game_beat;
	game_beat = (uint32)(game_time / timeunit);
}

// Given the current game time and song stats, calculate how many
// pixels "into" the song we should be at this point.
float GameplayState::pixelOffset(int index) {
	// Figure out what exact game beat we're at now (with fractional parts)
	float gbfloat = game_time / timeunit;

	// Each game beat is a 16th note, so divide that by 4
	gbfloat = gbfloat / 4.0f;

	// Now multiply that by the size of a quarter note
	return gbfloat * pp[index].qnpixels;
}

bool GameplayState::animArrows() {
	return mode == ModePlay || mode == ModeDemoPlay || mode == ModeDemoPause || mode == ModePractice;
}
bool GameplayState::flashBlaze() {
	return mode == ModeEdit || mode == ModeEditTest || mode == ModePractice;
}
bool GameplayState::measureBars() {
	return mode == ModeEdit || mode == ModeEditTest;
}
bool GameplayState::typingBars() {
	return (mode == ModePlay && typing) || (mode == ModePractice && game_options.practiceBeatBars);
}
bool GameplayState::pxlOffsets() {
	return mode == ModePlay || mode == ModeEditTest || mode == ModeDemoPlay || mode == ModePractice;
}
bool GameplayState::fullPlayfield() {
	return mode == ModePlay || mode == ModeDemoPlay || mode == ModeDemoPause || mode == ModePractice;
}
bool GameplayState::selector() {
	return mode == ModeEdit || mode == ModeEditTest;
}
bool GameplayState::meters() {
	return mode == ModePlay;
}
bool GameplayState::judge() {
	return mode == ModePlay || mode == ModePractice;
}
bool GameplayState::drawBackground() {
	return mode == ModePlay || (mode == ModePractice && game_options.practiceBG);
}
bool GameplayState::allowAttacks() {
	return mode != ModePractice;
}
bool GameplayState::doWinner() {
	return mode != ModePractice;
}
bool GameplayState::beatClaps() {
	return mode == ModePractice && game_options.practiceClapTrack;
}
bool GameplayState::showCharacters() {
	return mode == ModePlay;
}

