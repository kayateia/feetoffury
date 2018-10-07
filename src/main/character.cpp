/*
   Feet of Fury

   character.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "character.h"
#include "options.h"

const int Characters::count = CHAR_COUNT;

const int Characters::grid_w = 2;
const int Characters::grid_h = 5;

// Daron Veldan text
static const char * dv_text[] =
		{ "Feel free to dance",
		  "with me any time. I",
		  "will show you the",
		  "power of my DRUNKEN",
		  "ARROW technique!" };
static const int dv_text_cnt = 5;

// Sydney Veldan text
static const char * sv_text[] = 
		{ "Give me a break...",
		  "Beating you is like",
		  "stealing candy from",
		  "a kid!" };
static const int sv_text_cnt = 4;

// Kiwi text
static const char * kw_text[] =
		{ "How about you come",
		  "back later and try",
		  "again after you've",
		  "practiced for a",
		  "while?" };
static const int kw_text_cnt = 5;

// Pedro text
static const char * pv_text[] =
		{ "You can never",
		  "defeat the power",
		  "of the mighty,",
		  "mighty mambo king!" };
static const int pv_text_cnt = 4;

// MJ text
static const char * mj_text[] =
		{ "Aww yeah. Why don't",
		  "you try again later",
		  "when you can",
		  "actually keep up",
		  "with me?" };
static const int mj_text_cnt = 5;

// Charlotte text
static const char * cm_text[] =
		{ "Boo! I'm so good,",
		  "it's just scary! Do",
		  "a little worse next",
		  "time and you can",
		  "join me!" };
static const int cm_text_cnt = 5;

// Vengeance text
static const char * vn_text[] =
		{ "There's no doubt",
		  "that sea life is",
		  "superior when we",
		  "can beat you humans",
		  "in your own games!" };
static const int vn_text_cnt = 5;

// Pico text
static const char * pi_text[] =
		{ "I hate to melt your",
		  "ambitions, but",
		  "nothing will stop",
		  "me from getting",
		  "back my true form!" };
static const int pi_text_cnt = 5;

// Sedary text
static const char * sd_text[] =
		{ "I am dyno-mite!",
		  "Yahooooo! Out of",
		  "this world! Next time",
		  "try using some",
		  "power-ups!"  };
static const int sd_text_cnt = 5;

static const char * blank_text[] = { "" };

#define PREFIX ""

const Character Characters::info[] = {
	Character(
		"Daron Veldan",
		true,
		"NAME: DARON VELDAN * TECHNIQUE: DRUNKEN ARROWS * "
		"DARON AND SYDNEY VELDAN ARE A FRIENDLY BROTHER/SISTER "
		"TEAM WHICH COMPETE IN FEET OF FURY FOR THE SHEER FUN OF "
		"THE DANCE * NORMAL CLUBS WERE GETTING BORING FOR THE TWO OF THEM "
		"SO THEY HAVE NOW VOWED TO BECOME THE BEST DANCERS IN THE "
		"WHOLE WORLD ****** ",
		0, 0,
		dv_text, dv_text_cnt,
		"Drunken Arrows", AttackDrunkenArrow,
		PREFIX "dv_select_large",
		PREFIX "dv_select_thumb_color",
		PREFIX "dv_select_thumb_gray",
		PREFIX "dv_banner",
		"dv_sound" ),

	Character(
		"Sydney Veldan",
		true,
		"NAME: SYDNEY VELDAN * TECHNIQUE: ELIXIR * "
		"DARON AND SYDNEY VELDAN ARE A FRIENDLY BROTHER/SISTER "
		"TEAM WHICH COMPETE IN FEET OF FURY FOR THE SHEER FUN OF "
		"THE DANCE * NORMAL CLUBS WERE GETTING BORING FOR THE TWO OF THEM "
		"SO THEY HAVE NOW VOWED TO BECOME THE BEST DANCERS IN THE "
		"WHOLE WORLD ****** ",
		1, 0,
		sv_text, sv_text_cnt,
		"Elixir", AttackHealth,
		PREFIX "sv_select_large",
		PREFIX "sv_select_thumb_color",
		PREFIX "sv_select_thumb_gray",
		PREFIX "sv_banner",
		"sv_sound" ),

	Character(
		"Kiwi",
		true,
		"NAME: KIWI * TECHNIQUE: ARROW BOMB * CREATURE TYPE: SNOW KITTEN * "
		"KIWI WAS A MAGICAL CREATURE IN AN RPG FANTASY WORLD WHO "
		"GOT SUCKED INTO THE REAL WORLD THROUGH AN UNFORTUNATE "
		"SPELL GONE WRONG * "
		"IT WAS ONLY AFTER SHE ARRIVED THAT "
		"SHE DISCOVERED HER TRUE PASSION -- FEET OF FURY ****** ",
		0, 1,
		kw_text, kw_text_cnt,
		"Arrow Bomb", AttackBomb,
		PREFIX "kw_select_large",
		PREFIX "kw_select_thumb_color",
		PREFIX "kw_select_thumb_gray",
		PREFIX "kw_banner",
		"kw_sound" ),

	Character(
		"Pedro Villanueva",
		true,
		"NAME: PEDRO VILLANUEVA AKA MAMBO KING * TECHNIQUE: REPRIEVE * "
		"PEDRO HAS BEEN TRYING FOR YEARS TO GET HIS FAVORITE TYPE OF MUSIC "
		"INCLUDED IN FEET OF FURY -- MAMBO -- AND HAS FINALLY SUCCEEDED IN SOME "
		"VENUES * SINCE THAT TIME PEDRO HAS BEEN A FIXTURE ON THE FEET OF FURY CIRCUIT ****** ",
		1, 1,
		pv_text, pv_text_cnt,
		"Reprieve", AttackEraser,
		PREFIX "pv_select_large",
		PREFIX "pv_select_thumb_color",
		PREFIX "pv_select_thumb_gray",
		PREFIX "pv_banner",
		"pv_sound" ),

	Character(
		"Metal Junky",
		true,
		"NAME: METAL JUNKY * TECHNIQUE: TURBO * CREATURE TYPE: INTELLIGENT ROBOT * "
		"METAL JUNKY APPEARED ON THE FEET OF FURY SCENE SHORTLY AFTER THE INITIAL ARENA CONTROL MACHINE "
		"WAS SHIPPED * NO ONE KNOWS HIS TRUE ORIGINS OR HOW HE BECAME SO GOOD AT "
		"THE GAME SO QUICKLY BUT IT IS RUMORED THAT HE IS MORE THAN HE SEEMS ON THE SURFACE ****** ",
		0, 2,
		mj_text, mj_text_cnt,
		"Turbo", AttackTurbo,
		PREFIX "mj_select_large",
		PREFIX "mj_select_thumb_color",
		PREFIX "mj_select_thumb_gray",
		PREFIX "mj_banner",
		"mj_sound" ),

	Character(
		"Charlotte Montabelle",
		true,
		"NAME: CHARLOTTE MONTABELLE * TECHNIQUE: FIREBALL * CREATURE TYPE: GHOST * "
		"CHARLOTTE MONTABELLE HAD HAUNTED THE FEET OF FURY CLUBS OF TOKYO FOR YEARS BEFORE "
		"SHE FINALLY CAME OUT OF HER NORMAL SPOOKING AND TOOK DIRECT PART IN A GAME "
		"OF FEET OF FURY * THE REST IS HISTORY -- SHE NOW TERRORIZES ARENAS ACROSS "
		"THE WORLD WITH HER ETHEREAL PRESENCE ****** ",
		1, 2,
		cm_text, cm_text_cnt,
		"Fireball", AttackFireball,
		PREFIX "cm_select_large",
		PREFIX "cm_select_thumb_color",
		PREFIX "cm_select_thumb_gray",
		PREFIX "cm_banner",
		"cm_sound" ),

	Character(
		"Vengeance",
		true,
		"NAME: VENGEANCE * TECHNIQUE: VORTEX * CREATURE TYPE: EVOLVED FISH * "
		"VENGEANCE CRAWLED UP OUT OF THE SEA TO AVENGE HIS FELLOW CREATURES WHICH WERE "
		"DESTROYED BY THE ATLANTIANS 5000 YEARS AGO * HE DOESN'T REALIZE THAT "
		"MODERN HUMANS AREN'T EVEN THE SAME RACE REALLY, BUT THAT DOESN'T STOP "
		"HIM FROM USING HIS TELEKINETIC POWERS TO 'DANCE' WITH THE BEST OF THEM "
		"AND SHOW THE HUMANS THAT EVOLUTION TOOK A DOWNTURN WHEN THEY MOVED "
		"ON TO LAND ******",
		0, 3,
		vn_text, vn_text_cnt,
		"Vortex", AttackVortex,
		PREFIX "vn_select_large",
		PREFIX "vn_select_thumb_color",
		PREFIX "vn_select_thumb_gray",
		PREFIX "vn_banner",
		"vn_sound" ),

	// "Snowman"
	Character(
		"Pico",
		true,
		"NAME: PICO * TECHNIQUE: PHASING ARROWS * CREATURE TYPE: SNOWMAN * "
		"PICO USED TO BE A HUMAN MAN BUT A FREAK ACCIDENT TURNED HIM INTO A "
		"SNOWMAN MANY YEARS AGO * HE BARELY REMEMBERS WHAT IT'S LIKE TO BE A "
		"HUMAN AGAIN BUT HE KNOWS THAT HE WANTS IT AND THAT HE'LL DANCE LIKE "
		"A LUNATIC TO REGAIN HIS PREVIOUS FORM ****** ",
		1, 3,
		pi_text, pi_text_cnt,
		"Phasing Arrows", AttackPhasingArrows,
		PREFIX "pi_select_large",
		PREFIX "pi_select_thumb_color",
		PREFIX "pi_select_thumb_gray",
		PREFIX "pi_banner",
		"pi_sound" ),

	// Sedary
	Character(
		"Sedary Gatisan",
		true,
		"NAME: SEDARY GATISAN * TECHNIQUE: DISPEL * CREATURE TYPE: DANCING DRAGON * "
		"SEDARY APPEARED ON THE FEET OF FURY SCENE A FEW YEARS BACK * RUMOR HAS IT "
		"THAT HE WAS TRANSPORTED FROM THE SAME RPG WORLD AS KIWI BUT NO ONE REALLY "
		"KNOWS MUCH ABOUT HIS BACKGROUND EXCEPT THAT HE IS A TOTAL DANCING MACHINE ****** ",
		0, 4,
		sd_text, sd_text_cnt,
		"Dispel", AttackDispel,
		PREFIX "sg_select_large",
		PREFIX "sg_select_thumb_color",
		PREFIX "sg_select_thumb_gray",
		PREFIX "sg_banner",
		"pi_sound" ),

	// Random
	Character(
		"Random",
		true,
		"SELECT THIS OPTION TO CHOOSE A CHARACTER RANDOMLY ****** ",
		1, 4,
		blank_text, 1,
		"", AttackNone,
		PREFIX "rnd_select_large",
		PREFIX "rnd_select_thumb_color",
		PREFIX "rnd_select_thumb_gray",
		PREFIX "rnd_banner",
		NULL )
};

const char * Characters::locked =
"THIS CHARACTER IS STILL LOCKED. TO PLAY THESE LOCKED CHARACTERS AND UTILIZE THEIR "
"TECHNIQUES, YOU MUST USE UNLOCK POINTS IN THE EXTRAS/OPTIONS MENU.   ******  ";

int Characters::charAt(int x, int y) {
	for (int i=0; i<count; i++)
		if (x == info[i].grid_x && y == info[i].grid_y)
			return i;
	return -1;
}

int Characters::pickRandom() {
	// Make a list of available (unlocked) characters
	int ccnt = 0;
	for (int i=0; i<CHAR_COUNT; i++)
		if (game_options.char_unlocked[i]) {
			if (Characters::info[i].tech_type == AttackNone)
				continue;
			ccnt++;
		}
	int chars[CHAR_COUNT];
	ccnt = 0;
	for (int i=0; i<CHAR_COUNT; i++)
		if (game_options.char_unlocked[i]) {
			if (Characters::info[i].tech_type == AttackNone)
				continue;
			chars[ccnt] = i;
			ccnt++;
		}

	// Pick a random one from the list
	return chars[randnum(ccnt)];
}
