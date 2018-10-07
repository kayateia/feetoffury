/*      
   Feet of Fury
        
   lifemeter.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __LIFEMETER_H
#define __LIFEMETER_H

class LifeMeter : public Drawable {
public:
	LifeMeter(int list, Texture * texture, bool grow_left);
	virtual ~LifeMeter();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 tm);

	void setValue(float v);
	
private:
	int		m_list;
	RefPtr<Texture>	m_texture;
	float		m_value;
	float		m_value_to, m_dvalue;
	bool		m_grow_left;

	// Internal animation goodies
	int		m_frame;
	float		m_r, m_g, m_b;
};

#endif	/* __LIFEMETER_H */

