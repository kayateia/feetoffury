/*      
   Feet of Fury
        
   mouse.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __MOUSE_H
#define __MOUSE_H

class Mouse : public Drawable {
public:
	Mouse();
	virtual ~Mouse();

	void drawMouse();

	virtual void draw(ObjType list) {
		if (list != PLX_LIST_TR_POLY)
			return;
		drawMouse();
	}

private:
	RefPtr<Texture>		m_txr;
};

#endif	/* __MOUSE_H */

