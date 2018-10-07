/* 
   Feet of Fury
   
   drawables/mouse.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"

#if 0
/* XPM: 16x16 */
static char * mouse1_xpm =
"..              "
".+.             "
".++.            "
".+++.           "
".++++.          "
".+++++.         "
".++++++.        "
".+++++++.       "
".++++++++.      "
".+++++...       "
".++.++.         "
".+. .++.        "
" .  .++.        "
"     .++.       "
"     .++.       "
"      ..        ";

Mouse::Mouse() {
	int		x, y;
	uint16		txr[16*16];
	pvr_ptr_t	pp;

	// Build an ARGB1555 texture
	for (y=0; y<16; y++) {
		for (x=0; x<16; x++) {
			switch (mouse1_xpm[(15-y)*16+x]) {
			case ' ':
				txr[y*16+x] = 0; break;
			case '.':
				txr[y*16+x] = 0xffff; break;
			case '+':
				txr[y*16+x] = 0xf000; break;
			}
		}
	}

	// Load it up
	pp = pvr_mem_malloc(16*16*2);
	pvr_txr_load_ex(txr, pp, 16, 16, PVR_TXRLOAD_16BPP);
	
	glGenTextures(1, &m_txr);
	glBindTexture(GL_TEXTURE_2D, m_txr);
	glKosTex2D(GL_ARGB4444_TWID, 16, 16, pp);

	// Initial mouse position
	m_tx = 600;
	m_ty = 240;
	m_tz = 99.99;
}

Mouse::~Mouse() {
	glDeleteTextures(1, &m_txr);
}

// Assumes a glOrtho projection setup
void Mouse::draw_mouse() {
	// Update the mouse position if necessary
	maple_device_t	*addr;
	cont_state_t	*cst;
	mouse_state_t	*mst;
	int		i;

	i = 0;
	while ( (addr = maple_enum_type(i++, MAPLE_FUNC_CONTROLLER)) ) {
		cst = (cont_state_t *)maple_dev_status(addr);
		m_tx += cst->joyx / 16;
		m_ty += cst->joyy / 16;
	}
	
	i = 0;
	while ( (addr = maple_enum_type(i++, MAPLE_FUNC_MOUSE)) ) {
		mst = (mouse_state_t *)maple_dev_status(addr);
		m_tx += mst->dx;
		m_ty += mst->dy;
	}
	
	if (m_tx < 0) m_tx = 0;
	if (m_ty < 0) m_ty = 0;
	if (m_tx >= 640) m_tx = 639;
	if (m_ty >= 480) m_ty = 479;

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, m_txr);
	glColor4f(1.0f, 1.0f, 1.0f, m_alpha);

	glLoadIdentity();
	glBegin(GL_QUADS);
		/* Front Face */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(m_tx +0.0f, m_ty+ 0.0f, m_tz);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(m_tx +0.0f, m_ty+16.0f, m_tz);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(m_tx+16.0f, m_ty+16.0f, m_tz);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(m_tx+16.0f, m_ty+ 0.0f, m_tz);
	glEnd();
}
#endif

