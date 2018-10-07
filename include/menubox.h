/*      
   Feet of Fury
        
   menubox.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: menubox.h,v 1.2 2003/03/22 08:19:33 bard Exp $
*/

#ifndef __MENUBOX_H
#define __MENUBOX_H

class MenuBox : public Drawable {
public:
	MenuBox(bool border = true);
	virtual ~MenuBox();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

private:
	int	m_frame;
	bool	m_border;
};

#endif	/* __MENUBOX_H */

