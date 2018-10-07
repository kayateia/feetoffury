/*      
   Feet of Fury
        
   title.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: title.h,v 1.5 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __TITLE_H
#define __TITLE_H

namespace Title {
	// Do the title screen; returns true if it timed out
	//   or false if the user hit START.
	enum Result {
		Start,
		Reset,
		Timeout
	};
	
	Result doTitle();
}

#endif	/* __TITLE_H */
