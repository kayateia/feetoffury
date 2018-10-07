/*      
   Feet of Fury
        
   manualviewer.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: manualviewer.h,v 1.1 2003/05/06 05:03:51 bard Exp $
*/

#ifndef __MANUALVIEWER_H
#define __MANUALVIEWER_H

class ManualImage;

class ManualViewer : public Drawable {
public:
	ManualViewer(const char * srcfn);
	virtual ~ManualViewer();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	float getOffset() const;
	void setOffset(float o);

private:
	uint8			* m_manbuf;
	int			m_tokcnt;
	int			m_height;
	RefPtr<Font>		m_font;
	RefPtr<ManualImage>	* m_images;
	float			m_yoffs;
};

#endif	/* __MANUALVIEWER_H */

