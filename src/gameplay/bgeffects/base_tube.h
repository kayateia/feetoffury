/*      
   Feet of Fury
        
   gameplay/bgeffects/base_tube.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __BASE_TUBE_H
#define __BASE_TUBE_H

#include "tube.h"

class BaseTube : public Drawable {
public:
	BaseTube(Texture * txr) {
		Tube * t = new Tube(Opaque, txr);
		subAdd(t);
	}
	virtual ~BaseTube() { }
};

#endif	/* __BASE_TUBE_H */

