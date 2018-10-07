/*
   Feet of Fury
        
   bubble_selector.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: bubble_selector.h,v 1.8 2003/04/24 01:43:05 bard Exp $
*/

#ifndef __BUBBLE_SELECTOR_H
#define __BUBBLE_SELECTOR_H

class Bubble;
class Cube;

class BubbleSelector : public Drawable {
public:
	BubbleSelector(int numchoices, int choicecur);
	virtual ~BubbleSelector();

	void setTexture(int idx, Texture * txr);
	void setEnabled(int item, bool e);

	void finalize();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

	void	choiceSwitch(int which_way);
	void	setChoice(int which);
	int	getChoice() const { return m_choice_cur; }

	// Need to call this after calling setEnabled() if you've already
	// called finalize().
	void	update();

	void	setDist(float dist);

private:
	int			m_choice_cnt;
	RefPtr<Bubble>		* m_bubbles;
	RefPtr<Cube>		* m_cubes;
	RefPtr<Texture>		* m_textures;
	bool			* m_enables;
	float			* m_rots;
	float			m_dist;
	int			m_frame;
	bool			m_finalized;

	/* int			m_choice_rot, m_choice_rot_dir, m_choice_rot_dst; */
	int			m_choice_cur;
};

#endif	/* __BUBBLE_SELECTOR_H */

