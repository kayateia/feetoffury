/*      
   Feet of Fury
        
   cdtray.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: cdtray.h,v 1.10 2003/04/24 01:43:05 bard Exp $
*/

#ifndef __CDTRAY_H
#define __CDTRAY_H

class CDTray : public Drawable {
public:
	CDTray(int cnt, RefPtr<Texture> *txrs, Texture * shadow,
		Texture * shiny[2]);
	virtual ~CDTray();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 t);

	void move(int dir, int frames);
	bool isMoving() const { return m_move_dir != 0; }

	// Jumps.. only use for init!
	void setSelection(int idx);

private:
	void drawOne(int idx, float x, float y, float z, bool selected, bool shadow);

	static const float	m_cx, m_cy, m_r, m_per;

	int		m_frame;
	int		* m_cdrot;

	int		m_cnt;
	RefPtr<Texture>	* m_textures;
	RefPtr<Texture>	m_txr_shadow;
	RefPtr<Texture>	m_txr_shiny[2];

	int		m_selected, m_last_sel;
	int		m_move_dir;
	float		m_move_pf;
	float		m_move_cur;
};

#endif	/* __CDTRAY_H */
