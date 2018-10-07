/*
   Feet of Fury

   menu_extras/extrasMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: extrasMenu.h,v 1.3 2003/04/28 02:43:11 bard Exp $
*/

#ifndef __EXTRAS_MENU_H
#define __EXTRAS_MENU_H

#include "widget.h"

class MenuExtras;

// Define one of these per menu in the extras/options menu area. This is
// just a generic base class -- a specific subclass will implement the
// actual menu handling.
class ExtrasMenu : public Drawable, public Widget {
public:
	ExtrasMenu(MenuExtras * parent) {
		m_parent = parent;
	}
	virtual ~ExtrasMenu() { }

	virtual void gainFocus() { }

	virtual void rescanUnlock() { }

protected:
	MenuExtras	* m_parent;
};

#endif	// __EXTRAS_MENU_H

