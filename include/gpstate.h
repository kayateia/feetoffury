/*      
   Feet of Fury
        
   gpstate.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __GPSTATE_H
#define __GPSTATE_H

struct BPMChange;

// Arrow attacks/defenses
enum Attack {
	AttackNone = -1,

	// Offensive
	AttackFirstOffensive = 0,
	AttackBomb = 0,			// Arrow Bomb
	AttackDrunkenArrow = 1,		// Drunken Arrows
	AttackVortex = 2,		// Vortex
	AttackTurbo = 3,		// Turbo
	AttackPhasingArrows = 4,	// Phasing Arrows
	AttackFireball = 5,		// Fireball
	AttackLastOffensive = 5,

	// Defensive
	AttackFirstDefensive = 6,
	AttackHealth = 6,		// Caffeine
	AttackEraser = 7,		// Reprieve
	AttackDispel = 8,		// Dispel
	AttackReverse = 9,		// Reverse
	AttackLastDefensive = 9,

	AttackCount = 10
};


// The game "state" for usage during gameplay and edit modes.
class GameplayState : public RefCnt {
public:
	GameplayState();
	virtual ~GameplayState();

	// Record the game start time
	void recordStart();

	// How many milliseconds have passed since the start time?
	uint32 getGameTime();

	// Update game_time and game_beat
	void syncTime();

	// Given the current game time and song stats, calculate how many
	// pixels "into" the song we should be at this point.
	float pixelOffset(int index);

	// Game start time. This is used to calculate the "GAP" timing
	// and all subsequent arrow timings.
	uint64		start_tm;

	// Current game time, updated before each frame is drawn
	uint32		game_time;
	uint32		game_beat;
	bool		gap_used;

	// Previous game beat (prior to syncTime call)
	uint32		old_game_beat;

	// State info (where are we?)
	enum State {
		StateNormal,
		StateWinner,
		StateDead
	};
	State		state;

	// Current frame counter
	int		frame;

	// Alpha/Fade value for everything
	float		alpha;

	// Some values from the step data
	int		gap;
	float		timeunit;

	// Are we automatically hitting all arrows (for demo mode)?
	bool		hitall;

	// Are we in typing mode?
	bool		typing;

	// Are we in play mode, or in edit mode?
	enum Mode {
		ModePlay = 0,
		ModeEdit = 1,
		ModeEditTest = 2,
		ModeDemoPlay = 3,
		ModeDemoPause = 4,
		ModePractice = 5
	};
	Mode		mode;

	// Pointer to the _next_ BPM change, if any
	BPMChange	* next_bpm_change;

	// Scene container for all active drawables, for easy
	// color tint and such.
	RefPtr<Scene>	scene;

	// Per-player states
	struct PerPlayer {
		// Player's life meter [0.0..1.0]
		float		life;

		// Player's "furiosity" meter
		float		furiosity;
	
		// Arrow attack info
		struct AttackInfo {
			bool	active;		// Is this attack active?
			uint32	end;		// When does it end in game time?
			float	counter;	// Counter for the attack
			float	intensity;	// Intensity for the attack
		}		attacks[AttackCount];

		// How many pixels is a single quarter-note (for speed modes)
		float		qnpixels;

		// What sort of attack does the player have available?
		Attack		attack_avail;

		// Are we a computer-controlled AI? If so, what's
		// our accuracy rating?
		bool		ai;
		float		ai_accuracy;

		// The actual stepdata used for this player (used in scoring)
		RefPtr<StepData>	steps;

		// Number of unlock points (filled by score screen).
		int		unlock_pts;
	} pp[2];	// XXX Fix if we ever have >2 players

	// Various characteristic macros based on mode/state
	bool animArrows();
	bool flashBlaze();
	bool measureBars();
	bool typingBars();
	bool pxlOffsets();
	bool fullPlayfield();
	bool selector();
	bool meters();
	bool judge();
	bool drawBackground();
	bool allowAttacks();
	bool doWinner();
	bool beatClaps();
	bool showCharacters();
};

#endif	/* __GPSTATE_H */

