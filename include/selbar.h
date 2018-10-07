/*
   Feet of Fury

   include/selbar.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: selbar.h,v 1.4 2003/04/24 01:43:05 bard Exp $
*/

#ifndef __SELBAR_H
#define __SELBAR_H

class SelBar : public Drawable {
public:
	SelBar(float xsize, float yup, float ydown)
		: m_xsize(xsize), m_yup(yup), m_ydown(ydown) { }
	virtual ~SelBar() { }

	void setSize(float x, float yup, float ydown) {
		m_xsize = x;
		m_yup = yup;
		m_ydown = ydown;
	}

	virtual void draw(ObjType list) {
		if (list != Trans)
			return;

		plx_cxt_texture(NULL);
		plx_cxt_send(list);

		plx_dr_state_t dr;
		plx_dr_init(&dr);

		Vector pos = getPosition(); pos.z -= 0.01f;
		Color c = getColor(); c.a *= 0.8f;
		float xs = m_xsize/2;

		plx_vert_fnd(&dr, PLX_VERT, pos.x - xs, pos.y+m_ydown, pos.z, c.a, c.r*0.8f, c.g*0.8f, c.b*0.8f);
		plx_vert_fnd(&dr, PLX_VERT, pos.x - xs, pos.y-m_yup, pos.z, c.a, c.r*0.3f, c.g*0.3f, c.b*0.3f);
		plx_vert_fnd(&dr, PLX_VERT, pos.x + xs, pos.y+m_ydown, pos.z, c.a, c.r*0.8f, c.g*0.8f, c.b*0.8f);
		plx_vert_fnd(&dr, PLX_VERT_EOS, pos.x + xs, pos.y-m_yup, pos.z, c.a, c.r*0.3f, c.g*0.3f, c.b*0.3f);
	}

	virtual void nextFrame(uint64 tm) {
		Drawable::nextFrame(tm);
	}
private:
	float	m_xsize, m_yup, m_ydown;
};


#endif	// __SELBAR_H
