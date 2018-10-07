/*      
   Feet of Fury
        
   starfield.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: starfield.h,v 1.2 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __STARFIELD_H
#define __STARFIELD_H

class StarField : public Drawable {
public:
	StarField(int ns);
	virtual ~StarField();

	virtual void draw(ObjType list);

private:
	void polyPnt(float x, float y, float z, float size, uint32 color);

	int		m_ns;
	Vector		* m_stars;
};

#endif	/* __STARFIELD_H */

