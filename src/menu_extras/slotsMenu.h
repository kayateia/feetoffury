/*
   Feet of Fury

   menu_extras/slotsMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: slotsMenu.h,v 1.2 2003/05/03 19:29:44 bard Exp $
*/

#ifndef __SLOTS_MENU_H
#define __SLOTS_MENU_H

#include "widget.h"
#include "extrasMenu.h"

#include "textdisplay.h"

class SlotsMenu;

// Drawable for the cylinders
class Cylinder : public Drawable {
public:
	Cylinder(Texture * txr, Sound * click);
	virtual ~Cylinder();
	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);
	void stop();

	RefPtr<Texture>		m_txr;
	float			m_offs;
	float			m_speed, m_speedTo;
	float			m_accel;
	float			m_stopAt;
	bool			m_stop;
	RefPtr<Sound>		m_click;
	int			m_frame;
};

class Slot : public Drawable {
public:
	Slot(SlotsMenu * pr, Texture * txr);
	virtual ~Slot();
	void go();
	void stopAll();
	bool isStopped();
	virtual void nextFrame(uint64 tm);

	SlotsMenu		* m_parent;
	RefPtr<Cylinder>	m_cyl[3];
	bool			m_cylStopped[3];
	bool			m_stop, m_stopped;
	int			m_stopAt[3];
};

class SlotsMenu : public ExtrasMenu {
public:
	SlotsMenu(MenuExtras * parent, ExtrasMenu * back);
	virtual ~SlotsMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);
	virtual void gainFocus();

	void stopped();

private:
	// Update the various text buffers for the cursor pos
	void	updateBuffers();

	RefPtr<Texture>		m_txrSlots;

	RefPtr<Label>		m_title;

	RefPtr<Label>		m_creditsLbl;
	RefPtr<Label>		m_credits;
	RefPtr<Label>		m_winLbl;
	RefPtr<Label>		m_win;
	RefPtr<TextDisplay>	m_td;
	RefPtr<Slot>		m_slot;

	RefPtr<Sound>		m_sfxFail, m_sfxSucceed;

	char			m_creditsBuf[64];
	char			m_winBuf[64];
	bool			m_dirty;

	ExtrasMenu		* m_back;
};

#endif	// __SLOTS_MENU_H

