/*      
   Feet of Fury
        
   bouncetxr.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: bouncetxr.h,v 1.4 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __BOUNCETXR_H
#define __BOUNCETXR_H

class BounceTxr : public Drawable {
public:
	BounceTxr(int list, Texture * txr);
	virtual ~BounceTxr();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

	void setTexture(Texture * txr);

private:
	int	m_list;
	Texture	* m_txr;
	int	m_frame;
};

#endif	/* __BOUNCETXR_H */

