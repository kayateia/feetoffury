/*      
   Feet of Fury
        
   screenblinds.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __SCREENBLINDS_H
#define __SCREENBLINDS_H

// This class heirarchy is a bit unorthodox but it will
// serve the purpose :)
class ScreenBlinds : public Drawable, public Animation {
public:
	ScreenBlinds(bool closing);
	virtual ~ScreenBlinds();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	void drawLeftBar(const Color & col, float y, float width);
	void drawLeftTip(const Color & col, float y, float width);
	void drawRightBar(const Color & col, float y, float width);
	void drawRightTip(const Color & col, float y, float width);

	int	m_frame;
	bool	m_closing;
};

#endif	/* __SCREENBLINDS_H */

