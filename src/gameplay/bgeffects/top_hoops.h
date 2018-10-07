/*      
   Feet of Fury
        
   gameplay/bgeffects/top_hoops.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __TOP_HOOPS_H
#define __TOP_HOOPS_H

#include "loadhoops.h"

// A bubble field (like char select)
class TopHoops : public TopDrawable {
public:
	TopHoops() {
		LoadHoops * lh = new LoadHoops();
		subAdd(lh);
	}
	virtual ~TopHoops() { }

	virtual void draw(ObjType list) {
		Drawable::draw(list);
	}
};

#endif	/* __TOP_HOOPS_H */

