/*
   Feet of Fury

   menu_extras/multiOptionMenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_extras.h"

#include "multiOptionMenu.h"
#include "option.h"

#include "selbar.h"

MultiOptionMenu::MultiOptionMenu(MenuExtras * parent)
	: ExtrasMenu(parent)
{
	m_title = new Label(m_parent->getFont(), "Title", 36, true, false);
	m_title->setTranslate(Vector(320.0f, 60.0f, 15.0f));
	subAdd(m_title);

	m_sel = new SelBar(560.0f, 26.0f, 4.0f);
	m_sel->setTint(Color(1.0f, 1.0f, 0.0f));
	m_sel->setTranslate(Vector(320.0f, 140.0f, 1.0f));
	subAdd(m_sel);

	m_opts = NULL;
	m_optCount = 0;
	m_curOpt = 0;
}

MultiOptionMenu::~MultiOptionMenu() {
	if (m_opts)
		delete[] m_opts;
}

void MultiOptionMenu::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type !=  GenericMenu::Event::EvtKeypress)
		return;

	switch (evt.key) {
	case GenericMenu::Event::KeyCancel:
		if (m_backOpt) {
			GenericMenu::Event mevt = evt;
			mevt.key = GenericMenu::Event::KeySelect;
			m_backOpt->inputEvent(mevt);
		}
		break;
	case GenericMenu::Event::KeyUp:
		m_curOpt--;
		if (m_curOpt < 0)
			m_curOpt += m_optCount;
		m_parent->sfxCursor();
		break;
	case  GenericMenu::Event::KeyDown:
		m_curOpt++;
		if (m_curOpt >= m_optCount)
			m_curOpt -= m_optCount;
		m_parent->sfxCursor();
		break;
	default:
		m_opts[m_curOpt]->inputEvent(evt);
		break;
	}

	Vector p = m_sel->getTranslate();
	p.y = m_curOpt * 30.0f + 140.0f;
	m_sel->setTranslate(p);
}

void MultiOptionMenu::setTitle(const char * title) {
	m_title->setText(title);
}

void MultiOptionMenu::setOptionCount(int cnt) {
	if (m_opts)
		delete[] m_opts;

	m_optCount = cnt;
	m_opts = new RefPtr<Option>[cnt];
}

void MultiOptionMenu::setOption(int idx, Option * opt, bool backOption) {
	assert( idx < m_optCount );
	assert( opt != NULL );

	if (m_opts[idx])
		subRemove(m_opts[idx]);
	
	m_opts[idx] = opt;
	opt->setTranslate(Vector(0.0f, 140.0f + idx * 30, 5.0f));
	subAdd(opt);

	if (backOption)
		m_backOpt = opt;
}

int MultiOptionMenu::getOptionCount() const {
	return m_optCount;
}

Option * MultiOptionMenu::getOption(int idx) const {
	return m_opts[idx];
}

void MultiOptionMenu::rescanUnlock() {
	for (int i=0; i<m_optCount; i++) {
		m_opts[i]->rescanUnlock();
	}
}
