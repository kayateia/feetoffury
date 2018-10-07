/*      
   Feet of Fury
        
   gameplay/bgeffects/base_static.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __BASE_STATIC_H
#define __BASE_STATIC_H

// A static picture that fills the screen (a banner)
class BaseStatic : public Drawable {
public:
	BaseStatic(Texture * txr) {
		Banner * b = new Banner(Opaque, txr);
		b->setSize(640.0f, 640.0f);
		b->setTranslate(Vector(320.0f, 256.0f, BASE_Z));
		subAdd(b);
	}
	virtual ~BaseStatic() { }
};

#endif	/* __BASE_STATIC_H */

