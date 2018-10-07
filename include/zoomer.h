/*
   Feet of Fury

   zoomer.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: zoomer.h,v 1.1 2003/04/07 06:59:15 bard Exp $
*/

#ifndef __ZOOMER_H
#define __ZOOMER_H

class Zoomer : public Animation {
public:
	Zoomer(const Vector & src, const Vector & dst, float speed) {
		m_src = src;
		m_dst = dst;
		m_speed = speed;
		m_frame = 0;
	}

	void nextFrame(Drawable *t) {
		if (m_frame < m_speed) {
			Vector p;

			p.x = (m_frame / m_speed) * (m_dst.x - m_src.x);
			p.y = (m_frame / m_speed) * (m_dst.y - m_src.y);
			p.z = (m_frame / m_speed) * (m_dst.z - m_src.z);

			t->setScale(p);
		
			m_frame++;
		} else {
			t->setScale(m_dst);
			complete(t);
		}
	}

private:
	Vector	m_src, m_dst;
	float	m_speed;
	int	m_frame;
};


#endif	/* __ZOOMER_H */

