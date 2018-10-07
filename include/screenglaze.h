/*      
   Feet of Fury
        
   screenglaze.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __SCREENGLAZE_H
#define __SCREENGLAZE_H

class ScreenGlaze : public Drawable {
public:
	ScreenGlaze() { }
	virtual ~ScreenGlaze() { }

	virtual void draw(ObjType list) {
		if (list != PLX_LIST_TR_POLY)
			return;

		plx_cxt_texture(NULL);
		plx_cxt_send(list);

		float z = getTranslate().z;
		uint32 color = getColor();

		plx_vert_inp(PLX_VERT,       0.0f, 480.0f, z, color);
		plx_vert_inp(PLX_VERT,       0.0f,   0.0f, z, color);
		plx_vert_inp(PLX_VERT,     640.0f, 480.0f, z, color);
		plx_vert_inp(PLX_VERT_EOS, 640.0f,   0.0f, z, color);
	}
};

#endif	/* __SCREENGLAZE_H */

