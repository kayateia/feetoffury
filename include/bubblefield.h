/*      
   Feet of Fury
        
   bubblefield.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: bubblefield.h,v 1.9 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __BUBBLEFIELD_H
#define __BUBBLEFIELD_H

class BubbleField : public Drawable {
public:
	BubbleField(Texture * txr, int nb);
	virtual ~BubbleField();

	virtual void draw(ObjType list);

private:
	//int	pullRand();
	void	drawBubble(plx_dr_state_t * dr, float x, float y, float z, float wx, float wy, float dist);
	
	RefPtr<Texture>	m_txr;

	struct OneBubble {
		float   x, y, z;		// position
		float   vx, vy, vz;		// motion vector
		int     dm;			// motion change countdown 
		float   w;			// warp factors 
	} * m_bubbles;
	
	/*int		m_randcur;
	int		*m_randarray;*/
	int		m_nb;

	static const float m_max_bubble_size;
	static const int m_delta;
	//static const int m_randarray_len;
};

#endif	/* __BUBBLEFIELD_H */

