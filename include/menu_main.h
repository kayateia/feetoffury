/*      
   Feet of Fury
        
   menu_main.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __MENU_MAIN_H
#define __MENU_MAIN_H

// Our base class

// Forward refs
#include "topbar.h"
#include "slideshow.h"
#include "bubble_selector.h"
#include "textdisplay.h"
#include "messagebox.h"

// Our main menu class, derived from a generic menu
class MenuMain : public GenericMenu, public RefCnt {
public:
	// Constructor / Destructor ////////////////////////////////////////
	MenuMain();
	virtual ~MenuMain();

	// Methods /////////////////////////////////////////////////////////

	void processKey(Event::KeyConstant key);
	void processTach();

	// Overridden
	virtual void startExit();
	virtual void inputEvent(const Event & evt);

	// Variables ///////////////////////////////////////////////////////
	// Top bars
	RefPtr<Topbar> topbar[3];

	// Side bars
	RefPtr<Topbar> sidebar[2];

	// FoF banner
	RefPtr<Banner> fof_banner;

	// Slideshow for character pics
	RefPtr<SlideShow> slides;

	// Slideshow for the screen shots
	RefPtr<SlideShow> sshots;

	// Our font
	RefPtr<Font> font;

	// Text label for selected option
	RefPtr<Label> label;

	// Bubble selector
	RefPtr<BubbleSelector> sel;

	// Text display for menu option description
	RefPtr<TextDisplay> td;

	// Message box for keyboard warning popup
	RefPtr<MessageBox>	kbmb;

	// Sound effects
	RefPtr<Sound> sfx_select, sfx_cursor, sfx_invalid;
};

// Call this from main to do the main menu. You must pass in a pre-constructed
// main menu object.
//
// Returns -1 if the user requested an exit, -2 if we timed out,
// or a menu option index otherwise.
//
int doMenuMain(MenuMain * menu);
	
#endif	// __MENU_MAIN_H

