/*
   Feet of Fury

   menu_extras/placeholderMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: placeholderMenu.h,v 1.1 2003/04/26 21:33:24 bard Exp $
*/

#ifndef __PLACEHOLDER_MENU_H
#define __PLACEHOLDER_MENU_H

#include "widget.h"
#include "extrasMenu.h"

class PlaceholderMenu : public ExtrasMenu {
public:
	PlaceholderMenu(MenuExtras * parent, ExtrasMenu * back)
		: ExtrasMenu(parent)
	{
		m_title = new Label(parent->getFont(), "PLACEHOLDER", 36, true, false);
		m_title->setTranslate(Vector(320, 240, 0));
		subAdd(m_title);

		m_back = back;
	}
	virtual ~PlaceholderMenu() {
	}

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt) {
		m_parent->setSubMenu(m_back, false);
	}

private:
	RefPtr<Label>	m_title;
	ExtrasMenu	* m_back;
};

#endif	// __PLACEHOLDER_MENU_H

