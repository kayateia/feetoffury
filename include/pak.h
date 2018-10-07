/*      
   Feet of Fury
        
   pak.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#ifndef __PAK_H
#define __PAK_H

// main/pak.cc
int pakLoad(const char * pakname);
int pakLoadUnencr(const char * pakname, const char * prefix);
int pakUnload(const char * pakname);

int pakMount(const char * pakname);
int pakUnmount(const char * pakname);

int pakCacheEncrypted(const char * inpfn, const char * outpfn);

FILE * pakOpenEncrypted(const char * inpfn);

// Load and mount old-skool PAK file for Swap CDs. Use
// pakUnmount to undo.
int pakLMOldSkool(const char * pakname, const char * prefix);

bool pakInit();
bool pakShutdown();

#endif	/* __PAK_H */

