/*
   Feet of Fury

   menu_extras/disabledOption.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: disabledOption.h,v 1.1 2003/04/28 02:43:11 bard Exp $
*/

#ifndef __DISABLED_OPTION_H
#define __DISABLED_OPTION_H

#include "option.h"

class ExtrasMenu;

class DisabledOption : public Option {
public:
	DisabledOption(MenuExtras * parent, const char * name)
		: Option(parent)
	{
		m_name = new Label(parent->getFont(), name, 26, false, false);
		m_name->setTranslate(Vector(60.0f, 0.0f, 0.0f));
		m_name->setTint(Color(0.5f,0.5f,0.5f));
		subAdd(m_name);
	}
	virtual ~DisabledOption() {
	}

	virtual void inputEvent(const GenericMenu::Event & evt) {
		if (evt.key == GenericMenu::Event::KeySelect) {
			m_parent->sfxInvalid();
		}
	}

protected:
	RefPtr<Label>	m_name;
};

#endif	// __DISABLED_OPTION_H

