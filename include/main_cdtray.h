/*      
   Feet of Fury
        
   main_cdtray.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __MAIN_CDTRAY_H
#define __MAIN_CDTRAY_H

// main/cdtray.cc
void cdtrayInit(); 
void cdtrayShutdown();
void cdtrayAllowOpen(bool);
int cdtrayStatus();  
void cdtraySetCallback(void (*cb)(void * token, int l_status, int c_status), void * token);

#endif	/* __MAIN_CDTRAY_H */

