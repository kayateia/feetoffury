/*
   Feet of Fury

   soundfader.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: soundfader.h,v 1.1 2003/03/22 06:57:56 bard Exp $
*/

#ifndef __SOUNDFADER_H
#define __SOUNDFADER_H

/// Smoothly ramps the BGM volume. Attach to Scene.
class SoundFader : public Animation {
public:
	SoundFader(float dstvol, float dv);
	virtual ~SoundFader();

	virtual void nextFrame(Drawable *t);

private:
	float	m_dstvol, m_dv;
};

#endif	/* __SOUNDFADER_H */
