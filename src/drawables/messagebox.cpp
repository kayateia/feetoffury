/* 
   Feet of Fury
   
   drawables/messagebox.cc
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "messagebox.h"
#include "menubox.h"
#include "res.h"
#include "textdisplay.h"

MessageBox::MessageBox(float ps, Font * fnt) {
	m_width = m_height = 10;

	m_scale = 0.001f;
	m_txtalpha = 0.0f;

	m_box = new MenuBox();
	subAdd(m_box);

	if (!fnt)
		fnt = res.getFont("axaxax");

	m_td = new TextDisplay(fnt, ps, true);
	m_td->setTranslate(Vector(0,0,1));
	m_td->setTint(Color(1,0.85f,0.85f,0.85f));
	subAdd(m_td);
	
	m_anim = new PopupBirth();
	animAdd(m_anim);
}

MessageBox::~MessageBox() {
}

void MessageBox::setText(const char **text, int linecnt) {
	m_td->setText(text, linecnt);
	m_td->getSize(m_width, m_height);
	m_width += 48;
	m_height += 48;
}

void MessageBox::draw(ObjType list) {
	if (list != Trans)
		return;

	// Draw our menubox
	m_box->setScale(Vector(m_width * m_scale, m_height * m_scale, 1.0f));
	m_box->draw(list);

	// If it's not fully expanded yet, don't draw text.
	if (m_txtalpha > 0.0f) {
		m_td->setTint(getColor() * Color(m_txtalpha, 1, 1, 1));
		m_td->draw(list);
	}
}

void MessageBox::startExit() {
	if (m_anim)
		animRemove(m_anim);
	m_anim = new PopupDeath();
	m_anim->triggerAdd(new Death());
	animAdd(m_anim);
}


