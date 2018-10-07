/*      
   Feet of Fury
        
   gpskin.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __GPSKIN_H
#define __GPSKIN_H

#include "gpstate.h"

// Defines a "gameplay skin", which is basically just a set of textures
// that make up a gameplay screen. This is including textures, fonts,
// sound effects, etc. These will also be used in edit mode.

class GameplaySkin : public RefCnt {
public:
	GameplaySkin(bool load_for_play, int round = -1);
	virtual ~GameplaySkin();

	// Arrow textures: loaded at start time
	RefPtr<Texture>	txr_arrows;
	RefPtr<Texture>	txr_arrows2;
	RefPtr<Texture>	txr_arrows3;
	RefPtr<Texture>	txr_arrows_bombs;

	// Typing textures
	RefPtr<Texture>	txr_typing_font;

	// Background texture: again, loaded at start time
	RefPtr<Texture>	txr_framework;

	// "Step Judge" texture
	RefPtr<Texture>	txr_judge;

	// Reverse banner texture
	RefPtr<Texture>	txr_rev_banner;

	// Meter textures
	RefPtr<Texture>	txr_meters;

	// Twinkle texture for bomb
	RefPtr<Texture>	txr_twinkle;

	// Item textures
	RefPtr<Texture>	txr_items[AttackCount];

	// Winning tick textures
	RefPtr<Texture>	txr_ticks;

	// Round / Ready textures
	RefPtr<Texture>	txr_rnd, txr_ready;

	// Per-song texture for backgrounds, etc
	RefPtr<Texture>	txr_songbkg;

	// Banner for the score screen
	RefPtr<Texture>	txr_score_banner;

	// Font rendering helpers
	RefPtr<Font>	fnt, fnt_plain;

	// Sound effects
	RefPtr<Sound>	sfx_bomb, sfx_drunk, sfx_health, sfx_eraser, sfx_vortex,
			sfx_phasing, sfx_fireball, sfx_dispel;

	// Announcer voices
	RefPtr<Sound>	sfx_areready, sfx_getready, sfx_herewego, sfx_letsdance,
			sfx_letsstep, sfx_letstype, sfx_announce_rnd[3], sfx_player[2],
			sfx_wins, sfx_practicemode;

	// Practice clap
	RefPtr<Sound>	sfx_clap[2];

	// Score ticker
	RefPtr<Sound>	sfx_score_click;
};

#endif	/* __GPSKIN_H */

