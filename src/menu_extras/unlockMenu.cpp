/*
   Feet of Fury

   menu_extras/unlockMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"
#include "options.h"
#include "menu_extras.h"
#include "unlock.h"
#include "stepdata.h"

#include "unlockMenu.h"
#include "picviewMenu.h"
#include "multiOptionMenu.h"

#include "gridselector.h"
#include "menubox.h"
#include "textdisplay.h"

static const int unlockW = 7, unlockH = 7;

UnlockMenu::UnlockMenu(MenuExtras * parent, ExtrasMenu * back)
	: ExtrasMenu(parent)
{
	m_back = back;

	m_sfxUnlocked = res.getSound(Res::Unlock, "unlock");

	RefPtr<Font> def = res.getFont("default");
	m_txrSound = res.getTexture(Res::Unlock, "sound");
	m_txrSong = res.getTexture(Res::Unlock, "song");
	m_txrArt = res.getTexture(Res::Unlock, "art");

	m_title = new Label(m_parent->getFont(), "Store of Fury", 36, true, false);
	m_title->setTranslate(Vector(320.0f, 60.0f, 15.0f));
	subAdd(m_title);

	float x = 320.0f, xscoot = 120.0f;
	float y = 140;

	m_gridLbl = new Label(m_parent->getFont(), "Grid:", 24, false, false);
	m_gridLbl->setTranslate(Vector(x, y, 15.0f));
	m_gridLbl->setTint(Color(0,0.85f,0));
	subAdd(m_gridLbl);

	m_gridBuf[0] = 0;
	m_grid = new Label(def, m_gridBuf, 24, false, false);
	m_grid->setTranslate(Vector(x+xscoot, y, 15.0f));
	m_grid->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_grid);

	y += 28.0f;

	m_priceLbl = new Label(m_parent->getFont(), "Price:", 24, false, false);
	m_priceLbl->setTranslate(Vector(x, y, 15.0f));
	m_priceLbl->setTint(Color(0,0.85f,0));
	subAdd(m_priceLbl);

	m_priceBuf[0] = 0;
	m_price = new Label(def, m_priceBuf, 24, false, false);
	m_price->setTranslate(Vector(x+xscoot, y, 15.0f));
	m_price->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_price);

	y += 28.0f;

	m_creditsLbl = new Label(m_parent->getFont(), "Credits:", 24, false, false);
	m_creditsLbl->setTranslate(Vector(x, y, 15.0f));
	m_creditsLbl->setTint(Color(0,0.85f,0));
	subAdd(m_creditsLbl);

	m_creditsBuf[0] = 0;
	m_credits = new Label(def, m_creditsBuf, 24, false, false);
	m_credits->setTranslate(Vector(x+xscoot, y, 15.0f));
	m_credits->setTint(Color(0.85f,0.85f,0.85f));
	subAdd(m_credits);

	y += 28.0f;

	m_txrUnk = res.getTexture(Res::Unlock, "unknown");
	m_txrUnkGray = res.getTexture(Res::Unlock, "unknown_gray");

	m_gs = new GridSelector(unlockW, unlockH, 32, 32, 1);
	for (int y=0; y<unlockH; y++)
		for (int x=0; x<unlockW; x++) {
			int sq = y * unlockW + x;
			if (game_options.isLocked(sq)) {
				if (game_options.unlock_pts < Unlock::grid[sq].price || Unlock::grid[sq].price == -1)
					m_gs->setTextureAt(x, y, m_txrUnkGray, m_txrUnkGray);
				else
					m_gs->setTextureAt(x, y, m_txrUnk, m_txrUnk);
			} else {
				setUnlockedTexture(x, y);
			}
		}
	m_gs->setSelAttrs(0, 0.3f, 1.0f, 0.3f, NULL);
	m_gs->setSelection(0, 0, 0);
	m_gs->setTranslate(Vector(160,240,13));
	m_gs->setSpacing(2.0f);
	m_gs->setSelectorOutside(false);
	subAdd(m_gs);

	MenuBox * mb = new MenuBox(true);
	mb->setTranslate(Vector(160,240,10));
	mb->setScale(Vector(unlockW*34+2+8, unlockH*34+2+8, 1));
	subAdd(mb);

	m_td = new TextDisplay(m_parent->getFont(), 18, true);
	m_td->setBGAlpha(0.4f);
	m_td->setTranslate(Vector(460, 320.0f, 15.0f));
	subAdd(m_td);

	m_x = m_y = 0;
	rescanUnlock();
	//updateBuffers();
}

UnlockMenu::~UnlockMenu() {
}

void UnlockMenu::rescanUnlock() {
	for (int y=0; y<unlockH; y++)
		for (int x=0; x<unlockW; x++) {
			int sq = y * unlockW + x;
			if (game_options.isLocked(sq)) {
				if (game_options.unlock_pts < Unlock::grid[sq].price || Unlock::grid[sq].price == -1)
					m_gs->setTextureAt(x, y, m_txrUnkGray, m_txrUnkGray);
				else
					m_gs->setTextureAt(x, y, m_txrUnk, m_txrUnk);
			}
		}
	updateBuffers();
}

void UnlockMenu::updateBuffers() {
	int sq = m_y * unlockW + m_x;

	sprintf(m_creditsBuf, "%u", game_options.unlock_pts);
	sprintf(m_gridBuf, "%c%c", 'A'+m_x, '0'+m_y);

	if (Unlock::grid[sq].price == -1)
		strcpy(m_priceBuf, "(empty)");
	else
		sprintf(m_priceBuf, "%d", Unlock::grid[sq].price);

	if (game_options.isLocked(sq)) {
		m_td->setText(Unlock::locked.text, Unlock::locked.count);
	} else {
		m_td->setText(Unlock::grid[sq].text, Unlock::grid[sq].count);
	}
}

void UnlockMenu::unlock(int x, int y) {
	int sq = y * unlockW + x;

	// If it's a pic and we've unlocked it, then this will just switch
	// to a temp sub-menu to view it.
	if (game_options.isUnlocked(sq) && !strcmp(Unlock::grid[sq].txr, "art")) {
		m_subMenu = new PicviewMenu(m_parent, this, Unlock::grid[sq].txrTarget,
			Unlock::grid[sq].artMessage, Unlock::grid[sq].artMessageCnt);
		m_parent->setSubMenu(m_subMenu, true);
		return;
	}

	// Make sure they've got enough cashola :)
	if (game_options.isUnlocked(sq) || Unlock::grid[sq].price > game_options.unlock_pts) {
		m_parent->sfxInvalid();
		return;
	}

	// Deduct it from the bank
	game_options.unlock_pts -= Unlock::grid[sq].price;

	// Mark it as unlocked
	game_options.unlock(sq);
	game_options.dirty = true;

	// Update our display
	updateBuffers();
	setUnlockedTexture(x, y);

	// Update everyone else's display
	m_parent->rescanUnlock();

	// Play a pleasant and rewarding sound effect
	m_sfxUnlocked->play();
}

void UnlockMenu::setUnlockedTexture(int x, int y) {
	int sq = y * unlockW + x;

	// If it's a standard texture, use the preloaded one
	if (!strcmp(Unlock::grid[sq].txr, "song"))
		m_gs->setTextureAt(x, y, m_txrSong, m_txrSong);
	else if (!strcmp(Unlock::grid[sq].txr, "sound"))
		m_gs->setTextureAt(x, y, m_txrSound, m_txrSound);
	else if (!strcmp(Unlock::grid[sq].txr, "art"))
		m_gs->setTextureAt(x, y, m_txrArt, m_txrArt);
	else {
		Texture * txr = res.getTexture(Res::Unlock, Unlock::grid[sq].txr);
		m_gs->setTextureAt(x, y, txr, txr);
	}
}

void UnlockMenu::inputEvent(const GenericMenu::Event & evt) {
	bool moved = false;

	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

	switch (evt.key) {
	case GenericMenu::Event::KeySelect:
		unlock(m_x, m_y);
		break;
	case GenericMenu::Event::KeyCancel:
		m_parent->setSubMenu(m_back, false);
		return;
	case GenericMenu::Event::KeyUp:
		m_y--;
		moved = true;
		break;
	case  GenericMenu::Event::KeyDown:
		m_y++;
		moved = true;
		break;
	case GenericMenu::Event::KeyLeft:
		m_x--;
		moved = true;
		break;
	case  GenericMenu::Event::KeyRight:
		m_x++;
		moved = true;
		break;
	}

	if (moved) {
		if (m_x < 0) m_x += unlockW;
		if (m_x >= unlockW) m_x -= unlockW;
		if (m_y < 0) m_y += unlockH;
		if (m_y >= unlockH) m_y -= unlockH;
		m_gs->setSelection(0, m_x, m_y);
		m_parent->sfxCursor();
	}
	updateBuffers();
}
