/*      
   Feet of Fury
        
   ogg.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __OGG_H
#define __OGG_H

// main/ogg.cc
int oggStart(const char * fn, bool loop, bool cache, bool queue);
int oggStop(bool cache);
void oggVolume(float vol);
float oggVolume();

#endif	/* __OGG_H */

