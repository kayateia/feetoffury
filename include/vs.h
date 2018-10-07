/*      
   Feet of Fury
        
   vs.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __VS_H
#define __VS_H

#include "plrstate.h"
#include "starfield.h"
#include "topbar.h"

class VSDisplay : public Drawable {
public:
	VSDisplay(PlayerState * p1, PlayerState * p2);
	virtual ~VSDisplay();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	void startExit();

private:
	RefPtr<PlayerState>	m_plr[2];
	RefPtr<StarField>	m_sf;
	RefPtr<Banner>		m_banner[2];
	RefPtr<Label>		m_v, m_s;
	RefPtr<Topbar>		m_bar;

	int			m_frame;
	bool			m_needExit, m_exiting;
};

namespace VS {
	bool start(PlayerState * p1, PlayerState * p2);
	bool finish();

	void * thdproc(void *p);

	extern RefPtr<VSDisplay>	vsd;
	extern RefPtr<Thread::Thread>	thd;
	extern volatile bool		thdquit;
};

#endif	/* __VS_H */

