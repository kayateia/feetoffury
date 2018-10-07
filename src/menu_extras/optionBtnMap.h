/*
   Feet of Fury

   menu_extras/optionBtnMap.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: optionBtnMap.h,v 1.1 2003/05/04 00:12:43 bard Exp $
*/

#ifndef __OPTION_BTN_MAP_H
#define __OPTION_BTN_MAP_H

#include "textOption.h"

// This internal class will represent our "hit the desired button" screen.
class OptBtnMap;
class HitBtnMenu : public ExtrasMenu {
public:
	HitBtnMenu(OptBtnMap * parent);
	virtual ~HitBtnMenu();

	virtual void inputEvent(const GenericMenu::Event & evt);

	OptBtnMap * m_parent;
};


// A simple option class that displays a label with the name
// of the option to the left, and a value to the right. Subclass
// to modify behavior and display.
class OptBtnMap : public TextOption {
public:
	OptBtnMap(MenuExtras * parent, ExtrasMenu * mom, const char * desc, uint32 * setting);
	virtual ~OptBtnMap();

	virtual void inputEvent(const GenericMenu::Event & evt);

	virtual void rescanUnlock();

	MenuExtras * getParent() const;
	void updateBuffers();
	void finished();

	ExtrasMenu		* m_mom;
	uint32			* m_setting;
	RefPtr<HitBtnMenu>	m_submenu;
};

#endif	// __OPTION_BTN_MAP_H

