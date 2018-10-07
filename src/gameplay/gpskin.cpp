/*
   Feet of Fury

   gameplay/gpskin.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "gpskin.h"
#include "res.h"

// Constructor / Destructor
GameplaySkin::GameplaySkin(bool load_for_play, int round) {
	// Right now we just hard-code all of this stuff

	// Load textures
	txr_arrows = res.getTexture(Res::Gameplay, "arrow_matrix", true);
	txr_arrows2 = res.getTexture(Res::Gameplay, "arrow_matrix_phase2", true);
	txr_arrows3 = res.getTexture(Res::Gameplay, "arrow_matrix_phase3", true);
	txr_arrows_bombs = res.getTexture(Res::Gameplay, "arrow_matrix_bombs", true);

	if (load_for_play) {
		txr_typing_font = res.getTexture(Res::Gameplay, "typing_font", true);

		char tmp[64];
		sprintf(tmp, "framework%d", (rand() % 3) + 1);
		txr_framework = res.getTexture(Res::Gameplay, tmp, true);
		txr_judge = res.getTexture(Res::Gameplay, "judge", true);
		txr_rev_banner = res.getTexture(Res::Gameplay, "reverse_banner", true);
		txr_meters = res.getTexture(Res::Gameplay, "meters", true);
		txr_twinkle = res.getTexture(Res::Gameplay, "twinkle", true);

		txr_items[AttackBomb] = res.getTexture(Res::Gameplay, "bomb", true);
		txr_items[AttackDrunkenArrow] = res.getTexture(Res::Gameplay, "drunken", true);
		txr_items[AttackVortex] = res.getTexture(Res::Gameplay, "vortex", true);
		txr_items[AttackTurbo] = res.getTexture(Res::Gameplay, "turbo", true);
		txr_items[AttackPhasingArrows] = res.getTexture(Res::Gameplay, "phasing", true);
		txr_items[AttackFireball] = res.getTexture(Res::Gameplay, "fireball", true);
		txr_items[AttackHealth] = res.getTexture(Res::Gameplay, "health", true);
		txr_items[AttackEraser] = res.getTexture(Res::Gameplay, "eraser", true);
		txr_items[AttackDispel] = res.getTexture(Res::Gameplay, "dispel", true);
		txr_items[AttackReverse] = res.getTexture(Res::Gameplay, "reverse", true);

		txr_ticks = res.getTexture(Res::Gameplay, "ticks", true);

		assert( round != -1 );
		sprintf(tmp, "round%d", round+1);
		txr_rnd = res.getTexture(Res::Gameplay, tmp, true);
		txr_ready = res.getTexture(Res::Gameplay, "ready", true);

		txr_score_banner = res.getTexture(Res::Gameplay, "score/results", true);

		// Load sound effects
		sfx_bomb = res.getSound(Res::Gameplay, "arrow_bomb");
		sfx_drunk = res.getSound(Res::Gameplay, "arrow_drunk");
		sfx_health = res.getSound(Res::Gameplay, "arrow_health");
		sfx_eraser = res.getSound(Res::Gameplay, "arrow_eraser");
		sfx_vortex = res.getSound(Res::Gameplay, "arrow_vortex");
		sfx_phasing = res.getSound(Res::Gameplay, "arrow_phasing");
		sfx_fireball = res.getSound(Res::Gameplay, "arrow_fireball");
		sfx_dispel = res.getSound(Res::Gameplay, "arrow_dispel");

		// Announcer voices
		sfx_areready = res.getSound(Res::Gameplay, "areyouready");
		sfx_getready = res.getSound(Res::Gameplay, "getready");
		sfx_herewego = res.getSound(Res::Gameplay, "herewego");
		sfx_letsdance = res.getSound(Res::Gameplay, "letsdance");
		sfx_letsstep = res.getSound(Res::Gameplay, "letsstep");
		sfx_letstype = res.getSound(Res::Gameplay, "letstype");
		sfx_announce_rnd[0] = res.getSound(Res::Gameplay, "announce_r1");
		sfx_announce_rnd[1] = res.getSound(Res::Gameplay, "announce_r2");
		sfx_announce_rnd[2] = res.getSound(Res::Gameplay, "announce_r3");
		sfx_player[0] = res.getSound(Res::Gameplay, "player1");
		sfx_player[1] = res.getSound(Res::Gameplay, "player2");
		sfx_wins = res.getSound(Res::Gameplay, "wins");
		sfx_practicemode = res.getSound(Res::Gameplay, "practicemode");

		// Practice clap
		sfx_clap[0] = res.getSound(Res::Gameplay, "practice_clap_1");
		sfx_clap[1] = res.getSound(Res::Gameplay, "practice_clap_2");

		sfx_score_click = res.getSound(Res::Gameplay, "score/score_click");
	}

	fnt = res.getFont("axaxax");
	fnt_plain = res.getFont("default");
}

GameplaySkin::~GameplaySkin() {
}

