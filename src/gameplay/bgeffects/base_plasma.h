/*      
   Feet of Fury
        
   gameplay/bgeffects/base_plasma.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __BASE_PLASMA_H
#define __BASE_PLASMA_H

#include "plasma.h"

// "Plasma" demo effect using bilinear filtering
class BasePlasma : public Drawable {
public:
	BasePlasma() {
		Plasma *p = new Plasma(Opaque);
		p->setSize(640.0f, 480.0f);
		p->setTranslate(Vector(320.0f, 240.0f, BASE_Z));
		subAdd(p);
	}
	virtual ~BasePlasma() { }
};

#endif	/* __BASE_PLASMA_H */

