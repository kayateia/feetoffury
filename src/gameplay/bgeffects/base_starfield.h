/*      
   Feet of Fury
        
   gameplay/bgeffects/base_starfield.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __BASE_STARFIELD_H
#define __BASE_STARFIELD_H

#include "starfield.h"

// A completely blank background (draw nothing)
class BaseStarField : public Drawable {
public:
	BaseStarField() {
		StarField * sf = new StarField(512);
		sf->setTranslate(Vector(0,0,BASE_Z));
		subAdd(sf);
	}
	virtual ~BaseStarField() { }
};

#endif	/* __BASE_STARFIELD_H */

