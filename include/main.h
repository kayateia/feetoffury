/*      
   Feet of Fury
        
   main.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __MAIN_H
#define __MAIN_H

#include "stepdata.h"

// main/misc.cc  
extern bool swapcd_enabled;
extern char swapcd_path[];
void bail_handler(uint8 addr, uint32 btns);
StepData * loadSteps(const char * fn);

void loadInitial();
void loadFull();
void loadFullComplete();

int globalSetup();
int globalShutdown();


// main/main.cc




// Menus that can exit should use these result codes to specify what
// happened in the exiting. Ok specifies that everything is good to
// proceed, any of the others should send the user back to the
// specified location. Note that certain return values are only
// valid in certain situations. Like the character menu can never
// specify a return to the gameplay, and any menu can never specify
// a return to the Swap CD menu if a Swap CD isn't active.
namespace Main {

enum Result {
	Ok,		// All good
	Timeout,	// User sat doing nothing for too long
	Title,		// Send us back to the title (soft reset)
	MenuMain,	// Send us back to the main menu
	MenuSwapCD,	// Send us back to the Swap CD menu
	MenuChar,	// Send us back to the character select menu
	MenuStage,	// Send us back to the song select menu
	Gameplay	// Used to restart a song already in progress
};

};

#endif	/* __MAIN_H */

