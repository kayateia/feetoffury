/*      
   Feet of Fury
        
   menu_char.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __MENU_CHAR_H
#define __MENU_CHAR_H

// Base class
#include "plrgenmenu.h"

// Forward decls
#include "plrstate.h"
#include "slideshow.h"
#include "bubblefield.h"
#include "gridselector.h"
#include "topbar.h"
#include "scroller.h"
#include "popup.h"

#include "character.h"

// Our menu class
class MenuChar : public PlayerGenericMenu, public RefCnt {
public:
	MenuChar(bool typing);
	virtual ~MenuChar();

	Main::Result getResults(RefPtr<PlayerState> & p1out, RefPtr<PlayerState> & p2out);

private:
	// Methods
	void p2Out();
	void p2In();
	
	// Overrided stuff
	virtual void startExit();
	virtual void visualTransList();
	virtual void inputEvent(const Event & evt);

	virtual void processKeypress(int port, Event::KeyConstant key);
	virtual void processPeriph();

	// Handle the system menu popup
	void sysMenuCreate();
	void sysMenuDestroy();
	void sysMenuKey(Event::KeyConstant key);
	
	// Slideshow for character pics (one for each player)
	RefPtr<SlideShow>		m_chars[2];

	// Banners for character names (one for each player)
	RefPtr<Texture>			m_char_name_txrs[CHAR_COUNT];
	RefPtr<Banner>			m_char_names[2];

	// Our font
	RefPtr<Font>			m_font;

	// Bubble background
	RefPtr<BubbleField>		m_bf;

	// Grid selector
	RefPtr<GridSelector>		m_gs;
	RefPtr<Texture>			m_gs_txrs[2];
	RefPtr<Texture>			m_char_thumb_txrs[CHAR_COUNT];

	// Full-sized character textures
	RefPtr<Texture>			m_char_large_txrs[CHAR_COUNT];

	// Two "topbars" to go behind the character names
	RefPtr<Topbar>			m_namebars[2];

	// Scrollers with background info for each character
	RefPtr<Scroller>		m_char_bg[2];

	// Pointers to player state objects
	RefPtr<PlayerState>		m_pstate[2];

	// Sound effects
	RefPtr<Sound> 			m_sfx_announce;
	RefPtr<Sound>			m_sfx_select, m_sfx_cursor, m_sfx_p2in, m_sfx_invalid;

	// System menu popup (if any)
	RefPtr<Popup>			m_sysmenu;
	static const char		* m_cancelOpt, * m_mainMenuOpt, * m_swapMenuOpt;

	// The sub-scene that the actual menu itself goes into
	RefPtr<Scene>			m_chscene;

	// Are we in Typing of Fury mode?
	bool	m_typing;

	// Player stuff
	bool	m_p2start;
	bool	m_confirmed[2];

	// Port-to-player conversions
	int	m_portToPlayer[4];

	// Frame counter
	int	m_frame;

	// The return value that will be passed back in getResults()
	Main::Result	m_retval;
};

// Call this from main to do the character menu
// Returns -1 if the user requested an exit, -2 if we timed out,
// or zero otherwise. A newly allocated player state structure
// will be passed back for each player (including computer)
// if the return value is zero. Any returned structs will
// also be ref()'d already.
//
// 'menu' should be a previously created MenuChar.
//
Main::Result doMenuChar(MenuChar * menu, RefPtr<PlayerState> & p1out, RefPtr<PlayerState> & p2out);
	
#endif	/* __MENU_CHAR_H */

