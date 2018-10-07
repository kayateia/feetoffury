/* 
   Feet of Fury
   
   drawables/gamemode.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "gamemode.h"
#include "res.h"

GameMode::GameMode() {
	m_txrs[0] = res.getTexture(Res::MenuStage, "1p", true);
	m_txrs[1] = res.getTexture(Res::MenuStage, "vs", true);

	// Start off in the requested mode
	m_mode = m_modeto = -1;

	// We want to end up with our center at (50,80).
	// When the texture is off-screen, it needs to move about -110 pixels (-78,80)

	// Magic numbers :)
	m_xpos = -108.69f;
	m_ypos = 80.0f;
	m_xvel = 22.737;
}

GameMode::~GameMode() {
}

void GameMode::nextFrame(uint64 tm) {
	if (m_mode == -1)
		return;

	// Move the block at the proper velocity until it gets 
	// approximately where we want it
	if (m_xvel > 0.0f) {
		m_xpos += m_xvel;
		m_xvel = m_xvel / 1.25f;
		if (m_xpos >= 50.0f) {
			m_xpos = 50.0f;
			m_xvel = 0.0f;
		}
	} else if (m_xvel < 0) {
		m_xpos += m_xvel;
		m_xvel = m_xvel * 1.25f;
		if (m_xpos <= -109.0f) {
			m_xpos = -108.69f;
			m_xvel = 22.737f;
			m_mode = m_modeto;
		}
	}
}

void GameMode::setMode(int mode) {
	if (m_mode == -1)
		m_mode = m_modeto = mode;
	else {
		if (m_mode != mode && m_modeto != mode) {
			m_modeto = mode;
			m_xvel = -1.0f;
		}
	}
}

void GameMode::drawGamemode() {
	if (m_mode == -1)
		return;

	m_txrs[m_mode]->select();
	plx_spr_inp(64, 64, m_xpos + 50, m_ypos, 31, getColor());
}

