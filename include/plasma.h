/*      
   Feet of Fury
        
   plasma.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __PLASMA_H
#define __PLASMA_H

class Plasma : public Drawable {
public:
	Plasma(int list);
	virtual ~Plasma();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	void setSize(float w, float h);

private:
	int		m_list;
	Texture		* m_txr[2];
	int		m_txr_cur;

	short		m_xang, m_yang;
	short		m_xamp, m_yamp;
	float		m_w, m_h;
	float		m_hsv;
};

#endif	/* __PLASMA_H */

