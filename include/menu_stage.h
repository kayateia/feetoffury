/*      
   Feet of Fury
        
   menu_stage.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __MENU_STAGE_H
#define __MENU_STAGE_H

// Our base class
#include "plrgenmenu.h"

// Forward decls
#include "plrstate.h"
#include "gamemode.h"
#include "cdtray.h"
#include "songinfo.h"
#include "popup.h"

#include "stepdata.h"

// Class wrapper to make ref counting easier
class MenuStage : public PlayerGenericMenu, public RefCnt {
public:
	MenuStage();
	virtual ~MenuStage();

	// Call to rescan stuff after the initial construction, for background loading.
	void setPlayers(PlayerState * p1, PlayerState * p2, bool typing, bool checkPlayerCnt, int initialSong);

	// Get the selection index (after finish)
	int getSelection() const { return m_selected; }

	// Finish up any processing
	void finishUp(RefPtr<StepData> & sdout);

	// Get the number of players (after finish)
	int getPlayerCnt() const { return m_pcnt; }

	// Get the selected song index for initialSong
	int getSelectedSong() const { return m_selected; }

	// Get the return code
	Main::Result getResult() const { return m_result; }

private:
	// Overrides
	virtual void inputEvent(const Event & evt);
	virtual void startExit();
	virtual void processKeypress(int plr, Event::KeyConstant key);
	virtual void visualTransList();

	// Called to fill out our song list
	bool getSongList();

	// Start a song sample playing
	void songSample();

	// Stop the previous song sample
	void songSampleStop();

	// Switch to a new set of song data for the song info display
	void setSong(int idx);

	// Thread used to start new song sample (to avoid delays)
	static void * thdStart(void *ptr);
	void thdMethod();

	// Handle the system menu popup
	void sysMenuCreate();
	void sysMenuDestroy();
	void sysMenuKey(Event::KeyConstant key);

	// Start time for timeout
	uint32		m_start_s, m_start_ms;

	// Are we playing a song sample?
	bool		m_smp_playing;

	// What song do we believe we have selected?
	int		m_selected;

	// Frame ticker (to know when to change states, etc)
	int		m_frame;

	// Game mode drawable
	RefPtr<GameMode>	m_gamemode;

	// CD tray drawable
	RefPtr<CDTray>		m_cdtray;

	// SongInfo drawable
	RefPtr<SongInfo>	m_songinfo;

	// Background texture
	RefPtr<Texture>		m_txr_framework;
	RefPtr<Banner>		m_bkg;

	// Song info
	int			m_song_cnt;
	char			** m_song_names;
	int			* m_song_idxs;
	RefPtr<Texture>		* m_song_txrs;
	RefPtr<StepData>	* m_song_data;

	// System menu popup (if any)
	RefPtr<Scene>		m_subscene;
	RefPtr<Popup>		m_sysmenu;
	static const char 	* m_cancelOpt, * m_mainMenuOpt, * m_charMenuOpt, * m_swapMenuOpt;

	// The time the user last moved the cursor
	int			m_last_moved;

	// Sound effects
	RefPtr<Sound>	m_sfx_select, m_sfx_unselect, m_sfx_cursor, m_sfx_announce;

	// Player difficulties
	int		m_pcnt;

	// Font rendering helpers
	RefPtr<Font>	m_fnt;

	// Output variables
	RefPtr<PlayerState>	m_plr[2];

	// Output result
	Main::Result	m_result;

	// Do we need to pay attention to player count?
	bool		m_checkPlayerCnt;

	// Our ogg start thread (if any)
	RefPtr<Thread::Thread>	m_ogg_thd;
	bool		m_ogg_thd_running;
	char		m_ogg_thd_fn[64];
};


// Call this from main to do the stage menu. Returns -1 for
// user abort, -2 for timeout, or zero on success. If the
// return value is zero, then the song name will be written
// into the provided buffer, and the player states will be
// updated with difficulty settings.
//
// 'menu' should be a pre-loaded MenuStage, or NULL if we should
// put up the loading screen and do it ourselves.
//
// 'initialSong' should be -1 at first, and then its value should be
// retained across multiple invocations in the same game.
Main::Result doMenuStage(MenuStage * menu, RefPtr<StepData> & sdout, PlayerState * p1, PlayerState * p2, bool typing, bool practice, int & initialSong);
	
#endif	/* __MENU_STAGE_H */

