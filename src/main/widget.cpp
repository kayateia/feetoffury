/*
   Feet of Fury

   main/widget.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "widget.h"

Widget::Widget() {
	m_focused = false;
}

Widget::~Widget() {
}

void Widget::inputEvent(const GenericMenu::Event & evt) {
}

void Widget::setFocus(bool foc) {
	m_focused = foc;
}
