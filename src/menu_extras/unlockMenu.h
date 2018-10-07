/*
   Feet of Fury

   menu_extras/unlockMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: unlockMenu.h,v 1.3 2003/05/04 06:20:01 bard Exp $
*/

#ifndef __UNLOCK_MENU_H
#define __UNLOCK_MENU_H

#include "widget.h"
#include "extrasMenu.h"

#include "gridselector.h"
#include "textdisplay.h"
#include "picviewMenu.h"

class UnlockMenu : public ExtrasMenu {
public:
	UnlockMenu(MenuExtras * parent, ExtrasMenu * back);
	virtual ~UnlockMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);
	virtual void rescanUnlock();

private:
	// Update the various text buffers for the cursor pos
	void	updateBuffers();

	// Unlock a particular grid square, and update the options
	// file, etc.
	void	unlock(int x, int y);

	// Patch in the correct texture for an unlocked square
	void	setUnlockedTexture(int x, int y);

	RefPtr<Label>		m_title;
	RefPtr<Label>		m_creditsLbl;
	RefPtr<Label>		m_credits;
	RefPtr<Label>		m_gridLbl;
	RefPtr<Label>		m_grid;
	RefPtr<Label>		m_priceLbl;
	RefPtr<Label>		m_price;
	RefPtr<TextDisplay>	m_td;
	RefPtr<GridSelector>	m_gs;

	RefPtr<Texture>		m_txrSound, m_txrSong, m_txrArt,
				m_txrUnk, m_txrUnkGray;
	RefPtr<Sound>		m_sfxUnlocked;

	RefPtr<PicviewMenu>	m_subMenu;

	char			m_gridBuf[64];
	char			m_creditsBuf[64];
	char			m_priceBuf[64];
	int			m_x, m_y;

	ExtrasMenu		* m_back;
};

#endif	// __UNLOCK_MENU_H

