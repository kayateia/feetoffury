/*      
   Feet of Fury
        
   bgeffects.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: bgeffects.h,v 1.6 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __BGEFFECTS_H
#define __BGEFFECTS_H

// This subclass of Drawable will be the base class
// for all the "top effects".
class TopDrawable : public Drawable {
public:
	TopDrawable() { }
	virtual ~TopDrawable() { }

	// Begin a transition off the screen
	virtual void finishUp() { }
};

// The background effects drawable
class BgEffects : public Drawable {
public:
	BgEffects(Texture * txr);
	virtual ~BgEffects();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

private:
	// Produce a random effect drawable
	Drawable 	* getBaseEffect();
	TopDrawable	* getTopEffect();

	// Current and next base effects
	RefPtr<Drawable> m_base_cur, m_base_next;
	int		m_base_idx;

	// Remaining time until the next base transition (in frames)
	int		m_base_cnt;

	// Current top effect
	RefPtr<TopDrawable> m_top_cur;
	int		m_top_idx;

	// Remaining time until next top transition (in frames)
	int		m_top_cnt;

	// Frame counter
	int		m_frame;

	// Textures for use in various effects
	RefPtr<Texture>	m_txr, m_txr_cube, m_txr_bubble;
};

#endif	/* __BGEFFECTS_H */

