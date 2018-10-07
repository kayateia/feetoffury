/*
   Feet of Fury

   menu_extras/jukeboxMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: jukeboxMenu.h,v 1.4 2003/05/04 06:20:00 bard Exp $
*/

#ifndef __JUKEBOX_MENU_H
#define __JUKEBOX_MENU_H

#include "widget.h"
#include "extrasMenu.h"

#include "gridselector.h"
#include "messagebox.h"

class JukeboxMenu : public ExtrasMenu {
public:
	JukeboxMenu(MenuExtras * parent, ExtrasMenu * back);
	virtual ~JukeboxMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);
	virtual void gainFocus();
	virtual void rescanUnlock();

private:
	// Update the various text buffers for the cursor pos
	void	updateBuffers();

	void	startSong(int x, int y);

	bool	isLocked(int sq);

	RefPtr<Texture>		m_txrLocked, m_txrSong, m_txrSound;

	RefPtr<Label>		m_title;
	RefPtr<GridSelector>	m_gs;

	RefPtr<Label>		m_artistLbl;
	RefPtr<Label>		m_artist;
	RefPtr<Label>		m_nameLbl;
	RefPtr<Label>		m_name;
	RefPtr<Label>		m_new;
	RefPtr<Label>		m_info;
	float			m_new_x, m_new_y;

	RefPtr<MessageBox>	m_msgbox;

	// We'll use this drawable container to do fading
	class FinishTrigger : public Trigger {
		void trigger(Drawable * d, Animation * a);
	};
	RefPtr<Drawable>	m_fader;

	char			m_artistBuf[64];
	char			m_nameBuf[64];
	int			m_x, m_y;

	ExtrasMenu		* m_back;
};

#endif	// __JUKEBOX_MENU_H

