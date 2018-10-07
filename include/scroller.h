/*      
   Feet of Fury
        
   scroller.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __SCROLLER_H
#define __SCROLLER_H

class Scroller : public Drawable {
public:
	Scroller(Font * fnt, float ps, const char * text);
	virtual ~Scroller();

	void setText(const char * text);

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	const char *	m_text;		// Our text to display
	int		m_text_len;	// Length of the text
	RefPtr<Font>	m_font;		// The font we'll use to draw
	float		m_xoffs;	// X offset of the scroller so far
	int		m_frame;	// Frame counter
	float		m_ps;
};

#endif	/* __SCROLLER_H */

