/*      
   Feet of Fury
        
   cubecube.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __CUBECUBE_H
#define __CUBECUBE_H

#include "menu_playstyle.h"

class CubeCube : public Drawable {
public:
	CubeCube(MenuPlayStyle::MPSRes * psr, int init);
	virtual ~CubeCube();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

	void drawCube(Texture * txrs[6], const Color & col);

	void switchSelect(int idx);
	int getSelect() const;

private:
	RefPtr<MenuPlayStyle::MPSRes>	m_res;
	int				m_r;
	int				m_selected;

	Vector				m_currot, m_destrot;
};

#endif	/* __CUBECUBE_H */

