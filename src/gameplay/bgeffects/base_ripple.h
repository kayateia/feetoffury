/*      
   Feet of Fury
        
   gameplay/bgeffects/base_ripple.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __BASE_RIPPLE_H
#define __BASE_RIPPLE_H

#include "wavetxr.h"

// A static picture that fills the screen but ripples (WaveTexture)
class BaseRipple : public Drawable {
public:
	BaseRipple(Texture * txr) {
		WaveTexture * w = new WaveTexture(txr, Opaque);
		w->setScale(Vector(640.0f/512.0f, 480.0f/512.0f, 1.0f));
		w->setTranslate(Vector(320.0f, 256.0f, BASE_Z));
		w->setMag(10.0f);
		subAdd(w);
	}
	virtual ~BaseRipple() { }
};

#endif	/* __BASE_RIPPLE_H */

