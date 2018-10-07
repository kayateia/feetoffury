/*      
   Feet of Fury
        
   combometer.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: combometer.h,v 1.4 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __COMBOMETER_H
#define __COMBOMETER_H

class ComboMeter : public Drawable {
public:
	ComboMeter(int list, Texture * texture);
	virtual ~ComboMeter();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 tm);

	float getValue() const { return m_value; }
	void setValue(float v) { m_value = v; }

private:
	int		m_list;
	RefPtr<Texture>	m_texture;
	float		m_value;

	// Internal animation goodies
	int		m_frame;
	float		m_spec_ptr;
	float		m_r, m_g, m_b;
};

#endif	/* __COMBOMETER_H */

