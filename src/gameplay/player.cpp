/*
   Feet of Fury

   gameplay/player.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "player.h"
#include "gameplay.h"
#include "stepdata.h"
#include "gpstate.h"
#include "gpskin.h"
#include "plrstate.h"
#include "character.h"
#include "stepbomb.h"
#include "options.h"

#include "bouncetxr.h"
#include "combobomb.h"
#include "lifemeter.h"
#include "winticks.h"
#include "combometer.h"
#include "rotbanner.h"

#include <ctype.h>

// Constructor / Destructor
Player::Player(Gameplay * parent, GameplaySkin * skin, GameplayState * state,
		int index, float tx, float ty, float tz, StepData * steps, PlayerState *st,
		KeyboardState * kst) {
	m_kbstate = kst;
	m_parent = parent;
	m_skin = skin;
	m_state = state;
	m_stpp = m_state->pp + index;
	m_index = index;
	m_tx = tx; m_ty = ty; m_tz = tz;
	m_stepdata = steps;
	assert( m_stepdata->steps != NULL );

	m_pst = st;

	//m_charpic = new Banner(PLX_LIST_PT_POLY,
	//	new Texture("../dcload_root/pakfiles/menu_char_src/sb_select_large.png", true));
	//m_charpic->setSize(256,512);
	
	// Start at the top, no judging yet
	m_stepidx = 0;
	m_stepidx_last = -1;
	m_judge_last = Step::NotYet;

	// Start out with no combo, and a decent amount of life
	m_stpp->furiosity = 0.0f;
	m_combo_max = 50;

	// Setup life and combo meter drawables
	m_lifemeter = new LifeMeter(Drawable::Trans, m_skin->txr_meters, m_index == 0);
	m_lifemeter->setTranslate(Vector(tx, ty, 100.0f));
	state->scene->subAdd(m_lifemeter);

	m_combometer = new ComboMeter(Drawable::Trans, m_skin->txr_meters);
	switch (m_index) {
	case 0:
		m_combometer->setTranslate(Vector(tx + 256.0f + 5.0f, ty + 32*5.0f - 5.0f, 100.0f));
		break;
	case 1:
		m_combometer->setTranslate(Vector(tx - 32.0f + 5.0f, ty + 32*5.0f - 5.0f, 100.0f));
		break;
	default:
		assert( false );
	}
	state->scene->subAdd(m_combometer);

	m_combobomb = new ComboBomb(index, m_skin->txr_twinkle);
	m_combobomb->setTranslate(Vector(320.0f, 240.0f, 80.0f));
	state->scene->subAdd(m_combobomb);

	m_attack_icon = new BounceTxr(Drawable::Trans, NULL);
	state->scene->subAdd(m_attack_icon);
	
	if (m_index == 0)
		m_attack_icon->setTranslate(Vector(303.0f, 48.0f, 101.0f));
	else
		m_attack_icon->setTranslate(Vector(336.0f, 48.0f, 100.0f));
	m_attack_icon->setScale(Vector(24.0f/64.0f, 24.0f/64.0f, 1.0f));

	m_ticks = new WinTicks(m_skin->txr_ticks, m_index == 0);
	if (m_index == 0)
		m_ticks->setTranslate(Vector(266.0f, 24.0f, 100.0f));
	else
		m_ticks->setTranslate(Vector(372.0f, 24.0f, 100.0f));
	state->scene->subAdd(m_ticks);
	updateWins();

	m_charpic = new Banner(Drawable::Trans, st->char_thumb);
	if (index == 0) {
		m_charpic->setTranslate(Vector(40, 50, 500));
		m_charpic->setUV(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	} else
		m_charpic->setTranslate(Vector(640 - 40, 50, 500));
	m_charpic->setScale(Vector(40.0f/64.0f, 40.0f/60.0f, 1.0f));
	state->scene->subAdd(m_charpic);

	m_buttons = 0;
	m_rtrig_held = false;
}

Player::~Player() {
}

void Player::updateWins() {
	if (m_pst)
		m_ticks->setWins(m_pst->wins);
}

// Draw a game arrow
// This used to be GL-based but it was too slow...
// The parameter list has grown way out of control though :)
void Player::drawArrow(float x, float y, float z, float a,
	int phase, ArrowType type, ArrowDir dir, float scale, float rotate, int item_type)
{
	float size = 32 * scale;
	Texture * txr = m_skin->txr_arrows;

	if (type == ArrowFg) {
		switch (phase) {
		case 0:  txr = m_skin->txr_arrows; break;
		case 1:  txr = m_skin->txr_arrows2; break;
		case 2:  txr = m_skin->txr_arrows3; break;
		default: assert(false);
		}
	} else if (type == ArrowBomb || type == ArrowAdded) {
		txr = m_skin->txr_arrows_bombs;
	} else {
		txr = m_skin->txr_arrows;
	}

	// We also have to find where inside the arrow matrix the
	// arrow we actually want is located
	float u[4], v[4];
	int animidx;
	if (m_state->animArrows())
		animidx = (m_state->game_beat) % 12;
	else
		animidx = 0;
	switch (type) {
	case ArrowFg:
		u[0] = (animidx / 4) / 4.0f;
		v[0] = (animidx % 4) / 4.0f;
		break;
	case ArrowBg:
		u[0] = 0.75f;
		if ((animidx % 4) == 0 || (animidx % 4) == 1)
			v[0] = 0.25f;
		else
			v[0] = 0.0f;
		break;
	case ArrowBomb:
		u[0] = (((m_state->frame) % 6) / 4) / 4.0f;
		v[0] = (((m_state->frame) % 6) % 4) / 4.0f;
		break;
	case ArrowAdded:
		u[0] = (((animidx % 6) + 6) / 4) / 4.0f;
		v[0] = (((animidx % 6) + 6) % 4) / 4.0f;
		break;
	case ArrowBlaze:
		u[0] = 0.75f;
		v[0] = 0.50f;
		break;
	default:
		assert(false);
	}

	// Setup for a normal up-arrow
	u[1] = u[0];
	v[1] = v[0]+0.25f;
	u[2] = u[0]+0.25f;
	v[2] = v[0]+0.25f;
	u[3] = u[0]+0.25f;
	v[3] = v[0];

	// Now come up with x/y coords and do the rotations
	vector_t p[4] = {
		Vector( -size, -size, 0 ),
		Vector( -size, +size, 0 ),
		Vector( +size, +size, 0 ),
		Vector( +size, -size, 0 ) };

	if (rotate != 0.0f) {
		mat_identity();
		mat_rotate_z(rotate * 2*M_PI / 360.0f);
		mat_transform(p, p, 4, sizeof(vector_t));
	}

	txr->select();

	plx_dr_state_t dr;
	plx_dr_init(&dr);
	const Color & tint = m_state->scene->getTint();
	uint32 color = plx_pack_color(a, tint.r, tint.g, tint.b);

	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[1].x, y+p[1].y, z,
		color, u[(dir+1)%4], v[(dir+1)%4]);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[0].x, y+p[0].y, z,
		color, u[(dir+0)%4], v[(dir+0)%4]);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[2].x, y+p[2].y, z,
		color, u[(dir+2)%4], v[(dir+2)%4]);
	plx_vert_ifd(
		&dr, PLX_VERT_EOS,
		x+p[3].x, y+p[3].y, z,
		color, u[(dir+3)%4], v[(dir+3)%4]);

	// Potentially also draw a bomb icon to go with it
	if (type == ArrowBomb && item_type != -1)
		drawItem(x, y, z, a, scale, rotate, item_type);
}

void Player::drawLetter(float x, float y, float z, float a, int phase,
	ArrowType type, float scale, float rotate, int ch, int item_type)
{
	// Setup for the font texture
	m_skin->txr_typing_font->select();

	// Figure out the u/v coords
	int cidx = tolower(ch) - 'a';
	int cx = cidx % 8, cy = cidx / 8;
	float u[4], v[4];
	u[0] = cx / 8.0f; v[0] = cy / 8.0f;
	
	// Setup for a normal up-arrow
	float d = 64/512.0f;
	u[1] = u[0];
	v[1] = v[0]+d;
	u[2] = u[0]+d;
	v[2] = v[0]+d;
	u[3] = u[0]+d;
	v[3] = v[0];

	// Now come up with x/y coords and do the rotations
	float size = 32 * scale;
	vector_t p[4] = {
		Vector( -size, -size, 0 ),
		Vector( -size, +size, 0 ),
		Vector( +size, +size, 0 ),
		Vector( +size, -size, 0 ) };

	if (rotate != 0.0f) {
		mat_identity();
		mat_rotate_z(rotate * 2*M_PI / 360.0f);
		mat_transform(p, p, 4, sizeof(vector_t));
	}

	plx_dr_state_t dr;
	plx_dr_init(&dr);
	Color colo;
	uint32 color;
	const Color & tint = m_state->scene->getTint();

	switch (phase) {
	case -1: colo = Color(a, 0.5f, 0.5f, 0.5f); break;
	case -2: colo = Color(a, 1.0f, 1.0f, 1.0f); break;
	case 0: colo = Color(a, 1.0f, 1.0f, 0.5f); break;
	case 1: colo = Color(a, 0.5f, 0.5f, 1.0f); break;
	case 2: colo = Color(a, 1.0f, 0.5f, 0.5f); break;
	default: colo = Color(1,0.5f,0.5f,0.5f); assert(false);
	}
	color = colo * tint;

	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[1].x, y+p[1].y, z,
		color, u[1], v[1]);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[0].x, y+p[0].y, z,
		color, u[0], v[0]);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p[2].x, y+p[2].y, z,
		color, u[2], v[2]);
	plx_vert_ifd(
		&dr, PLX_VERT_EOS,
		x+p[3].x, y+p[3].y, z,
		color, u[3], v[3]);

	/* char str[2] = {0};
	Font * fnt = m_skin->fnt;

	fnt->setAlpha(a);
	fnt->setSize(64.0f * scale);
	str[0] = ch;
	fnt->smearDrawCentered(x, y, z, str); */

	// Potentially also draw a bomb icon to go with it
	if (type == ArrowBomb && item_type != -1)
		drawItem(x, y, z, a, scale, rotate, item_type);
}

void Player::drawItem(float x, float y, float z, float a, float scale, float rotate, int item_type) {
	float size = scale * 32.0f;

	Texture * txr = NULL;
	switch (item_type) {
	case STEPFLAG_ITEM_BOMB:	txr = m_skin->txr_items[AttackBomb]; break;
	case STEPFLAG_ITEM_DRUNKEN:	txr = m_skin->txr_items[AttackDrunkenArrow]; break;
	case STEPFLAG_ITEM_VORTEX:	txr = m_skin->txr_items[AttackVortex]; break;
	case STEPFLAG_ITEM_TURBO:	txr = m_skin->txr_items[AttackTurbo]; break;
	case STEPFLAG_ITEM_PHASING:	txr = m_skin->txr_items[AttackPhasingArrows]; break;
	case STEPFLAG_ITEM_FIREBALL:	txr = m_skin->txr_items[AttackFireball]; break;
	case STEPFLAG_ITEM_HEALTH:	txr = m_skin->txr_items[AttackHealth]; break;
	case STEPFLAG_ITEM_ERASER:	txr = m_skin->txr_items[AttackEraser]; break;
	case STEPFLAG_ITEM_DISPEL:	txr = m_skin->txr_items[AttackDispel]; break;
	case STEPFLAG_ITEM_REVERSE:	txr = m_skin->txr_items[AttackReverse]; break;
	default: assert(false);
	}

	vector_t p2[4] = {
		Vector( -size, -size, 0 ),
		Vector( -size, +size, 0 ),
		Vector( +size, +size, 0 ),
		Vector( +size, -size, 0 ) };

	for (int i=0; i<4; i++) {
		p2[i].x *= 0.7f;
		p2[i].y *= 0.7f;
	}
	z += 0.5f;

	txr->select();

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	const Color & tint = m_state->scene->getTint();
	uint32 color = Color(a * 0.8f, 1.0f, 1.0f, 1.0f) * tint;

	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p2[1].x, y+p2[1].y, z,
		color, 0.0f, 1.0f);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p2[0].x, y+p2[0].y, z,
		color, 0.0f, 0.0f);
	plx_vert_ifd(
		&dr, PLX_VERT,
		x+p2[2].x, y+p2[2].y, z,
		color, 1.0f, 1.0f);
	plx_vert_ifd(
		&dr, PLX_VERT_EOS,
		x+p2[3].x, y+p2[3].y, z,
		color, 1.0f, 0.0f);
}

void Player::drawTopArrows() {
	float x = m_tx + 32.0f, y = m_ty + 32.0f, z = m_tz + 2.0f;
	float a = m_state->alpha;

	if (!m_state->typing) {	
		if (m_state->typingBars())
			drawTypingBar(x-32.0f, y-32.0f, z, a, -1);

		if (m_buttons & Step::Left) {
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowLeft, 0.8f);
		} else
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowLeft, 1.0f);
		x += 64.0f;
		if (m_buttons & Step::Down) {
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowDown, 0.8f);
		} else
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowDown, 1.0f);
		x += 64.0f;
		if (m_buttons & Step::Up) {
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowUp, 0.8f);
		} else
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowUp, 1.0f);
		x += 64.0f;
		if (m_buttons & Step::Right) {
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowRight, 0.8f);
		} else
			drawArrow(x, y, z, a, 0, ArrowBg, ArrowRight, 1.0f);
	} else {
		float zoom = 1.0f; //m_buttons ? 0.8f : 1.0f;
		int phase = (m_state->game_beat % 4) ? -1 : -2;

		drawTypingBar(x-32.0f, y-32.0f, z, a, -1);
		
		drawLetter(x, y, z, a, phase, ArrowBg, zoom, 0.0f, '{', -1);
		x += 64.0f;
		drawLetter(x, y, z, a, phase, ArrowBg, zoom, 0.0f, '{', -1);
		x += 64.0f;
		drawLetter(x, y, z, a, phase, ArrowBg, zoom, 0.0f, '{', -1);
		x += 64.0f;
		drawLetter(x, y, z, a, phase, ArrowBg, zoom, 0.0f, '{', -1);

	}
}

void Player::drawSelector() {
	/* glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glLoadIdentity();
	glTranslatef(m_tx, m_ty + 3*64, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(64.0f*4.0f, 0.0f, 0.0f);
		glVertex3f(64.0f*4.0f, 64.0f, 0.0f);
		glVertex3f(0.0f, 64.0f, 0.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D); */
	assert( false );
}

// Convert an arrow pattern into a controller pattern
uint32 Player::arrowToCont(int pattern) {
	return Step::toCont(pattern);
}

// Draw an arrow pattern, from the step data        
void Player::drawGameArrowPattern(float x, float y, float z, float a,
	int phase, ArrowType type, int pattern, float scale, int item_type, int ch)
{
	uint32 mask = arrowToCont(pattern);
	float r;

	// If we're in typing mode, don't do more than one per row
	if (m_state->typing) {
		if (pattern == 'A')
			mask = Step::Up;
		else if (pattern == 'B')
			mask = Step::Right;
		else if (pattern == '7')
			mask = Step::Left;
		else if (pattern == '1')
			mask = Step::Down;
		else if (pattern == '9')
			mask = Step::Up;
		else if (pattern == '3')
			mask = Step::Right;
	}
	if (mask && m_state->typingBars())
		drawTypingBar(x, y, z, a, phase);

	x += 32.0f; y += 32.0f;
	if (mask & Step::Left) {
		if (m_stpp->attacks[AttackVortex].active)
			r = m_stpp->attacks[AttackVortex].counter * m_stpp->attacks[AttackVortex].intensity;
		else
			r = 0.0f;
		if (m_state->typing)
			drawLetter(x, y, z, a, phase, type, scale, r, ch, item_type);
		else
			drawArrow(x, y, z, a, phase, type, ArrowLeft, scale, r, item_type);
	}

	x += 64.0f;
	if (type == ArrowBlaze) z++;
	if (mask & Step::Down) {
		if (m_stpp->attacks[AttackVortex].active)
			r = m_stpp->attacks[AttackVortex].counter * m_stpp->attacks[AttackVortex].intensity;
		else
			r = 0.0f;
		if (m_state->typing)
			drawLetter(x, y, z, a, phase, type, scale, r, ch, item_type);
		else
			drawArrow(x, y, z, a, phase, type, ArrowDown, scale, r, item_type);
	}

	x += 64.0f;
	if (type == ArrowBlaze) z++;
	if (mask & Step::Up) {
		if (m_stpp->attacks[AttackVortex].active)
			r = m_stpp->attacks[AttackVortex].counter * m_stpp->attacks[AttackVortex].intensity;
		else
			r = 0.0f;
		if (m_state->typing)
			drawLetter(x, y, z, a, phase, type, scale, r, ch, item_type);
		else
			drawArrow(x, y, z, a, phase, type, ArrowUp, scale, r, item_type);
	}

	x += 64.0f;
	if (type == ArrowBlaze) z++;
	if (mask & Step::Right) {
		if (m_stpp->attacks[AttackVortex].active)
			r = m_stpp->attacks[AttackVortex].counter * m_stpp->attacks[AttackVortex].intensity;
		else
			r = 0.0f;
		if (m_state->typing)
			drawLetter(x, y, z, a, phase, type, scale, r, ch, item_type);
		else
			drawArrow(x, y, z, a, phase, type, ArrowRight, scale, r, item_type);
	}
}

// Draw a measure bar (for edit mode)
void Player::drawMeasureBar(float x, float y, float z, int beat) {
	if (m_state->mode == GameplayState::ModePractice && beat < m_stepdata->firstStep)
		return;

	// The measure bar itself
	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);
	
	Color color;
	if ((beat % 16) == 0)
		color = Color(1.0f, 0.0f, 1.0f, 0.0f);
	else if ((beat % 4) == 0)
		color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	else
		color = Color(0.2f, 0.0f, 0.0f, 0.0f);
	uint32 c = color * m_state->scene->getTint();

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	z -= 0.1f;
	plx_vert_ind(&dr, PLX_VERT, x, y, z, c);
	plx_vert_ind(&dr, PLX_VERT, x+64*4.0f, y, z, c);
	plx_vert_ind(&dr, PLX_VERT, x, y+2.0f, z, c);
	plx_vert_ind(&dr, PLX_VERT_EOS, x+64*4.0f, y+2.0f, z, c);

	/* if ((beat % 16) == 0) {
		// Measure counter
		char tmp[64]; sprintf(tmp, "%d", beat / 16);

		m_skin->fnt->set_color(1.0f, 1.0f, 1.0f);
		m_skin->fnt->begin();
		m_skin->fnt->draw(xreal + 64.0f*4, yreal + 16.0f, m_tz, 16.0f, tmp, 1.0f);
		m_skin->fnt->end();
	} */
}

void Player::drawTypingBar(float x, float y, float z, float a, int phase) {
	// The measure bar itself
	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);

	Color color, colordim;
	a *= 0.7f;
	switch (phase) {
	case -1: color = Color(a, 0.8f, 0.8f, 0.8f); colordim = Color(a, 0.2f, 0.2f, 0.2f); break;
	case 0: color = Color(a, 1.0f, 1.0f, 0.5f); colordim = Color(a, 0.25, 0.25f, 0.125f); break;
	case 1: color = Color(a, 0.5f, 0.5f, 1.0f); colordim = Color(a, 0.125f, 0.125f, 0.25f); break;
	case 2: color = Color(a, 1.0f, 0.5f, 0.5f); colordim = Color(a, 0.25f, 0.125f, 0.125f); break;
	default: return;
	}

	const Color & tint = m_state->scene->getTint();
	uint32 c = color * tint;
	uint32 cd = colordim * tint;

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	z -= 10.0f; y += 32.0f; 
	plx_vert_ind(&dr, PLX_VERT, x, y-2.0f, z, cd);
	plx_vert_ind(&dr, PLX_VERT, x+64*4.0f, y-2.0f, z, cd);
	plx_vert_ind(&dr, PLX_VERT, x, y+0.0f, z, c);
	plx_vert_ind(&dr, PLX_VERT, x+64*4.0f, y+0.0f, z, c);
	plx_vert_ind(&dr, PLX_VERT, x, y+2.0f, z, cd);
	plx_vert_ind(&dr, PLX_VERT_EOS, x+64*4.0f, y+2.0f, z, cd);
}

// Draw all game arrows, as the counters stand
void Player::drawGameArrows() {
	int	i, istart = -6, idx;
	float	a, x, y = 0, z, xoffs, yoffs;
	int	phase;
	
	idx = m_state->game_beat;

	switch (m_state->mode) {
	case GameplayState::ModePlay:
	case GameplayState::ModeDemoPlay:
	case GameplayState::ModeDemoPause:
	case GameplayState::ModePractice:
		istart = -6;
		y = (m_ty - m_state->pixelOffset(m_index)) + (idx + istart) * (m_stpp->qnpixels/4.0f);
		break;
	case GameplayState::ModeEdit:
	case GameplayState::ModeEditTest:
		istart = -32;
		y = (m_ty - m_state->pixelOffset(m_index)) + (idx + istart + 12) * (m_stpp->qnpixels/4.0f);
		break;
	default:
		assert( false );
	}

	// Go in and draw all the "normal" arrows first.
	float starty = y;
	x = m_tx; z = m_tz + 40.0f;
	//glColor4f(1.0f, 1.0f, 1.0f, m_state->alpha);
	float matc = m_stpp->attacks[AttackVortex].counter;
	for (i=istart; y<480.0f; i++) {
		switch ((i+idx) % 4) {
		case 0:
			phase = 0; break;
		case 1:
		case 3:
			phase = 2; break;
		case 2:
			phase = 1; break;
		default:
			phase = 0; break;
		}

		// I'm abusing "while" here a little -- it's really just a breakout
		// scope for aborting doing arrows.
		while /*if*/ ( (i+idx) >= 0 && (i+idx) < m_stepdata->step_cnt ) {
			if (m_stepdata->steps[(i+idx)].fade <= 0.0f)
				break;

			xoffs = 0.0f;
			if (m_stpp->attacks[AttackDrunkenArrow].active) {
				xoffs = fsin((y + m_state->frame*60.0f/hz) * 16.0f / 480.0f)
					* m_stpp->attacks[AttackDrunkenArrow].intensity;
			}
			
			yoffs = m_ty - y;
			if (y < (m_ty-2) && (m_stepdata->steps[(i+idx)].hit > Step::Miss || m_state->hitall)) {
				// Happens below now
				/* a = 1.0f - yoffs / m_stpp->qnpixels;
				if (a < 0) a = 0;
				drawGameArrowPattern(x + xoffs, y + yoffs, z + 30.0f, a * m_state->alpha, phase,
					ArrowBlaze, m_stepdata->steps[(i + idx)].pattern, 1.0f + yoffs / 96.0f,
					-1, m_stepdata->steps[(i+idx)].ch); */
			} else {
				ArrowType type = ArrowFg;
				if (m_stepdata->steps[(i+idx)].flags & STEPFLAG_ITEM_MASK)
					type = ArrowBomb;
				else if (m_stepdata->steps[(i+idx)].flags & STEPFLAG_ADDED)
					type = ArrowAdded;
				else if ((uint32)(i+idx) == m_state->game_beat && m_state->flashBlaze())
					type = ArrowBlaze;

				a = m_state->alpha * m_stepdata->steps[(i+idx)].fade;

				if (m_stpp->attacks[AttackPhasingArrows].active) {
					float fade;
					fade = fsin(m_state->frame * 2*M_PI / hz) / 2.0f - 0.5f;
					if ( ((i+idx) / 4) % 2 == 1 )
						fade = -1.0f - fade;
					fade *= m_stpp->attacks[AttackPhasingArrows].intensity;
					a += fade;
					if (a < 0.0f) a = 0.0f;
					if (a > 1.0f) a = 1.0f;
				}

				drawGameArrowPattern(x + xoffs, y, z, a,
					phase, type, m_stepdata->steps[(i + idx)].pattern, 1.0f,
					m_stepdata->steps[(i+idx)].flags & STEPFLAG_ITEM_MASK,
					m_stepdata->steps[(i+idx)].ch);

				// Draw measure bars for edit modes
				if (m_state->measureBars())
					drawMeasureBar(x, y, z, i+idx);
			}

			break;
		}

		y += m_stpp->qnpixels / 4.0f;
		z -= 0.5f;
		if (m_stpp->attacks[AttackVortex].active)
			m_stpp->attacks[AttackVortex].counter += 4.0f;
	}

	if (m_stpp->attacks[AttackVortex].active) {
		m_stpp->attacks[AttackVortex].counter = matc + 4.0f;
		if (m_stpp->attacks[AttackVortex].counter >= 360.0f)
			m_stpp->attacks[AttackVortex].counter -= 360.0f;
	}
	
	// Now go back and do arrow "blazes" as needed.
	x = m_tx; y = starty; z = m_tz + 50.0f;
	matc = m_stpp->attacks[AttackVortex].counter;
	for (i=istart; y<480.0f; i++) {
		switch ((i+idx) % 4) {
		case 0:
			phase = 0; break;
		case 1:
		case 3:
			phase = 2; break;
		case 2:
			phase = 1; break;
		default:
			phase = 0; break;
		}

		// I'm abusing "while" here a little -- it's really just a breakout
		// scope for aborting doing arrows.
		while /*if*/ ( (i+idx) >= 0 && (i+idx) < m_stepdata->step_cnt ) {
			if (m_stepdata->steps[(i+idx)].fade <= 0.0f)
				break;

			xoffs = 0.0f;
			if (m_stpp->attacks[AttackDrunkenArrow].active) {
				xoffs = fsin((y + m_state->frame*60.0f/hz) * 16.0f / 480.0f)
					* m_stpp->attacks[AttackDrunkenArrow].intensity;
			}
			
			yoffs = m_ty - y;
			if (y < (m_ty-2) && (m_stepdata->steps[(i+idx)].hit > Step::Miss || m_state->hitall)) {
				a = 1.0f - yoffs / m_stpp->qnpixels;
				if (a < 0) a = 0;
				drawGameArrowPattern(x + xoffs, y + yoffs, z, a * m_state->alpha, phase,
					ArrowBlaze, m_stepdata->steps[(i + idx)].pattern, 1.0f + yoffs / 96.0f,
					-1, m_stepdata->steps[(i+idx)].ch);
			}
			break;
		}

		y += m_stpp->qnpixels / 4.0f;
		z+=4;
		if (m_stpp->attacks[AttackVortex].active)
			m_stpp->attacks[AttackVortex].counter += 4.0f;
	}

	if (m_stpp->attacks[AttackVortex].active) {
		m_stpp->attacks[AttackVortex].counter = matc + 4.0f;
		if (m_stpp->attacks[AttackVortex].counter >= 360.0f)
			m_stpp->attacks[AttackVortex].counter -= 360.0f;
	}
}

// Draw an arrow background
void Player::drawArrowBg() {
	plx_cxt_texture(NULL);

	float alpha = 0.4f * m_state->alpha;
	float z = m_tz - 9.8f;
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	/* plx_vert_fnd(&dr, PLX_VERT, m_tx-8, 480, z, alpha, 0.0f, 1.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, m_tx-8, m_ty-8, z, alpha, 1.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, m_tx-8+256+16, 480, z, alpha, 0.0f, 0.0f, 1.0f);
	plx_vert_fnd(&dr, PLX_VERT_EOS, m_tx-8+256+16, m_ty-8, z, alpha, 1.0f, 1.0f, 0.0f); */
	plx_vert_fnd(&dr, PLX_VERT, m_tx-8, 480, z, alpha, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, m_tx-8, m_ty-8, z, alpha, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT, m_tx-8+256+16, 480, z, alpha, 0.0f, 0.0f, 0.0f);
	plx_vert_fnd(&dr, PLX_VERT_EOS, m_tx-8+256+16, m_ty-8, z, alpha, 0.0f, 0.0f, 0.0f);
}

// Draw the current "judge" banner (if any)
void Player::drawJudge() {
	if (m_judge_last == Step::NotYet)
		return;

	float v = 1.0f;
	switch (m_judge_last) {
	case Step::Perfect:
		v = 0.0f;
		break;
	case Step::Great:
		v = 1 / 5.0f;
		break;
	case Step::Good:
		v = 2 / 5.0f;
		break;
	case Step::Boo:
		v = 3 / 5.0f;
		break;
	case Step::Miss:
		v = 4 / 5.0f;
		break;
	default:
		assert(false);
	}

	float x = m_tx + 128.0f, y = 240.0f, z = 64.0f;
	float sc = m_judge_scale;
	plx_dr_state_t dr;
	plx_dr_init(&dr);

	m_skin->txr_judge->select();

	uint32 color = m_state->scene->getTint();

	plx_vert_ifd(&dr, PLX_VERT, x-128*sc, y+(256.0f/10.0f)*sc, z, color, 0.0f, v+1.0f/5.0f);
	plx_vert_ifd(&dr, PLX_VERT, x-128*sc, y-(256.0f/10.0f)*sc, z, color, 0.0f, v);
	plx_vert_ifd(&dr, PLX_VERT, x+128*sc, y+(256.0f/10.0f)*sc, z, color, 1.0f, v+1.0f/5.0f);
	plx_vert_ifd(&dr, PLX_VERT_EOS, x+128*sc, y-(256.0f/10.0f)*sc, z, color, 1.0f, v);

	if (m_judge_scale < 1.0f)
		m_judge_scale += 0.1f;
}

// Draw the life meter
void Player::drawLife() {
	m_lifemeter->setValue(m_stpp->life);
	// m_lifemeter->nextFrame();
	m_lifemeter->drawAll(Drawable::Trans);
}

// Draw the combo meter / combo count / etc
void Player::drawCombo() {
	m_combometer->setValue(m_stpp->furiosity / m_combo_max);
	if (m_combometer->getValue() > 1.0f)
		m_combometer->setValue(1.0f);
	// m_combometer->nextFrame();
	m_combometer->drawAll(Drawable::Trans);
}

// Overall visuals per-frame
void Player::visualPerFrame(Drawable::ObjType list) {
	/* if (list == PLX_LIST_PT_POLY) {
		m_charpic->setTranslate(Vector(m_tx + 128.0f, 256.0f, m_tz - 9.9f));
		m_charpic->draw(PLX_LIST_PT_POLY);
	} */
	
	if (list != Drawable::Trans)
		return;

	if (m_state->fullPlayfield()) {
		if (m_state->meters()) {
			m_combobomb->drawAll(Drawable::Trans);
			// m_combobomb->nextFrame();
		}

		// Draw a background behind the arrows
		drawArrowBg();

		// Draw arrows at the top
		drawTopArrows();

		// Draw character pics
		if (m_state->showCharacters())
			m_charpic->drawAll(Drawable::Trans);
	} else if (m_state->selector()) {
		drawSelector();
	}

	if (m_state->state == GameplayState::StateNormal) {
		if (m_state->gap_used || (m_state->mode != GameplayState::ModePlay && m_state->mode != GameplayState::ModePractice)) {
			// Draw game arrows
			drawGameArrows();
		}

		if (m_state->judge())
			drawJudge();

		/* if (m_state->meters()) {
			m_state->scene->draw(PLX_LIST_TR_POLY);
		} */
		if (m_rev_banner)
			m_rev_banner->drawAll(Drawable::Trans);

		attacksPerFrame();
	}
	if (m_state->meters()) {
		drawLife();
		drawCombo();
		if (m_attack_icon) {
			m_attack_icon->drawAll(Drawable::Trans);
			// m_attack_icon->nextFrame();
		}
		m_ticks->drawAll(Drawable::Trans);
		// m_ticks->nextFrame();
	}
}

// Do scoring each frame
void Player::scoring() {
	// Grab the game time
	int gt = m_state->game_time;
	int gb = (int)(gt / m_stepdata->timeunit);

	// Search in the vicinity of the current song time looking for an
	// arrow pattern that matches what the player is pressing. If we find
	// one close enough, then that's the one the player was pressing for.
	// Note that this allows arrow presses to overlap, and the closest
	// arrow will always be the one used (in case an arrow was missed in
	// a long string of 16ths, etc)
	int idx = -1;
	float distance = 100000000.0f, thisdist;
	if (!m_stpp->ai) {
		// Check actual user controls
		if (m_buttons ^ m_buttons_last) {
			for (int i=gb - 2; i<gb + 2; i++) {
				// Are we within bounds?
				if (i < 0) continue;
				if (i >= m_stepdata->step_cnt)
					break;

				// Is the note already used?
				if (m_stepdata->steps[i].hit != Step::NotYet)
					continue;

				// How far are we away from this note?
				thisdist = m_stepdata->steps[i].d - gt;
				thisdist = thisdist < 0 ? -thisdist : thisdist;

				// If it's a blank, then also forget about it
				if (m_stepdata->steps[i].pattern == '0' || (m_stepdata->steps[i].flags & STEPFLAG_ERASED))
					continue;

				// Typing or controller mode...
				if (m_state->typing) {
					// Typing -- process in terms of chars instead of patterns
					int ch = tolower(m_stepdata->steps[i].ch) - 'a';
					if (m_buttons & (1 << ch)) {
						// Is it closer than any other arrow so far?
						if (thisdist < distance) {
							idx = i;
							distance = thisdist;
						}
					}
				} else {
					// Is it the pattern the user is hitting?
					uint32 pattern = arrowToCont(m_stepdata->steps[i].pattern);
					if ((pattern & m_buttons) == pattern) {
						// Is it closer than any other arrow so far?
						if (thisdist < distance) {
							idx = i;
							distance = thisdist;
						}
					}
				}
			}
		}

		// Did the player miss something?
		int ep = gb - 3;
		if (idx != -1)
			ep = idx;
		for (int i=m_stepidx_last+1; i<ep; i++) {
			if (m_stepdata->steps[i].pattern != '0') {
				if (m_stepdata->steps[i].hit == Step::NotYet) {
					if (!(m_stepdata->steps[i].flags & (STEPFLAG_ITEM_MASK | STEPFLAG_ERASED))) {
						m_judge_last = Step::Miss;
						m_judge_scale = 0.8f;
						m_stepdata->steps[i].hit = Step::Miss;
						m_stpp->furiosity = 0.0f;		// Combo stop!
						m_stpp->life -= 0.03f;
						if (m_stpp->life < 0.0f)
							m_stpp->life = 0.0f;
						m_stepidx_last = i;
					}
				}
			}
		}
	} else {
		// If we have an arrow underneath us now, check for AI accuracy
		if (m_stepdata->steps[gb].pattern != '0' && !(m_stepdata->steps[gb].flags & STEPFLAG_ERASED)) {
			float acc = aiAccuracy();
			if (acc == 0.0f) {
				m_judge_last = Step::Miss;
				m_judge_scale = 0.8f;
				m_stepdata->steps[gb].hit = Step::Miss;
				m_stpp->furiosity = 0.0f;
				m_stpp->life -= 0.03f;
				if (m_stpp->life < 0.0f)
					m_stpp->life = 0.0f;
			} else {
				idx = gb;
				distance = m_stepdata->timeunit * 3.0f  * (1.0f - acc);
			}
		}
	}

	// Do we care?
	if (idx == -1)
		return;

	// Update the last step we actually hit (to save time scoring Misses)
	//m_stepidx_last = idx;

	// Figure out how well they hit it
	Step & step = m_stepdata->steps[idx];
	/*printf("gt/gb %d/%d: Hit arrow at %d (gt=%d), pattern %c, distance was %4.4f\n",
		gt, gb, idx, step.d, step.pattern, distance); */
	if (distance <= m_stepdata->timeunit * 0.9f) {
		if (step.flags & STEPFLAG_ITEM_MASK) {
		} else {
			m_stpp->furiosity += 1.0f;
			m_stpp->life += 0.0025f; if (m_stpp->life > 1.0f) m_stpp->life = 1.0f;
		}
		m_judge_last = Step::Perfect;
		m_judge_scale = 0.8f;
		step.hit = Step::Perfect;
	} else if (distance <= m_stepdata->timeunit * 1.3f) {
		if (step.flags & STEPFLAG_ITEM_MASK) {
		} else {
			m_stpp->furiosity += 0.8f;
			m_stpp->life += 0.00125f; if (m_stpp->life > 1.0f) m_stpp->life = 1.0f;
		}
		m_judge_last = Step::Great;
		m_judge_scale = 0.8f;
		step.hit = Step::Great;
	} else if (distance <= m_stepdata->timeunit * 1.5f) {
		if (step.flags & STEPFLAG_ITEM_MASK) {
		} else {
			m_stpp->furiosity += 0.2f;
			m_stpp->life -= 0.001f; if (m_stpp->life < 0.0f) m_stpp->life = 0.0f;
			m_judge_last = Step::Good;
			m_judge_scale = 0.8f;
		}
		step.hit = Step::Good;
	} else {
		if (step.flags & STEPFLAG_ITEM_MASK) {
		} else {
			m_stpp->furiosity = 0.0f;		// Combo stop!
			m_stpp->life -= 0.00125f; if (m_stpp->life < 0.0f) m_stpp->life = 0.0f;
			m_judge_last = Step::Boo;
			m_judge_scale = 0.8f;
		}
		step.hit = Step::Boo;
	}

	// If it was an item and they actually hit it, then collect it
	if ((step.hit == Step::Perfect || step.hit == Step::Great
		|| step.hit == Step::Good) && (step.flags & STEPFLAG_ITEM_MASK)
		&& m_state->allowAttacks())
	{
		Attack type = AttackNone;
		
		switch (step.flags & STEPFLAG_ITEM_MASK) {
		case STEPFLAG_ITEM_BOMB:	type = AttackBomb; break;
		case STEPFLAG_ITEM_DRUNKEN:	type = AttackDrunkenArrow; break;
		case STEPFLAG_ITEM_VORTEX:	type = AttackVortex; break;
		case STEPFLAG_ITEM_TURBO:	type = AttackTurbo; break;
		case STEPFLAG_ITEM_PHASING:	type = AttackPhasingArrows; break;
		case STEPFLAG_ITEM_FIREBALL:	type = AttackFireball; break;
		case STEPFLAG_ITEM_HEALTH:	type = AttackHealth; break;
		case STEPFLAG_ITEM_ERASER:	type = AttackEraser; break;
		case STEPFLAG_ITEM_DISPEL:	type = AttackDispel; break;
		case STEPFLAG_ITEM_REVERSE:	type = AttackReverse; break;
		default:	assert(false);
		}

		m_stpp->attack_avail = type;
		m_attack_icon->setTexture(m_skin->txr_items[type]);
		/* if (m_parent) {
			m_parent->send_bomb(this, (1.0f - (distance / 4.0f)) * m_combo);
		}
		m_combo = 0.0f; */
	}
}

// Util func to convert the keyboard matrix to a bitmask (32 bits, 26 alphas)
static uint32 keysToMask(KeyboardState * mat) {
	uint32 mask = 0;

	for (int i=0; i<26; i++) {
		if (mat->isPressed('a' + i))
			mask |= (1 << i);
	}

	return mask;
}

// Translate a keyboard state into a controller-like state for gameplay.
// XXX Allow mappings from control panel
static uint32 mapKeyState(int idx, KeyboardState * mat) {
	uint32 mask = 0;

	switch(idx) {
	case 0:
		if (mat->isPressed('a'))
			mask |= Step::Left;
		if (mat->isPressed('d'))
			mask |= Step::Right;
		if (mat->isPressed('s'))
			mask |= Step::Down;
		if (mat->isPressed('w'))
			mask |= Step::Up;
		// XXX
		/* if (mat[KBD_KEY_SPACE] > 0)
			mask |= CONT_RTRIG; */
		break;
	case 1:
		if (mat->isPressed(Hid::Event::KeyLeft))
			mask |= Step::Left;
		if (mat->isPressed(Hid::Event::KeyRight))
			mask |= Step::Right;
		if (mat->isPressed(Hid::Event::KeyDown))
			mask |= Step::Down;
		if (mat->isPressed(Hid::Event::KeyUp))
			mask |= Step::Up;
		// XXX
		/* if (mat[KBD_KEY_ENTER] > 0)
			mask |= CONT_RTRIG; */
		break;
	}
	
	// XXX
	/* if (mat[KBD_KEY_ESCAPE] > 0)
		mask |= CONT_START; */
	return mask;
}

// Translate a raw controller state into something matching what
// we're expecting for gameplay. This takes care of mapping
// the buttons back and forth with the option settings.
// XXX
#if 0
static uint32 mapState(cont_state_t *st) {
	// Map in ltrig and rtrig
	if (st->ltrig > 0)
		st->buttons |= CONT_LTRIG;
	if (st->rtrig > 0)
		st->buttons |= CONT_RTRIG;

	// Ok, go through and check for each key type. If we find
	// a match, add it to the output mask.
	uint32 omask = 0;
	if ((st->buttons & game_options.buttonSettings[Options::Up1]) || (st->buttons & game_options.buttonSettings[Options::Up2]))
		omask |= CONT_DPAD_UP;
	if ((st->buttons & game_options.buttonSettings[Options::Left1]) || (st->buttons & game_options.buttonSettings[Options::Left2]))
		omask |= CONT_DPAD_LEFT;
	if ((st->buttons & game_options.buttonSettings[Options::Right1]) || (st->buttons & game_options.buttonSettings[Options::Right2]))
		omask |= CONT_DPAD_RIGHT;
	if ((st->buttons & game_options.buttonSettings[Options::Down1]) || (st->buttons & game_options.buttonSettings[Options::Down2]))
		omask |= CONT_DPAD_DOWN;
	if (st->buttons & game_options.buttonSettings[Options::UseItem])
		omask |= CONT_RTRIG;

	return omask;
}
#endif

// Controller polling
void Player::controlPerFrame() {
	bool requestAttack = false;

	if (m_parent->isDemoMode()) {
		if (m_kbstate->isAnyKeyPressed())
			m_parent->exitNow();
	} else if (!m_state->typing) {
		// Standard controller semantics here

		m_buttons_last = m_buttons;
		m_buttons = mapKeyState(m_index, m_kbstate);
		/* 	if (st->buttons & CONT_START)
				if (game_options.allowSystemMenu)
					m_parent->sysMenuPopup(); */

/*#ifdef RUN_FROM_PC
			if (st->ltrig > 0) {
				sndoggvorbis_stop();
			}
#endif */

		/*	if (m_buttons & CONT_RTRIG)
				requestAttack = true; */
	} else {
		// Typing semantics
		m_buttons_last = m_buttons;
		m_buttons = keysToMask(m_kbstate);

		/*	// ESC -> quit
			if (st->matrix[KBD_KEY_DEL] && (st->shift_keys & (KBD_MOD_LCTRL | KBD_MOD_RCTRL)) &&
				(st->shift_keys & (KBD_MOD_LALT | KBD_MOD_RALT)))
			{
				m_parent->exitNow();
			} */

		// Enter -> attack
		if (m_kbstate->isPressed('\r'))
			requestAttack = true;

		/*if (m_kbstate->isPressed(Hid::Event::KeyEsc))
			m_parent->sysMenuPopup();*/

/*#ifdef RUN_FROM_PC
			if (st->ltrig > 0) {
				sndoggvorbis_stop();
			}
#endif */
	}

	if (m_state->allowAttacks()) {
		if (!m_stpp->ai) {
			if ((requestAttack && !m_rtrig_held) || m_pst->auto_attack) {
				if (m_stpp->attack_avail != AttackNone) {
					attack(m_stpp->attack_avail);
					m_stpp->attack_avail = AttackNone;
					m_attack_icon->setTexture(NULL);
				} else if (m_stpp->furiosity >= m_combo_max) {
					// m_pst->char_sound->play();
					attack(Characters::info[m_pst->charidx].tech_type, true);
					m_stpp->furiosity = 0.0f;
				}
				m_rtrig_held = 1;
			} else if (!requestAttack) {
				m_rtrig_held = 0;
			}
		} else {
			if (m_stpp->attack_avail == AttackNone && m_stpp->furiosity >= m_combo_max) {
				// m_pst->char_sound->play();
				attack(Characters::info[m_pst->charidx].tech_type, true);
				m_stpp->furiosity = 0.0f;
			}
		}
	}

	scoring();
	if (m_stpp->ai && m_stpp->attack_avail != AttackNone && m_state->allowAttacks()) {
		if (m_stpp->attack_avail >= AttackFirstDefensive &&
			m_stpp->attack_avail <= AttackLastDefensive)
		{
			int rnd = randnum(1000);
			if (rnd < 100) {
				attack(m_stpp->attack_avail);
				m_stpp->attack_avail = AttackNone;
				m_attack_icon->setTexture(NULL);
			}
		} else {
			int rnd = randnum(1000);
			if (rnd < 25) {
				attack(m_stpp->attack_avail);
				m_stpp->attack_avail = AttackNone;
				m_attack_icon->setTexture(NULL);
			}
		}
	}

	// Handle demo mode
	if (m_parent->isDemoMode()) {
		if (m_state->game_time > 40*1000) {
			if (m_state->alpha > 0.0f)
				m_state->alpha -= 1.0f/30.0f;
			else {
				m_state->alpha = 0.0f;
				m_state->state = GameplayState::StateDead;
			}
		}
	}
}

// Do an attack of some sort
void Player::attack(Attack type, bool sound) {
	if (m_state->typing && type == AttackBomb)
		type = AttackTurbo;

	// What kind of attack was it?
	if (type >= AttackFirstOffensive && type <= AttackLastOffensive) {
		// In reverse mode, offensive attacks target _us_; otherwise
		// they are sent to the opponent via m_parent.
		if (m_stpp->attacks[AttackReverse].active)
			attackReceive(type, sound);
		else
			m_parent->doAttack(this, type, sound);
	} else if (m_stpp->attacks[AttackReverse].active) {
		// In reverse mode, defensive attacks (except reverse) will
		// target _the opponent_ via m_parent; otherwise they target us.
		if (type != AttackReverse) {
			if (type == AttackDispel)
				attackReceive(type, sound);
			else
				m_parent->doAttack(this, type, sound);
		}
	} else {
		// Not in reverse mode, and we're doing a defensive attack
		if (type != AttackReverse)
			attackReceive(type, sound);
		else
			m_parent->doAttack(this, type, sound);
	}
}

// Receive an attack from another player
void Player::attackReceive(Attack type, bool sound) {
	switch (type) {
	// Offensive attacks
	case AttackBomb:
		attackBomb(10.0f, sound); break;
	case AttackDrunkenArrow:
		attackDrunken(sound); break;
	case AttackVortex:
		attackVortex(sound); break;
	case AttackTurbo:
		attackTurbo(sound); break;
	case AttackPhasingArrows:
		attackPhasingArrows(sound); break;
	case AttackFireball:
		attackFireball(sound); break;

	// Defensive attacks
	case AttackHealth:
		if (sound)
			m_skin->sfx_health->play();
		m_stpp->life += 0.1f;
		if (m_stpp->life > 1.0f)
			m_stpp->life = 1.0f;
		break;

	case AttackEraser: {
		if (sound)
			m_skin->sfx_eraser->play();
		int st = m_state->game_beat + 4;
		if (st & 3)
			st = (st & ~3) + 4;
		for (int i=st; i<st + 32; i++) {
			if (i >= m_stepdata->step_cnt)
				break;
			if (i & 7) {
				m_stepdata->steps[i].flags |= STEPFLAG_ERASED;
			}
		}
		break;
		}

	case AttackDispel:
		if (sound)
			m_skin->sfx_dispel->play();
		for (int i=0; i<AttackCount; i++) {
			if (!m_stpp->attacks[i].active) continue;
				m_stpp->attacks[i].end = m_state->game_time;
		}
		break;
			
	case AttackReverse:
		attackReverse(sound);
		break;
	
	default:	assert(false);
	}
}
        
// Do an arrow bomb attack
void Player::attackBomb(float power, bool sound) {
	StepBomb bomb;

	switch (m_stepdata->diff) {
	case StepData::Easy:
		bomb.pattern = "4000800060002000";
		break;
	case StepData::Medium:
	case StepData::Hard:
		bomb.pattern = "40806020";
		break;
	}

	int where = m_state->game_beat + 12*4;
	where = where & ~15;
	//bool initial = true;
	while (power > 0.0f) {
		/*if (initial) {
			printf("Adding bomb at %d...\n", where);
			initial = false;
		} else 
			printf("   and %d...\n", where); */
		m_stepdata->combineBomb(where, &bomb);

		where += strlen(bomb.pattern);
		power -= 8.0f;
	}

	if (sound)
		m_skin->sfx_bomb->play();
	m_combobomb->reset();
}

// Do a drunken arrow attack
void Player::attackDrunken(bool sound) {
	if (sound)
		m_skin->sfx_drunk->play();

	m_stpp->attacks[AttackDrunkenArrow].active = true;
	m_stpp->attacks[AttackDrunkenArrow].end = m_state->getGameTime()
		+ game_options.attack_duration * m_pst->secLength();
	m_stpp->attacks[AttackDrunkenArrow].intensity = 0;
}

// Do a vortex attack  
void Player::attackVortex(bool sound) {
	if (sound)
		m_skin->sfx_vortex->play();

	m_stpp->attacks[AttackVortex].active = true;
	m_stpp->attacks[AttackVortex].end = m_state->getGameTime()
		+ game_options.attack_duration * m_pst->secLength();
	m_stpp->attacks[AttackVortex].counter = 0;
	m_stpp->attacks[AttackVortex].intensity = 1.0f;
}

// Do a turbo attack 
void Player::attackTurbo(bool sound) {
	if (sound)
		m_skin->sfx_bomb->play();

	m_stpp->attacks[AttackTurbo].active = true;
	m_stpp->attacks[AttackTurbo].end = m_state->getGameTime()
		+ game_options.attack_duration * m_pst->secLength();
}

// Do a phasing arrows attack
void Player::attackPhasingArrows(bool sound) {
	if (sound)
		m_skin->sfx_phasing->play();

	// Phasing arrows are hard, so they last half as long
	m_stpp->attacks[AttackPhasingArrows].active = true;
	m_stpp->attacks[AttackPhasingArrows].end = m_state->getGameTime()
		+ game_options.attack_duration * m_pst->secLength()/2;
}

// Do a fireball attack
void Player::attackFireball(bool sound) {
	if (sound)
		m_skin->sfx_fireball->play();

	m_stpp->life -= 0.1f;
	if (m_stpp->life < 0.0f)
		m_stpp->life = 0.0f;
}

// Do a reverse attack
void Player::attackReverse(bool sound) {
	// Reverse is hard, so it lasts half as long
	if (sound)
		m_skin->sfx_phasing->play();
	m_stpp->attacks[AttackReverse].end = m_state->getGameTime()
		+ game_options.attack_duration * m_pst->secLength()/2 - 500;

	// Can't do it more than once
	if (!m_stpp->attacks[AttackReverse].active) {
		m_stpp->attacks[AttackReverse].active = true;

		m_rev_banner = new RotBanner(m_skin->txr_rev_banner, 96, 96);
		m_rev_banner->setTranslate(Vector(m_tx + 128, - 160, 500));
		m_rev_banner->animAdd(new LogXYMover(m_tx+128, 480 - 128));
		m_state->scene->subAdd(m_rev_banner);
	}
}

// End any currently running attack
void Player::attackEnd(Attack which) {
	m_stpp->attacks[which].active = false;
	m_stpp->attacks[which].end = 0xffffffff;
	m_stpp->attacks[which].counter = 0;
	m_stpp->attacks[which].intensity = 0;
	if (which == AttackTurbo)
		m_stpp->qnpixels = 64.0f;
}

// Handle any ongoing attack things
void Player::attacksPerFrame() {
	int st = m_state->game_beat;
	for (int i=st; i<(st+16*4); i++) {
		if (i >= m_stepdata->step_cnt)
			break;
		if (m_stepdata->steps[i].flags & STEPFLAG_ERASED) {
			if (m_stepdata->steps[i].fade > 0.0f) {
				m_stepdata->steps[i].fade -= 1.0f/(hz/2.0f);
				if (m_stepdata->steps[i].fade <= 0.0f)
					m_stepdata->steps[i].fade = 0.0f;
			}
		}
	}

	for (int i=0; i<=AttackCount; i++) {
		if (!m_stpp->attacks[i].active)
			continue;
			
		if (m_state->game_time < m_stpp->attacks[i].end) {
			switch (i) {
			case AttackDrunkenArrow:
				if (m_stpp->attacks[i].intensity < 24.0f)
					m_stpp->attacks[i].intensity++;
				break;
				
			case AttackTurbo:
				if (m_stpp->qnpixels < 64.0f*3.0f)
					m_stpp->qnpixels += 4.0f;
				else
					m_stpp->qnpixels = 64.0f * 3.0f;
				break;
				
			case AttackPhasingArrows:
				if (m_stpp->attacks[i].intensity < 1.0f)
					m_stpp->attacks[i].intensity += 1.0f / hz;
				else
					m_stpp->attacks[i].intensity = 1.0f;
				break;
			case AttackReverse: {
				assert( m_rev_banner != NULL );
				Color t = m_rev_banner->getTint();
				if (t.a < 1.0f) {
					t.a += 1.0f / 30.0f;
					if (t.a > 1.0f)
						t.a = 1.0f;
					m_rev_banner->setTint(t);
				}
				break;
				}
			}
		} else {
			switch (i) {
			case AttackDrunkenArrow:
				if (m_stpp->attacks[i].intensity > 0.0f)
					m_stpp->attacks[i].intensity -= 0.5f;
				else
					attackEnd((Attack)i);
				break;
				
			case AttackVortex:
				if (m_stpp->attacks[i].intensity > 0.0f)
					m_stpp->attacks[i].intensity -= 1/(hz/2.0f);
				else
					attackEnd((Attack)i);
				break;
				
			case AttackTurbo:
				if (m_stpp->qnpixels > 64.0f)
					m_stpp->qnpixels -= 2.0f;
				else
					attackEnd((Attack)i);
				break;
				
			case AttackPhasingArrows:
				if (m_stpp->attacks[i].intensity > 0.0f)
					m_stpp->attacks[i].intensity -= 1.0f / (hz/2.0f);
				else
					attackEnd((Attack)i);
				break;

			case AttackReverse: {
				assert( m_rev_banner != NULL );
				Color t = m_rev_banner->getTint();
				if (t.a > 0.0f) {
					t.a -= 1.0f / (hz/2.0f);
					if (t.a < 0.0f)
						t.a = 0.0f;
					m_rev_banner->setTint(t);
				} else {
					m_state->scene->subRemove(m_rev_banner);
					m_rev_banner = NULL;
					attackEnd((Attack)i);
				}
				break;
				}
			}
		}
	}
}
                                                
// Given all of our parameters, how likely are we to hit
// an arrow in AI mode?
float Player::aiAccuracy() {
	// Start out with a base accuracy based on the AI's accuracy rating
	float bias = m_stpp->ai_accuracy;

	// If we're being attacked, then lower the accuracy
	for (int i=AttackFirstOffensive; i<=AttackLastOffensive; i++) {
		if (m_stpp->attacks[i].active) {
			bias *= 0.9f;
		}
	}

	// Do a Gaussian distribution centered around the bias point
	// Thanks Jyrki Lahtonen
	float phi = 2.0f*M_PI*(rand() * 1.0f / RAND_MAX);
	float rho = fsqrt(-2.0f*logf(1.0 - (rand() * 1.0f / RAND_MAX)));
	float x = fcos(phi)*rho; //, y = fsin(phi)*rho;

	// Pull in the values a bit and add to the bias
	float div = randnum((int)(10 * m_stpp->ai_accuracy));
	if (div == 0) div = 1;
	x = x / div;
	x += bias;
	if (x < 0.0f) x = 0.0f;
	if (x > 1.0f) x = 1.0f;
	
	return x;
}

// Set new step data (for editor mode)
void Player::setStepData(StepData *sd) {
	m_stepdata = sd;
}
