/*      
   Feet of Fury
        
   menu_playstyle.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __MENU_PLAYSTYLE_H
#define __MENU_PLAYSTYLE_H

// Our base class
#include "plrgenmenu.h"
#include "widget.h"

#include "gpstate.h"

#include "plrstate.h"
#include "scrollybg.h"
#include "selbar.h"
// #include "cubecube.h"
#include "menu_playstyle/attack_mode_popup.h"
#include "textdisplay.h"
#include "gridselector.h"
#include "popup.h"

// The actual menu class.
class MenuPlayStyle : public PlayerGenericMenu, public RefCnt {
public:
	MenuPlayStyle(PlayerState * p1, PlayerState * p2, StepData * song, bool typing, bool practice);
	virtual ~MenuPlayStyle();

	// Get the return code
	Main::Result getResult() const { return m_result; }

	// Resource wrapper -- pass one of these in on construction
	class MPSRes : public RefCnt {
	public:
		MPSRes();
		virtual ~MPSRes();

		RefPtr<Texture>		tech_txr[AttackCount],
					attack_modes, diff[3];
		RefPtr<Font>		font;

		RefPtr<Sound>		sfx_cursor, sfx_select;
	};

private:
	// We'll override this
	virtual void startExit();
	virtual void inputEvent(const Event & evt);
	virtual void processKeypress(int player, Event::KeyConstant key);

	// Handle the system menu popup
	void sysMenuCreate();
	void sysMenuDestroy();
	void sysMenuKey(Event::KeyConstant key);

	// One of these will be used per player. If there is only one
	// player, it will be in the center of the screen. Otherwise, it'll
	// be offset to allow room for 2.
	class Plr : public Drawable, public Widget {
	public:
		Plr(MenuPlayStyle * parent, PlayerState * ps, MPSRes * res, float tdxoffs, int idx);
		virtual ~Plr();

		virtual void draw(ObjType list);
		virtual void nextFrame(uint64 tm);

		virtual void inputEvent(const Event & evt);

		bool finished() { return m_state == StateDecided; }

	private:
		/* void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight(); */

		void updateDiff(bool tdalso);

		enum State {
			StateAttackMode,
			// StateFury,
			StateDifficulty,
			StateDecided
		};

		State			m_state;

		int			m_idx;

		MenuPlayStyle		* m_parent;
		RefPtr<PlayerState>	m_ps;
		RefPtr<MPSRes>		m_res;

		RefPtr<Banner>		m_char;

		RefPtr<AttackModePopup>	m_am;

		// RefPtr<CubeCube>	m_cc;

		RefPtr<GridSelector>	m_gs;

		RefPtr<Label>		m_decided;

		RefPtr<TextDisplay>	m_td;
	};

	// Our resources (textures, sounds, etc)
	RefPtr<MPSRes>		m_res;

	// Our scrolly background
	RefPtr<ScrollyBG>	m_sf;

	// Our two player objects
	RefPtr<Plr>		m_players[2];

	// The song we're selecting for
	RefPtr<StepData>	m_song;

	// Output result
	Main::Result	m_result;

	// Are we in practice mode?
	bool		m_practice;

	// System menu popup (if any)
	RefPtr<Scene>		m_subscene;
	RefPtr<Popup>		m_sysmenu;
	static const char	* m_cancelOpt, * m_mainMenuOpt, * m_charMenuOpt, * m_swapMenuOpt,
				* m_stageMenuOpt;
};

void testMenuPlayStyle();

#endif	/* __MENU_PLAYSTYLE_H */

