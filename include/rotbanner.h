/*
   Feet of Fury

   include/rotbanner.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: rotbanner.h,v 1.4 2003/04/24 05:26:33 bard Exp $
*/

#ifndef __ROTBANNER_H
#define __ROTBANNER_H

// A rotating banner
class RotBanner : public Drawable {
public:
	RotBanner(Texture *t, int w = -1, int h = -1) {
		m_txr = t;
		m_w = w;
		m_h = h;

		if (m_w == -1) {
			assert( t != NULL );
			m_w = t->getW();
		}
		if (m_h == -1) {
			assert( t != NULL );
			m_h = t->getH();
		}
	}
	virtual ~RotBanner() { }

	virtual void draw(ObjType list) {
		if (list != Trans || !m_txr) return;

		m_txr->select();

		Vector pos = getPosition();
		const Vector & rot = getRotate();
		const Vector & scale = getScale();
		
		mat_identity();
		mat_translate(pos.x, pos.y, pos.z);
		mat_rotate_z( rot.z );
		plx_spr_imp(
			scale.x * m_w,
			scale.y * m_h,
			0.0f, 0.0f, 0.0f,
			getColor());
	}

	virtual void nextFrame(uint64 tm) {
		Drawable::nextFrame(tm);

		Vector rot = getRotate();
		rot.z -= 2*M_PI/120.0f;
		while (rot.z < 0)
			rot.z += 2*M_PI;
		setRotate(rot);
	}

	void setTexture(Texture * t) {
		m_txr = t;
	}

private:
	RefPtr<Texture>		m_txr;
	int			m_w, m_h;
};

#endif	// __ROTBANNER_H

