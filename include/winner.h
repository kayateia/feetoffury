/*      
   Feet of Fury
        
   winner.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __WINNER_H
#define __WINNER_H

class Winner : public Drawable {
public:
	Winner();
	virtual ~Winner();

	struct StarPos {
		StarPos() { }
		StarPos(int i, float xx, float yy, float ss) {
			idx = i;
			x = xx;
			y = yy;
			s = ss;
			col = Color(0,1,1,1);
			dcol = Color(0,0,0,0);
		}
		void push() {
			col += dcol;
		}
		int	idx;
		float	x, y, s;
		Color	col, dcol;
	};

	// Set the winning side and reset our parameters
	void setSide(int s);

	void drawOne(StarPos *sp);
	void drawWinner();
	virtual void draw(ObjType list) {
		if (list != Trans)
			return;
		drawWinner();
	}

	virtual void nextFrame(uint64 tm);

private:
	StarPos		m_stars[6];
	int		m_frame;
	int		m_side;
	RefPtr<Texture>	m_txr_star, m_txr_winner;
};

#endif	/* __WINNER_H */

