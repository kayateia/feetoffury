/*      
   Feet of Fury
        
   cube.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __CUBE_H
#define __CUBE_H

class Cube : public Drawable {
public:
	Cube(ObjType list, Texture * txr);
	virtual ~Cube();

	virtual void draw(ObjType list);

	void drawCube();

	void setTexture(Texture * newtxr);

private:
	ObjType			m_list;
	RefPtr<Texture>		m_texture;
};

#endif	/* __CUBE_H */

