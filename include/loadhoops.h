/*      
   Feet of Fury
        
   loadhoops.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __LOADHOOPS_H
#define __LOADHOOPS_H

class LoadHoops : public Drawable {
public:
	LoadHoops();
	virtual ~LoadHoops();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	void	drawOuterRing();
	void	drawMidRing();
	void	drawInnerRing();
	
	int	m_frame;
};

#endif	/* __LOADHOOPS_H */

