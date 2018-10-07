/*
   Feet of Fury

   main/plrgenmenu.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "plrgenmenu.h"

// XXX
// PlayerGenericMenu::PlayerGenericMenu(Event::TypeConstant contType, int plrmax) {
PlayerGenericMenu::PlayerGenericMenu() {
//	m_contType = contType;
//	m_plrMax = plrmax;
	
//	m_plrCnt = 0;
	for (int i=0; i<4; i++)
		m_portToPlayer[i] = -1;
}

PlayerGenericMenu::~PlayerGenericMenu() {
}

/* void PlayerGenericMenu::setDeviceType(Event::TypeConstant contType) {
	m_contType = contType;
} */

void PlayerGenericMenu::setPlayerCount(int plrmax) {
	m_plrMax = plrmax;
}

void PlayerGenericMenu::inputEvent(const Event & evt) {
	// GenericMenu::inputEvent(evt);

	switch (evt.type) {

	// A key was pressed
	case Event::EvtKeypress: {
		// Reset the timeout timer
		resetTimeout();

		// Figure out which player hit stuff and make sure we actually care
// XXX
#if 0
		int port = evt.port;
		assert( port >= 0 && port <= 3 );
		int pidx = m_portToPlayer[port];
		if (pidx == -1) return;
#endif
		int pidx = 0;

		processKeypress(pidx, (Event::KeyConstant)evt.key);
		
		break;
	}

	// Someone plugged in a device
	case Event::EvtAttach:
		processPeriph();
		break;

	// Someone unplugged a device
	case Event::EvtDetach:
		processPeriph();
		break;
	}
}

void PlayerGenericMenu::processKeypress(int player, Event::KeyConstant key) {
}

void PlayerGenericMenu::processPeriph() {
	// Reset our mapping array
	for (int i=0; i<4; i++)
		m_portToPlayer[i] = -1;

	// Redo mappings and count players again
	m_plrCnt = 0;
	for (int i=0, j=0; i<4; i++) {
		// XXX
#if 0
		if (m_contType == Event::TypeAny || getType(i) == m_contType) {
			m_portToPlayer[i] = j;
			j++;
			m_plrCnt++;
			if (j >= m_plrMax)
				break;
		}
#endif
	}

	resetTimeout();
}

