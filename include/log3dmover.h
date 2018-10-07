/*
   Feet of Fury

   log3dmover.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: log3dmover.h,v 1.1 2003/03/09 03:53:50 bard Exp $
*/

#ifndef __LOG3DMOVER_H
#define __LOG3DMOVER_H

/// Logarithmic object mover on all three axes
class Log3DMover : public Animation {
public:
	Log3DMover(float dstx, float dsty, float dstz);
	virtual ~Log3DMover();

	virtual void nextFrame(Drawable *t);

private:
	float	m_dstx, m_dsty, m_dstz;
};

#endif	/* __LOG3DMOVER_H */
