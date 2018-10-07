/*
   Feet of Fury

   menu_extras/manualMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: manualviewMenu.h,v 1.1 2003/05/06 05:03:51 bard Exp $
*/

#ifndef __MANUALVIEW_MENU_H
#define __MANUALVIEW_MENU_H

#include "extrasMenu.h"

#include "manualviewer.h"

class ManualviewMenu : public ExtrasMenu {
public:
	ManualviewMenu(MenuExtras * parent, ExtrasMenu * back, const char * srcfn);
	virtual ~ManualviewMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);
	virtual void gainFocus();

private:
	RefPtr<ManualViewer>	m_viewer;
	ExtrasMenu		* m_back;
	const char		* m_srcfn;
};

#endif	// __MANUALVIEW_MENU_H

