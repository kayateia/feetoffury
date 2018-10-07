/*
   Feet of Fury

   gameplay/keyboardstate.cpp

   Copyright (C)2005 Cryptic Allusion, LLC
*/

#include "global.h"
#include "keyboardstate.h"


KeyboardState::KeyboardState() {
	memset(m_state, 0, 512);
	m_ec = new Hid::EventCollector();
}

KeyboardState::~KeyboardState() {
}

void KeyboardState::processEvent(Hid::Event & evt) {
	if (evt.key > 512) {
		Debug::printf("processEvent: warning -- key %d > 512\n", evt.key);
		return;
	}
	
	if (evt.type == Hid::Event::EvtKeyDown)
		m_state[evt.key] = 1;
	else if (evt.type == Hid::Event::EvtKeyUp)
		m_state[evt.key] = 0;
}

bool KeyboardState::update() {
	Hid::Event evt;
	
	if (!m_ec->getEvent(evt))
		return false;
		
	if (evt.type == Hid::Event::EvtQuit)
		return false;
	
	if (evt.type == Hid::Event::EvtKeyUp || evt.type == Hid::Event::EvtKeyDown) {
		processEvent(evt);
	}

	return true;
}

bool KeyboardState::isPressed(int key) {
	return !!m_state[key];
}

bool KeyboardState::isAnyKeyPressed() {
	for (int i=0; i<512; i++)
		if (m_state[i])
			return true;
	return false;
}
