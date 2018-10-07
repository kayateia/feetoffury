/*      
   Feet of Fury
        
   menu_score.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __MENU_SCORE_H
#define __MENU_SCORE_H

class GameplayState;
class GameplaySkin;
class ScrollyBG;
class Topbar;
class StepData;

// The actual menu class.
class MenuScore : public GenericMenu, public RefCnt {
public:
	MenuScore(GameplayState * gps, GameplaySkin * sk, int winner);
	virtual ~MenuScore();

private:
	// We need one of these per player
	class Plr : public Drawable {
	public:
		Plr(MenuScore * parent, int idx);
		virtual ~Plr();

		void initOne(int i, int value, int digits);

		virtual void nextFrame(uint64 tm);

		// Player index
		int		m_idx;

		// Our parent
		MenuScore	* m_parent;

		// Perfect, Great, Good, Boo, Miss
		RefPtr<Banner>	m_banner_types[5];

		// % Complete, Score, Unlock pnts
		RefPtr<Label>	m_lbl_types[3];

		// Actual scores
		RefPtr<Label>	m_lbl_scores[8];

		// Score buffers (for spinning numbers)
		char		m_bufs[8][64];

		// Score destination buffers
		char		m_bufs_dst[8][64];

		// Did we change anything on the last nextFrame()?
		// This should be checked and reset after nextFrame().
		bool		m_changed;
	};

	// We'll override this
	virtual void startExit();
	virtual void inputEvent(const Event & evt);
	virtual void visualPerFrame();

	// The gameplay state we're representing
	RefPtr<GameplayState>	m_gps;
	RefPtr<GameplaySkin>	m_skin;

	// Our scrolly background
	RefPtr<ScrollyBG>	m_sf;

	// Banner
	RefPtr<Banner>		m_banner;

	// Top bar
	RefPtr<Topbar>		m_topbar;

	// Player objects
	RefPtr<Plr>		m_plr[2];

	int			m_frame;
	int			m_winner;
};

void testMenuScore();

#endif	/* __MENU_SCORE_H */

