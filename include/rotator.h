/*      
   Feet of Fury
        
   rotator.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __ROTATOR_H
#define __ROTATOR_H

class Rotator : public Animation {
public:
	Rotator() {
		m_rot = 0.0f;
	}
	virtual ~Rotator() { }

	virtual void nextFrame(Drawable * t) {
		t->setRotate(Vector(0.8f, 0.4f, 0.9f, m_rot * 360.0f));
		m_rot += 1.0f/(4.0*hz);
		while (m_rot >= 1.0f)
			m_rot -= 1.0f;
	}

private:
	float			m_rot;
};

#endif	/* __ROTATOR_H */

