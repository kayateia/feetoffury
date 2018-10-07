/*      
   Feet of Fury
        
   cubetunnel.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __CUBETUNNEL_H
#define __CUBETUNNEL_H

class Cube;

class CubeTunnel : public Drawable {
public:
	CubeTunnel(Texture * txr, bool out_of_screen);
	virtual ~CubeTunnel();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	RefPtr<Texture>		m_txr;
	RefPtr<Cube>		*m_cubes;
	int			m_cc;
	bool			m_out;
};

#endif	/* __CUBE_H */

