/*      
   Feet of Fury
        
   slideshow.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __SLIDESHOW_H
#define __SLIDESHOW_H

class SlideShow : public Drawable {
public:
	SlideShow(int numpics, int delay, float speed);
	virtual ~SlideShow();

	// Set a picture in the slideshow
	void setPic(int idx, Texture * txr, bool hflip = false);

	// Force a switch to another slide in no-auto mode
	void setSlide(int s);

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	float		m_fadespeed;
	RefPtr<Texture>	* m_pics;
	bool		* m_flips;
	int		m_pic_cnt;
	int		m_pic_cur, m_pic_to;
	float		m_fade;
	bool		m_fading;
	int		m_delay;
	int		m_cntdown;
};

#endif	/* __SLIDESHOW_H */

