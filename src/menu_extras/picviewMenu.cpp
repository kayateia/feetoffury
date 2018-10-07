/*
   Feet of Fury

   menu_extras/picviewMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_extras.h"
#include "unlock.h"

#include "picviewMenu.h"
#include "multiOptionMenu.h"

#include "delay.h"
#include "messagebox.h"

class Backplane : public Drawable {
public:
	Backplane() { }
	virtual ~Backplane() { }

	virtual void draw(ObjType list) {
		Color t = getColor();
		
		if (t.a == 1.0f) {
			if (list != Opaque) return;
		} else {
			if (list != Trans) return;
		}

		plx_cxt_texture(NULL);
		plx_cxt_send(list);

		Vector p = getPosition();

		plx_vert_inp(PLX_VERT, 0.0f+p.x, 480.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT, 0.0f+p.x, 0.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT, 640.0f+p.x, 480.0f+p.y, p.z, t);
		plx_vert_inp(PLX_VERT_EOS, 640.0f+p.x, 0.0f+p.y, p.z, t);
	}
};

PicviewMenu::PicviewMenu(MenuExtras * parent, ExtrasMenu * back, const char * srcfn, const char **msg, int msgcnt)
	: ExtrasMenu(parent)
{
	assert( srcfn != NULL );

	char buf[1024];
	// XXX
	// char buf[128 + strlen(srcfn)];
	// sprintf(buf, "/pd%s/extras/%s", fs_getwd(), srcfn);
	Debug::printf("picview: loading pic '%s'\n", buf);

	m_back = back;
	
	m_txrPic = new Texture();
	m_txrPic->loadFromFile(buf, false);

	m_bkg = new Backplane();
	m_bkg->setTint(Color(1,0,0,0));
	m_bkg->setTranslate(Vector(0,0,10));
	subAdd(m_bkg);

	m_pic = new Banner(Opaque, m_txrPic);
	m_pic->setSize(440.0f, 440.0f);
	m_pic->setTint(Color(0.85f,0.85f,0.85f));
	m_pic->setTranslate(Vector(320,240,15));
	subAdd(m_pic);

	m_zoom = 1.0f;
	m_xoffs = m_yoffs = 0.0f;

	if (!m_parent->shownPicDirs()) {
		Label * lbl = new Label(m_parent->getFont(), "Press Y for more info for this", 24, true, true);
		lbl->setTranslate(Vector(320,240-24,18));
		subAdd(lbl);

		// Delay for 4 seconds, fade out in one second, then kill the label
		DelayAnim * del = new DelayAnim(hz*4);
		lbl->animAdd(del);
		AlphaFader * af = new AlphaFader(0.0f, -1.0f/hz);
		ChainAnimation * ch = new ChainAnimation(af);
		del->triggerAdd(ch);
		af->triggerAdd(new Death());

		lbl = new Label(m_parent->getFont(), "or any other art unlockable", 24, true, true);
		lbl->setTranslate(Vector(320, 240, 18));
		subAdd(lbl);

		del = new DelayAnim(hz*4);
		lbl->animAdd(del);
		af = new AlphaFader(0.0f, -1.0f/hz);
		ch = new ChainAnimation(af);
		del->triggerAdd(ch);
		af->triggerAdd(new Death());

		lbl = new Label(m_parent->getFont(), "X to zoom in, A to zoom out", 24, true, true);
		lbl->setTranslate(Vector(320, 240+24, 18));
		subAdd(lbl);

		del = new DelayAnim(hz*4);
		lbl->animAdd(del);
		af = new AlphaFader(0.0f, -1.0f/hz);
		ch = new ChainAnimation(af);
		del->triggerAdd(ch);
		af->triggerAdd(new Death());

		m_parent->setPicDirsShown();
	}

// XXX
#if 0
	m_parent->setAutoRepeat(GenericMenu::Event::KeyUp, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeyDown, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeyLeft, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeyRight, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeyMiscX, true);
	m_parent->setAutoRepeat(GenericMenu::Event::KeySelect, true);
#endif

	m_msg = msg;
	m_msgCnt = msgcnt;
}

PicviewMenu::~PicviewMenu() {
}

void PicviewMenu::redo() {
	if (m_zoom < 0.7f)
		m_zoom = 0.7f;
	if (m_zoom > 4.0f)
		m_zoom = 4.0f;
	m_pic->setSize(440.0f * m_zoom, 440.0f * m_zoom);

	if (m_xoffs < -440.0f)
		m_xoffs = -440.0f;
	if (m_xoffs > 440.0f)
		m_xoffs = 440.0f;
	if (m_yoffs < -440.0f)
		m_yoffs = -440.0f;
	if (m_yoffs > 440.0f)
		m_yoffs = 440.0f;
	m_pic->setTranslate(Vector(320+m_xoffs*m_zoom, 240+m_yoffs*m_zoom, 15));
}

void PicviewMenu::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

	if (m_msgbox) {
		m_msgbox->startExit();
		m_msgbox = NULL;
		return;
	}

	switch (evt.key) {
// XXX
#if 0
	case GenericMenu::Event::KeyMiscX:
		m_zoom += 1.5f/(hz*2);
		redo();
		break;
#endif

	case GenericMenu::Event::KeySelect:
		m_zoom -= 1.5f/(hz*2);
		redo();
		break;

	case GenericMenu::Event::KeyLeft:
		m_xoffs += 512.0f/(hz*2);
		redo();
		break;
	
	case GenericMenu::Event::KeyRight:
		m_xoffs -= 512.0f/(hz*2);
		redo();
		break;
	
	case GenericMenu::Event::KeyUp:
		m_yoffs += 512.0f/(hz*2);
		redo();
		break;
	
	case GenericMenu::Event::KeyDown:
		m_yoffs -= 512.0f/(hz*2);
		redo();
		break;

// XXX
#if 0	
	case GenericMenu::Event::KeyMiscY:
		m_msgbox = new MessageBox(20, m_parent->getFont());
		m_msgbox->setTranslate(Vector(320,240,30));
		m_msgbox->setText(m_msg, m_msgCnt);
		subAdd(m_msgbox);
		break;
#endif

	case GenericMenu::Event::KeyCancel:
		m_zoom = 1.0f;
		m_xoffs = m_yoffs = 0.0f;
		redo();

		m_parent->setSubMenu(m_back, false);

// XXX
#if 0
		m_parent->setAutoRepeat(GenericMenu::Event::KeyUp, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeyDown, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeyLeft, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeyRight, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeyMiscX, false);
		m_parent->setAutoRepeat(GenericMenu::Event::KeySelect, false);
#endif
		return;
	}
}
