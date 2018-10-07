/*
   Feet of Fury

   menu_extras/manualviewMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_extras.h"

#include "manualviewMenu.h"

#include "manualviewer.h"

class MBackplane : public Drawable {
public:
	MBackplane() { }
	virtual ~MBackplane() { }

	virtual void draw(ObjType list) {
		Color t = getColor();
		
		if (t.a == 1.0f) {
			if (list != Opaque) return;
		} else {
			if (list != Trans) return;
		}

		plx_cxt_texture(NULL);

		Vector p = getPosition();

		plx_vert_inp(PLX_VERT, 0.0f+p.x, 480.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT, 0.0f+p.x, 0.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT, 640.0f+p.x, 480.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT_EOS, 640.0f+p.x, 0.0f+p.y, p.z, t);
	}
};

ManualviewMenu::ManualviewMenu(MenuExtras * parent, ExtrasMenu * back, const char * srcfn)
	: ExtrasMenu(parent)
{
	assert( srcfn != NULL );
	m_srcfn = srcfn;
	m_back = back;
	
	MBackplane * bkg = new MBackplane();
	bkg->setTint(Color(1, 38/255.0f, 12/255.0f, 63/255.0f));
	bkg->setTranslate(Vector(0,0,10));
	subAdd(bkg);
}

ManualviewMenu::~ManualviewMenu() {
}

void ManualviewMenu::gainFocus() {
// XXX
#if 0
	m_viewer = new ManualViewer(m_srcfn);
	m_viewer->setTranslate(Vector(0,0,25));
	subAdd(m_viewer);

	m_parent->setAutoRepeat(GenericMenu::Event::KeyUp, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeyDown, true);
#endif
}

void ManualviewMenu::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

// XXX
#if 0
	float o;
	switch (evt.key) {
	case GenericMenu::Event::KeyUp:
		o = m_viewer->getOffset();
		o -= 8;
		if (o < 0.0f)
			o = 0.0f;
		m_viewer->setOffset(o);
		break;
	case GenericMenu::Event::KeyDown:
		o = m_viewer->getOffset();
		o += 8;
		m_viewer->setOffset(o);
		break;
	case GenericMenu::Event::KeyCancel:
		subRemove(m_viewer);
		m_viewer = NULL;
		m_parent->setAutoRepeat(GenericMenu::Event::KeyUp, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeyDown, false);
		m_parent->setSubMenu(m_back, false);
		return;
	}
#endif
}
