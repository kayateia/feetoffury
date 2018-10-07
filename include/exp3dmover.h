/*
   Feet of Fury

   exp3dmover.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: exp3dmover.h,v 1.1 2003/03/09 03:53:49 bard Exp $
*/

#ifndef __EXP3DMOVER_H
#define __EXP3DMOVER_H

/// Logarithmic object mover on all three axes
class Exp3DMover : public Animation {
public:
	Exp3DMover(float dx, float dy, float dz, float maxx, float maxy, float maxz);
	virtual ~Exp3DMover();

	virtual void nextFrame(Drawable *t);

private:
	float	m_dx, m_dy, m_dz, m_maxx, m_maxy, m_maxz;
};

#endif	/* __EXP3DMOVER_H */
