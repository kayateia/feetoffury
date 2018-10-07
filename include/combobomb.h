/*      
   Feet of Fury
        
   combobomb.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: combobomb.h,v 1.8 2003/03/22 06:57:56 bard Exp $
*/

#ifndef __COMBOBOMB_H
#define __COMBOBOMB_H

// Sub-bomb -- one of these for each twinkly; we don't make these
// real drawables because it just adds unnecessary overhead.
class ComboSubBomb : public RefCnt {
public:
	ComboSubBomb();
	virtual ~ComboSubBomb();

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

// Main over-bomb
class ComboBomb : public Drawable {
public:
	ComboBomb(int index, Texture * texture);
	virtual ~ComboBomb();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 t);

	void explode();

	void reset();

private:
	int		m_index;
	RefPtr<Texture>	m_texture;
	bool		m_exploding;
	int		m_frame;
	List<ComboSubBomb>	m_bombs;
};

#endif	/* __COMBOBOMB_H */

