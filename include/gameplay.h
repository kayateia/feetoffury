/*      
   Feet of Fury
        
   gameplay.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __GAMEPLAY_H
#define __GAMEPLAY_H

#include "winner.h"
#include "stepdata.h"
#include "plrstate.h"
#include "bgeffects.h"
#include "player.h"
#include "gpskin.h"
#include "popup.h"
#include "keyboardstate.h"

#include "gpstate.h"

class Gameplay {
public:
	Gameplay(StepData * steps, PlayerState * p1, PlayerState * p2, bool demomode, bool typing, GameplayState::Mode mode);
	virtual ~Gameplay();

	// Called to actually do the gameplay
	int doGameplay();

	// Do an attack on all other player(s)
	void doAttack(Player * from, Attack type, bool sound);

	// Are we in demo mode?
	bool isDemoMode() const { return m_demo_mode; }

	// Exit handler
	void exitNow(int who = -1);

	// Who won?
	int whoWon() const;

	// What was the result code?
	Main::Result getResult() const;

	// Called by Player to pop up the system menu
	void sysMenuPopup();

	// Called to get the skin and state to pass on to the stats screen
	GameplayState * getState() const;
	GameplaySkin * getSkin() const;
	
private:
	// Controller polling
	void controlPerFrame();

	// Draw a running game time counter (for debugging)
	void drawGameTime();

	// Convert an arrow pattern into a controller pattern
	uint32 arrowToCont(int pattern);

	// Draw framework background
	void drawFramework();

	// Overall visuals per-frame
	void visualPerFrame();

	// Do scoring each frame
	void scoring();

	// Our starting thread (for music queueing)
	static void * thdStartStatic(void *ptr);
	void thdMethod();
	void thdStart();
	void thdCleanup();
	volatile bool	m_is_queued;	// Is the thread method done?
	enum {
		ThdModeBegin,
		ThdModeWin
	}		m_thd_mode;

	RefPtr<Thread::Thread>	m_thdStart;
	bool			m_thdStartValid;

	// Keyboard input collector
	RefPtr<KeyboardState>	m_kbstate;

	// Player objects
	RefPtr<Player>		* m_players;
	int			m_player_cnt;
	RefPtr<PlayerState>	* m_psts;

	// Winner visual (for the winning player)
	RefPtr<Winner>	m_winner;
	RefPtr<Sound>	m_sfx_win;
	int		m_win_frame;

	// Who won?
	int		m_who_won;

	// Gameplay state (state, time data, frame counter, etc)
	RefPtr<GameplayState>	m_state;

	// Gameplay Skin -- contains all our textures, sounds, etc
	RefPtr<GameplaySkin>	m_skin;

	// Our background effects drawable
	RefPtr<BgEffects>	m_bg;

	// Wave texture for our intro banners
	//RefPtr<WaveTexture>	m_intro_banner;
	RefPtr<Banner>		m_intro_banner;

	// GenericMenu shell for the system menu popup
	class GMShell : public GenericMenu, public RefCnt {
	public:
		GMShell(Gameplay * parent)
			: m_parent(parent) { }
		virtual ~GMShell() { }

		virtual void inputEvent(const Event & evt) {
			m_parent->sysMenuInputEvent(evt);
		}

		void cpf() {
			controlPerFrame();
		}

		Gameplay * m_parent;
	};

	// Popup for the system menu, if applicable
	RefPtr<Popup>		m_sysmenu;
	RefPtr<GMShell>		m_sysmenugm;
	static const char	* m_restartOpt, * m_stageMenuOpt, * m_charMenuOpt,
				* m_swapMenuOpt, * m_mainMenuOpt;
	void	sysMenuCreate();
	void	sysMenuDestroy();
	void	sysMenuControlPerFrame();
	void	sysMenuInputEvent(const GenericMenu::Event & evt);

	// Are we in demo mode?
	bool		m_demo_mode;

	// Return value from doGameplay
	Main::Result	m_result;
};

// Call this from main to do the gameplay; "winner" return values:
//  -1 == user abort
//   0 == tie
//   1 == player 1 won
//   2 == player 2 won
Main::Result doGameplay(StepData * steps, PlayerState * p1, PlayerState * p2, bool demomode, bool typing, GameplayState::Mode mode, int & winner);
	
#endif	/* __GAMEPLAY_H */

