/*      
   Feet of Fury
        
   menu_extras.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __MENU_EXTRAS_H
#define __MENU_EXTRAS_H

#include "scrollybg.h"
#include "menu_extras/multiOptionMenu.h"
#include "menu_extras/jukeboxMenu.h"
#include "menu_extras/extrasMenu.h"
#include "menu_extras/unlockMenu.h"
#include "menu_extras/manualviewMenu.h"
#include "menu_extras/slotsMenu.h"

class MenuExtras : public GenericMenu {
public:
	// Methods /////////////////////////////////////////////////////////
	MenuExtras();
	virtual ~MenuExtras();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	// Get some internals for options popups
	Font * getFont() const { return m_font; }

	void sfxSelect() { m_sfx_select->play(); }
	void sfxCursor() { m_sfx_cursor->play(); }
	void sfxUnselect() { m_sfx_unselect->play(); }
	void sfxInvalid() { m_sfx_invalid->play(); }

	// Called by a sub-menu to switch to a new sub-menu
	void setSubMenu(ExtrasMenu * menu, bool moveRight, bool playSound = true);

	// Called by a sub-menu to go through and rescan everything for unlocking.
	void rescanUnlock();

	// Final results
	Main::Result getResult() const { return m_result; }

	// Have we shown the pic popup directions yet?
	bool shownPicDirs() const { return m_picPopupDirs; }
	void setPicDirsShown() { m_picPopupDirs = true; }

private:
	// Variables ///////////////////////////////////////////////////////

	// Our font
	RefPtr<Font>	m_font;

	// Sound effects
	RefPtr<Sound>	m_sfx_select, m_sfx_cursor, m_sfx_unselect, m_sfx_invalid;

	// Return code
	Main::Result	m_result;

	//////////////////////////////

	// Our scrollybg
	RefPtr<ScrollyBG>	m_sb;

	// Root of the menu tree
	RefPtr<ExtrasMenu>	m_root;

	// The menu we're currently looking at
	RefPtr<ExtrasMenu>	m_curMenu;

	// The menu that's scrolling off the side (if any)
	RefPtr<ExtrasMenu>	m_lastMenu;

	// Ref holders for all our submenus
	RefPtr<MultiOptionMenu>	m_mom;
	RefPtr<MultiOptionMenu>	m_settings, m_audioSet, m_gameSet,
				m_practiceSet, m_keySet;
	RefPtr<JukeboxMenu>	m_jukebox;
	RefPtr<UnlockMenu>	m_unlock;
	RefPtr<MultiOptionMenu>	m_manual, m_manGameplay, m_manExtras;
	RefPtr<ManualviewMenu>	m_manLegal, m_manIntro,
				m_manGameplayIntro, m_manGameplayItemBattles,
				m_manGameplayTyping, m_manGameplayPractice,
				m_manSwapCD, m_manExtrasOptions,
				m_manExtrasUnlock, m_manExtrasJukebox,
				m_manAboutMusic, m_manAboutCA, m_manAboutGSP;
	RefPtr<SlotsMenu>	m_slots;

	// Was the casino open previously?
	bool			m_casinoOpen;

	// Have we shown the info popup directions yet?
	bool			m_picPopupDirs;

	///////////////////////////////

	void finishedMoving();

	// This trigger is added to the move animations when we change menus
	// so we can be notified when the move is complete.
	class FinishTrigger : public Trigger {
	public:
		FinishTrigger(MenuExtras * parent) { m_parent = parent; }
		virtual void trigger(Drawable * t, Animation * a) {
			m_parent->finishedMoving();
		}

		MenuExtras * m_parent;
	};
};

// Call this from main to do the extras menu.
Main::Result doMenuExtras();
	
#endif	/* __MENU_EXTRAS_H */

