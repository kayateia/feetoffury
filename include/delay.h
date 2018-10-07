/*
   Feet of Fury

   delay.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: delay.h,v 1.1 2003/04/07 06:59:15 bard Exp $
*/

#ifndef __DELAY_H
#define __DELAY_H

class DelayAnim : public Animation {
public:
	DelayAnim(int frames) {
		m_frames = frames;
		m_frame = 0;
	}

	void nextFrame(Drawable * t) {
		if (m_frame < m_frames) {
			m_frame++;
		} else {
			complete(t);
		}
	}

private:
	int	m_frame, m_frames;
};


#endif	/* __DELAY_H */

