/*
   Feet of Fury

   menu_extras/textOption.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_extras.h"
#include "scrollybg.h"

#include "textOption.h"
#include "extrasMenu.h"

TextOption::TextOption(MenuExtras * parent, const char * name, const char * value)
	: Option(parent)
{
	m_name = new Label(parent->getFont(), name, 26, false, false);
	m_name->setTranslate(Vector(60.0f, 0.0f, 0.0f));
	subAdd(m_name);

	m_value = new Label(parent->getFont(), value, 24, false, false);
	m_value->setTranslate(Vector(320.0f, 0.0f, 0.0f));
	subAdd(m_value);
}

TextOption::~TextOption() {
}
