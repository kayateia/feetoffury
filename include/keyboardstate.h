/*
   Feet of Fury

   keyboardstate.h

   Copyright (C)2005 Cryptic Allusion, LLC
*/

#ifndef __KEYBOARDSTATE_H
#define __KEYBOARDSTATE_H


// This class will work with a Hid EventCollector to make a static
// "keyboard state" which can be polled like the old DC keyboard.

class KeyboardState : public RefCnt {
public:
	KeyboardState();
	virtual ~KeyboardState();

	bool update();
	bool isPressed(int key);
	bool isAnyKeyPressed();

private:
	void processEvent(Hid::Event & evt);

	RefPtr<Hid::EventCollector>	m_ec;
	int	m_state[512];
};


#endif	/* __KEYBOARDSTATE_H */
