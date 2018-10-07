/*      
   Feet of Fury
        
   textdisplay.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: textdisplay.h,v 1.10 2003/04/26 01:15:32 bard Exp $
*/

#ifndef __TEXTDISPLAY_H
#define __TEXTDISPLAY_H

class TextDisplay : public Drawable {
public:
	TextDisplay(Font * fh, float ps, bool center = false);
	virtual ~TextDisplay();

	void setText(const std::vector<string> & text);
	void setText(const char **text, int lineCnt);
	void setBGAlpha(float a);
	void setFont(Font * f);
	void setSize(float s);

	// Get our pixel size (for sub-widgeting)
	void getSize(float & w, float & h);

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	void recalcTextSize();

	float		m_w, m_h;
	float		m_ps;
	float		m_bga;
	RefPtr<Font>	m_fh;
	bool		m_center;
	int		m_text_len;
	std::vector<string>	m_text;
};

#endif	/* __TEXTDISPLAY_H */

