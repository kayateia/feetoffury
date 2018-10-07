/*
   Feet of Fury

   menu_extras/textOption.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: textOption.h,v 1.1 2003/04/26 21:33:24 bard Exp $
*/

#ifndef __TEXT_OPTION_H
#define __TEXT_OPTION_H

#include "option.h"

// A simple option class that displays a label with the name
// of the option to the left, and a value to the right. Subclass
// to modify behavior and display.
class TextOption : public Option {
public:
	TextOption(MenuExtras * parent, const char * name, const char * value);
	virtual ~TextOption();

protected:
	RefPtr<Label>	m_name;
	RefPtr<Label>	m_value;
};

#endif	// __TEXT_OPTION_H

