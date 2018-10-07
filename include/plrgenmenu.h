/*      
   Feet of Fury
        
   plrgenmenu.h

   Copyrignt Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __PLRGENMENU_H
#define __PLRGENMENU_H

/* This is a slight modification of the generic menu system that
   implements some player-specific key handling. Basically your
   constructor should define what sort of devices you're looking for
   per player and a max number of players, and processKeypress() will
   be called with a player index instead of a port index whenever a
   particular player presses a key. You can still override inputEvent(),
   but make sure to call the base class! */

class PlayerGenericMenu : public GenericMenu {
public:
	// Constructor / destructor
	// XXX
	//PlayerGenericMenu(Event::TypeConstant contType = Event::TypeAny, int plrmax = 4);
	PlayerGenericMenu();
	virtual ~PlayerGenericMenu();

protected:
	// Set our device type if it didn't get set on the constructor. Note
	// that this really isn't effective after you've already started accepting
	// input. It needs to be done before the first use still.
	//void setDeviceType(Event::TypeConstant contType);

	// Set our maximum player count if it didn't get set on the constructor
	void setPlayerCount(int plrmax);

	// We'll hook this and demangle things into player slots
	virtual void inputEvent(const Event & evt);

	// Override this to receive per-player events
	virtual void processKeypress(int player, Event::KeyConstant key);

	// Handles attach/detach events
	virtual void processPeriph();


	// Controller type we'll be looking for
	// Event::TypeConstant	m_contType;

	// Maximum number of players to allow
	int		m_plrMax;

	// Number of players currently attached
	int		m_plrCnt;

	// Port-to-player mappings
	int		m_portToPlayer[4];
};

#endif	/* __PLRGENMENU_H */

