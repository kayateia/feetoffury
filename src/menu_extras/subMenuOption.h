/*
   Feet of Fury

   menu_extras/subMenuOption.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: subMenuOption.h,v 1.2 2003/04/28 02:43:11 bard Exp $
*/

#ifndef __SUB_MENU_OPTION_H
#define __SUB_MENU_OPTION_H

#include "option.h"

class ExtrasMenu;

// Option class which activates a sub-menu of some sort. A
// text label is displayed at the previous menu level. We do NOT
// hold references to the submenus (to avoid circular refs since
// these options can go backwards as well) so make sure you do
// that somewhere.
class SubMenuOption : public Option {
public:
	SubMenuOption(MenuExtras * parent, const char * name, ExtrasMenu * target, bool moveRight)
		: Option(parent)
	{
		m_target = target;
		m_moveRight = moveRight;

		m_name = new Label(parent->getFont(), name, 26, false, false);
		m_name->setTranslate(Vector(60.0f, 0.0f, 0.0f));
		subAdd(m_name);
	}
	virtual ~SubMenuOption() {
	}

	virtual void inputEvent(const GenericMenu::Event & evt) {
		if (evt.key == GenericMenu::Event::KeySelect) {
			m_parent->setSubMenu(m_target, m_moveRight);
		}
	}

	virtual void rescanUnlock() {
		if (m_moveRight)
			m_target->rescanUnlock();
	}

protected:
	RefPtr<Label>	m_name;
	ExtrasMenu	* m_target;
	bool		m_moveRight;
};

#endif	// __TEXT_OPTION_H

