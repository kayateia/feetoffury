/*      
   Feet of Fury
        
   player.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __PLAYER_H
#define __PLAYER_H

// Forward declare
class Gameplay;
#include "lifemeter.h"
#include "gpskin.h"
#include "stepdata.h"
#include "plrstate.h"
#include "combometer.h"
#include "combobomb.h"
#include "bouncetxr.h"
#include "winticks.h"
#include "rotbanner.h"
#include "keyboardstate.h"

#include "gpstate.h"
#include "step.h"

// Arrow types (for draw_arrow)
enum ArrowType { ArrowBg, ArrowFg, ArrowBlaze, ArrowBomb, ArrowAdded };
enum ArrowDir { ArrowUp, ArrowRight, ArrowDown, ArrowLeft };

// Visuals, controls, and other stuff for an individual player
class Player : public RefCnt {
public:
	Player(Gameplay * parent, GameplaySkin * skin, GameplayState * state,
		int index, float tx, float ty, float tz, StepData * steps, PlayerState * st,
		KeyboardState * kst);
	virtual ~Player();

	// Controller polling
	void controlPerFrame();

	// Overall visuals per-frame (all in the transparent list)
	void visualPerFrame(Drawable::ObjType list);

	// Receive an attack from another player
	void attackReceive(Attack type, bool sound);

	// Set new step data (for editor mode)
	void setStepData(StepData *sd);

	// Update the wins counter after the round
	void updateWins();

private:
	// Draw a game arrow at the given position / scaling / rotation
	void drawArrow(float x, float y, float z, float a, int phase,
		ArrowType type, ArrowDir dir, float scale, float rotate = 0.0f, int item_type = -1);

	// Draw a game letter at the given position (for typing of fury)
	void drawLetter(float x, float y, float z, float a, int phase,
		ArrowType type, float scale, float rotate, int ch, int item_type = -1);

	// Draws an item on top of an arrow or letter
	void drawItem(float x, float y, float z, float a, float scale, float rotate, int item_type);

	// Draw the "back" arrows at the top of the screen
	void drawTopArrows();

	// Draw the selector for editing mode
	void drawSelector();

	// Convert an arrow pattern into a controller pattern
	uint32 arrowToCont(int pattern);

	// Draw an arrow pattern, from the step data
	void drawGameArrowPattern(float x, float y, float z, float a, int phase, ArrowType type,
		int pattern, float scale, int item_type = -1, int ch = 0);

	// Draw a measure bar (for edit mode)
	void drawMeasureBar(float x, float y, float z, int beat);

	// Draw a beat bar for typing mode
	void drawTypingBar(float x, float y, float z, float a, int phase);

	// Draw all game arrows, as the counters stand
	void drawGameArrows();

	// Draw an arrow background
	void drawArrowBg();

	// Draw the current "judge" banner (if any)
	void drawJudge();

	// Draw the life meter
	void drawLife();

	// Draw the combo meter / combo count / etc
	void drawCombo();

	// Do scoring each frame
	void scoring();

	// Do an attack of some sort
	void attack(Attack type, bool sound = true);

	// Do an arrow bomb attack
	void attackBomb(float power, bool sound);

	// Do a drunken arrow attack
	void attackDrunken(bool sound);

	// Do a vortex attack
	void attackVortex(bool sound);

	// Do a turbo attack
	void attackTurbo(bool sound);

	// Do a phasing arrows attack
	void attackPhasingArrows(bool sound);

	// Do a fireball attack
	void attackFireball(bool sound);

	// Do a reverse attack
	void attackReverse(bool sound);

	// End any currently running attack
	void attackEnd(Attack which);

	// Handle any ongoing attack things
	void attacksPerFrame();

	// Given all of our parameters, how likely are we to hit
	// an arrow in AI mode?
	float aiAccuracy();

	// Parent Gameplay object
	Gameplay	* m_parent;

	// Skin data
	RefPtr<GameplaySkin>	m_skin;

	// State data
	RefPtr<GameplayState>	m_state;
	GameplayState::PerPlayer * m_stpp;

	// What player are we?
	int		m_index;

	// Where are we located in screen-space?
	float		m_tx, m_ty, m_tz;

	// Our keyboard input state
	RefPtr<KeyboardState>	m_kbstate;

	// Life/Combo meter drawables
	RefPtr<LifeMeter>	m_lifemeter;
	RefPtr<ComboMeter>	m_combometer;

	// Keep one of these around just in case
	RefPtr<ComboBomb>	m_combobomb;

	// Bouncing graphic for attack item
	RefPtr<BounceTxr>	m_attack_icon;

	// Win tick marks
	RefPtr<WinTicks>	m_ticks;

	// Our step data
	RefPtr<StepData>	m_stepdata;

	// Our player state
	RefPtr<PlayerState>	m_pst;

	// Step judge status
	Step::Hit	m_judge_last;
	float		m_judge_scale;

	// Character mugshot
	RefPtr<Banner>		m_charpic;

	// Reverse banner pointer (if any)
	RefPtr<RotBanner>	m_rev_banner;

	// Player's button status
	uint32		m_buttons, m_buttons_last;

	// Is the right trigger held down right now?
	bool		m_rtrig_held;

	// Where are we in the steps so far? i.e., what set of arrows is the
	// set which is scheduled to be at the top next?
	int		m_stepidx;
	int		m_stepidx_last;

	// The player's current combo points
	float		m_combo;
	float		m_combo_max;
};

#endif	/* __PLAYER_H */

