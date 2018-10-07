/*      
   Feet of Fury
        
   tunnel.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __TUNNEL_H
#define __TUNNEL_H

// Tunnel setup
void tunnelLoadTxrs();
void tunnelInit();

// Tunnel per-frame
void tunnelDraw(float alpha);

// Tunnel shutdown
void tunnelShutdown();

#endif	/* __LOADING_H */

