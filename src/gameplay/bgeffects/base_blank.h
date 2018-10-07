/*      
   Feet of Fury
        
   gameplay/bgeffects/base_blank.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __BASE_BLANK_H
#define __BASE_BLANK_H

// A completely blank background (draw nothing)
class BaseBlank : public Drawable {
public:
	BaseBlank() { }
	virtual ~BaseBlank() { }
	virtual void draw(ObjType list) { }
	virtual void nextFrame(uint64 tm) { }
};

#endif	/* __BASE_BLANK_H */

