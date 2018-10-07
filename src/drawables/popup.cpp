/* 
   Feet of Fury
   
   drawables/popup.cc
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "popup.h"
#include "menubox.h"
#include "selbar.h"
#include "res.h"

Popup::Popup(const char * title, bool autoRemove)
	: m_title(title, NULL)
{
	m_frame = 0;
	m_result = -1;
	m_selected = 0;
	m_width = m_height = 10;
	m_optcnt = 0;
	m_autoRemove = autoRemove;

	m_cancellable = true;

	m_scale = 0.001f;
	m_txtalpha = 0.0f;

	m_font = res.getFont("axaxax");

	m_cursor = res.getSound(Res::Preload, "menu_cursor");
	m_select = res.getSound(Res::Preload, "options_select");
	m_abort = res.getSound(Res::Preload, "options_back");

	m_box = new MenuBox();
	m_selbar = new SelBar(0.0f, 0.0f, 0.0f);
	m_selbar->setTint(Color(1.0f, 1.0f, 1.0f, 0.0f));

	// Add so we get prelative coloring
	subAdd(m_box);
	// subAdd(m_selbar);
	
	m_anim = new PopupBirth();
	animAdd(m_anim);
}

Popup::~Popup() {
}

void Popup::draw(ObjType list) {
	if (list != Trans)
		return;

	// Draw our menubox
	m_box->setScale(Vector(m_width * m_scale, m_height * m_scale, 1.0f));
	m_box->draw(list);

	// If it's not fully expanded yet, don't draw text.
	if (m_txtalpha <= 0.0f)
		return;

	Vector t = getPosition();
	Color col = getColor();
	col.a *= m_txtalpha;
	t.y -= m_height/2;
	t.z += 1.0f;

	if (col.r == 1.0f && col.g == 1.0f && col.b == 1.0f) {
		col.r = 0.85f;
		col.g = 0.85f;
		col.b = 0.85f;
	}
	
	// Draw a title, if applicable
	m_font->setAlpha(col.a);
	if (m_title.text) {
		Vector xy(m_title.x + t.x, m_title.y + t.y, t.z);
		m_font->setSize(32);
		m_font->setColor(Color(col.r*0.3f, col.g*1.0f, col.b*0.3f));
		m_font->centerCoords(m_title.text, xy);

		m_font->drawBegin();

		int len = strlen(m_title.text);
		float phase = m_frame * 2*M_PI*2 / hz;
		for (int i=0; i<len; i++) {
			Vector p;
			p = m_font->drawCharGetPos();
			m_font->drawCharSetPos(Vector(p.x, xy.y + fsin(phase) * 4.0f, t.z));
			m_font->drawChar(m_title.text[i]);

			phase = phase + 2*M_PI / len;
		}
		m_font->drawEnd();
		
		// m_font->drawCentered(m_title.x + t.x, m_title.y + t.y, t.z, m_title.text);
	}
	m_font->setSize(24);
	m_font->setColor(col);

	int idx = 0;
	ListNode<Item> * i = m_options.getHead();
	while (i) {
		const Item & item = *(i->getData());
		float w, h;

		if (item.text) {
			m_font->getTextSize(item.text, w, h);
		} else {
			// XXX
		}

		if (idx == m_selected) {
			// m_selbar->setTranslate(Vector(0.0f, item.y - m_height/2, 0.0f));
			m_selbar->setTranslate(Vector(t.x, item.y + t.y, t.z));
			m_selbar->setSize(m_width-6, h/2, h/2+3);
			m_selbar->draw(list);
		}
	
		if (item.text) {
			m_font->drawCentered(Vector(item.x + t.x, item.y + t.y, t.z), item.text);
		} else {
			// XXX
		}

		i = i->getNext();
		idx++;
	}
}

void Popup::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;
}

void Popup::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type != GenericMenu::Event::EvtKeypress)
		return;

	switch (evt.key) {

	case GenericMenu::Event::KeyUp:
		m_selected--;
		if (m_selected < 0)
			m_selected += m_optcnt;
		m_cursor->play();
		break;

	case GenericMenu::Event::KeyDown:
		m_selected++;
		if (m_selected >= m_optcnt)
			m_selected -= m_optcnt;
		m_cursor->play();
		break;

	case GenericMenu::Event::KeySelect:
		m_result = m_selected;
		startExit();
		m_select->play();
		break;

	case GenericMenu::Event::KeyCancel:
		if (m_cancellable) {
			m_result = -2;
			startExit();
			m_abort->play();
		}
		break;
	}
}

void Popup::startExit() {
	if (m_anim)
		animRemove(m_anim);
	m_anim = new PopupDeath();
	// XXX This crashes in OSX for some reason (e.g. try to exit the char select menu)
	//if (m_autoRemove)
	//	m_anim->triggerAdd(new AutoRemove());
	//else
		m_anim->triggerAdd(new Death());
	animAdd(m_anim);
}

void Popup::addOption(const char * str) {
	Item * i = new Item(str, NULL);
	m_options.insertTail(i);
	m_optcnt++;
	calcSize();
}

void Popup::addOption(Texture * txr) {
	Item * i = new Item(NULL, txr);
	m_options.insertTail(i);
	m_optcnt++;
	calcSize();
}

const char * Popup::getOptionString(int idx) {
	assert( idx >= 0 );

	ListNode<Item> * i = m_options.getHead();
	while (idx && i) {
		i = i->getNext();
		idx--;
	}

	if (i)
		return (*i)->text;
	else
		return NULL;
}

int Popup::getOptionCount() {
	return m_optcnt;
}

void Popup::calcSize() {
	float y = 16.0f;
	
	m_width = 0.0f;
	m_height = 32.0f;	// 16 pixels on top and bottom

	// If we've got a title, add it to the total
	if (m_title.text) {
		float w, h;
		m_font->setSize(32);
		m_font->getTextSize(m_title.text, w, h);
		w += 32;
		h += 4;		// wave room
		if (m_width < w)
			m_width = w;
		m_height += h + 16;

		m_title.x = 0;
		m_title.y = y + (h+8)/2;

		y += h + 16;
	}

	m_font->setSize(24);

	ListNode<Item> * i = m_options.getHead();
	while (i) {
		// Text or texture?
		if ((*i)->text != NULL) {
			// Text
			float w, h;
			m_font->getTextSize((*i)->text, w, h);
			w += 32;
			if (m_width < w)
				m_width = w;
			m_height += h + 4;

			(*i)->x = 0;
			(*i)->y = y + (h+4)/2;

			y += h+2;
		} else {
			// Texture
			Texture * t = (*i)->texture;
			float w = t->getW() + 32;
			float h = t->getH() + 2;
			if (w > m_width)
				m_width = w;
			m_height += h;
			
			(*i)->x = -(w-32)/2;
			(*i)->y = y + h/2;

			y += h+2;
		}

		// Get the next item
		i = i->getNext();
	}
}



