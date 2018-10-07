/*      
   Feet of Fury
        
   hsvcycler.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __HSVCYCLER_H
#define __HSVCYCLER_H

#include "hsv.h"
#include "cube.h"

class HsvCycler : public Animation {
public:
	HsvCycler() {
		m_rot = 0.0f;
	}
	virtual ~HsvCycler() { }

	virtual void nextFrame(Drawable * t) {
		float r, g, b;
		hsvSpectrum(m_rot, r, g, b);

		Color tint = t->getTint();
		tint.r = r;
		tint.g = g;
		tint.b = b;
		t->setTint(tint);

		m_rot += 1.0f/(hz*4);
		while (m_rot >= 1.0f)
			m_rot -= 1.0f;
	}

private:
	float			m_rot;
};

#endif	/* __HSVCYLER_H */

