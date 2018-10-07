/*
   Feet of Fury

   menu_extras/multiOptionMenu.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: multiOptionMenu.h,v 1.3 2003/05/04 00:12:43 bard Exp $
*/

#ifndef __MULTI_OPTION_MENU_H
#define __MULTI_OPTION_MENU_H

#include "widget.h"
#include "extrasMenu.h"

#include "option.h"
#include "selbar.h"

// Define one of these per menu in the extras/options menu area. This is
// just a generic base class -- a specific subclass will implement the
// actual menu handling.
class MultiOptionMenu : public ExtrasMenu {
public:
	MultiOptionMenu(MenuExtras * parent);
	virtual ~MultiOptionMenu();

	// Overrides
	virtual void inputEvent(const GenericMenu::Event & evt);
	virtual void rescanUnlock();

	// Set a title string (must be persistent elsewhere)
	void setTitle(const char * title);

	// Call to set the number of options
	void setOptionCount(int n);

	// Get the number of options
	int getOptionCount() const;

	// Call to set an option by index. Set true on backOption for
	// this to be the default action if the user presses Cancel.
	void setOption(int idx, Option * opt, bool backOption = false);

	// Get the requested option by index
	Option * getOption(int idx) const;

private:
	RefPtr<Label>	m_title;
	RefPtr<Option>	* m_opts;
	RefPtr<Option>	m_backOpt;
	RefPtr<SelBar>	m_sel;
	int		m_optCount;
	int		m_curOpt;
};

#endif	// __MULTI_OPTION_MENU_H

