/*      
   Feet of Fury
        
   topbar.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: topbar.h,v 1.3 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __TOPBAR_H
#define __TOPBAR_H

class Topbar : public Drawable {
public:
	enum Orientation {
		Horizontal,
		Vertical
	};

	enum Pattern {
		Barcode,
		Checker
	};

	Topbar(float sizemax, float speed, Orientation o, Pattern p, float offset = 0.0f);
	virtual ~Topbar();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	int	m_frame;	// Frame counter
	float	m_barsize;	// For frames 0-59
	float	m_baralpha;	// For frames 60+
	float	m_barcode[64];	// Bar code pattern for frames 60+
	float	m_barcodesize;	// Total bar code pattern size in pixels
	float	m_barcodealpha;	// Bar code alpha for frames 60+
	float	m_barcodeoffs;	// Bar code pattern offset (frames 60+)

	float	m_barsizemax;	// Maximum bar size
	float	m_barcodespeed;	// Bar code animation speed

	Orientation	m_orient;	// Screen orientation
};

#endif	/* __TOPBAR_H */

