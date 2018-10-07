/*      
   Feet of Fury
        
   particlestorm.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: particlestorm.h,v 1.1 2003/04/07 06:59:15 bard Exp $
*/

#ifndef __PARTICLESTORM_H
#define __PARTICLESTORM_H

// Sub-bomb -- one of these for each twinkly; we don't make these
// real drawables because it just adds unnecessary overhead.
class SubPart : public RefCnt {
public:
	SubPart();
	virtual ~SubPart();

	virtual void draw();

	virtual void nextFrame(uint64 tm);

	void recycle(float x, float y, float z);

	void explode();

	void setVel(float x, float y);
	void setColor(const Color & c) { m_color = c; }
	const Color & getColor() { return m_color; }

private:
	float		m_vx, m_vy;
	float		m_tx, m_ty, m_tz;
	float		m_sc;
	Color		m_color;
};

class ParticleStorm : public Drawable {
public:
	ParticleStorm(Texture * texture, int cnt);
	virtual ~ParticleStorm();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 t);

	void explode();

	void reset();

private:
	RefPtr<Texture>	m_texture;
	bool		m_exploding;
	int		m_frame;
	List<SubPart>	m_bombs;
};

#endif	/* __PARTICLESTORM_H */

