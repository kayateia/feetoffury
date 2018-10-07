/*      
   Feet of Fury
        
   gameplay/bgeffects/top_bubblefield.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __TOP_BUBBLEFIELD_H
#define __TOP_BUBBLEFIELD_H

#include "bubblefield.h"

// A bubble field (like char select)
class TopBubbleField : public TopDrawable {
public:
	TopBubbleField(Texture * txr) {
		BubbleField * bf = new BubbleField(txr, 128);
		bf->setTranslate(Vector(0.0f, 0.0f, 0.001f));
		subAdd(bf);
	}
	virtual ~TopBubbleField() { }
};

#endif	/* __TOP_BUBBLEFIELD_H */

