/*
   Feet of Fury

   menu_extras/option.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: option.h,v 1.2 2003/04/28 02:43:11 bard Exp $
*/

#ifndef __OPTION_H
#define __OPTION_H

#include "widget.h"

class MenuExtras;

// Submenus are made up of these. They are basically really
// simple widgets with a well-defined drawable interface.
class Option : public Drawable, public Widget {
public:
	Option(MenuExtras * parent) {
		m_parent = parent;
	}
	virtual ~Option() { }

	virtual void rescanUnlock() { }

protected:
	MenuExtras	* m_parent;
};

#endif	// __OPTION_H

