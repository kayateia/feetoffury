/*      
   Feet of Fury
        
   saving.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __SAVING_H
#define __SAVING_H

#include "scrollybg.h"
#include "messagebox.h"

class SavingDisplay : public Drawable {
public:
	SavingDisplay(ScrollyBG * oldbg);
	virtual ~SavingDisplay();

	void startExit();
	void setFail();

private:
	RefPtr<ScrollyBG>	m_sb;
	RefPtr<MessageBox>	m_mb;
};

namespace Saving {
	bool start(ScrollyBG * oldbg);
	bool finish();
	bool fail();

	void * thdproc(void *p);

	extern RefPtr<SavingDisplay> sd;
//	extern pthread_t	thd;
	extern volatile bool	thdquit;
};

#endif	/* __SAVING_H */

