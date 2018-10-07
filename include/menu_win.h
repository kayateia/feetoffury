/*      
   Feet of Fury
        
   menu_win.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __MENU_WIN_H
#define __MENU_WIN_H

class PlayerState;

// Call this to do the winner notification screen. Pass in a
// PlayerState object for the winning player and the index of
// the player that won.
void doMenuWin(int index, PlayerState * winner);
	
#endif	/* __MENU_WIN_H */

