/*      
   Feet of Fury
        
   gameplay/bgeffects/top_cubetunnel.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __TOP_CUBETUNNEL_H
#define __TOP_CUBETUNNEL_H

#include "cubetunnel.h"

// A bubble field (like char select)
class TopCubeTunnel : public TopDrawable {
public:
	TopCubeTunnel(Texture * txr, bool out) {
		CubeTunnel * ct = new CubeTunnel(txr, out);
		subAdd(ct);
	}
	virtual ~TopCubeTunnel() { }

	virtual void draw(ObjType list) {
		Drawable::draw(list);
	}
};

#endif	/* __TOP_CUBETUNNEL_H */

