/*      
   Feet of Fury
        
   menu_options.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __MENU_OPTIONS_H
#define __MENU_OPTIONS_H

// Our base class
#include "genmenu.h"

// Forward decls
class ScrollyBG;
class Option;
class SelBar;

// Number of options
#define OPTION_COUNT 9

// Put everything in a class wrapper to make the ref counting easier
class MenuOptions : public GenericMenu {
public:
	// Methods /////////////////////////////////////////////////////////
	MenuOptions();
	virtual ~MenuOptions();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);
	virtual void visualPerFrame();

	// Set a popup to have focus for keyboard and visuals
	void setPopup(Option * target);
	void removePopup();

	// Get some internals for options popups
	Font * getFont() const { return font; }
	Sound * getSfxSelect() const { return sfx_select; }
	Sound * getSfxUnselect() const { return sfx_unselect; }

	// Final results
	int getSelOpt() const { return sel_opt; }

private:
	// Variables ///////////////////////////////////////////////////////

	// Our font
	RefPtr<Font> font;

	// Sound effects
	RefPtr<Sound> sfx_select, sfx_cursor, sfx_unselect, sfx_invalid;

	// Currently selected option
	int		sel_opt;

	//////////////////////////////

	// Our scrollybg
	RefPtr<ScrollyBG> scrolly;

	//////////////////////////////

	// Second-level scene containing all the options
	RefPtr<Drawable> opt_scene;

	// Text labels for titles
	RefPtr<Label> label;

	// Option handlers
	RefPtr<Option>	options[OPTION_COUNT];

	// Labels for options
	RefPtr<Label>	opt_labels[OPTION_COUNT][2];

	// Selection bar
	RefPtr<SelBar>	sel_bar;

	// If we have a popup active, this will point to it.
	RefPtr<Option>	active_popup;
};


// Call this from main to do the options menu. Returns 0
// if everything is cool, returns -2 on timeout.
int doMenuOptions();
	
#endif	/* __MENU_OPTIONS_H */

