/*
   Feet of Fury
        
   songinfo.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: songinfo.h,v 1.9 2003/05/03 19:29:44 bard Exp $
*/

#ifndef __SONGINFO_H
#define __SONGINFO_H

class StepData;

class SongInfo : public Drawable {
public:
	SongInfo(Font * fnt);
	virtual ~SongInfo();

	void drawOne(float size, float y, float r, float g, float b, const char *text, float x = 0.0f);

	// X is the _right_ edge of the stars, Y is the top edge. scale = 1.0f -> 32x32 stars.
	void drawStars(float x, float y, int cnt, float scale);
	
	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	void newSong(StepData * si, bool newsong);

	static const float	m_y_artist, m_y_name, m_y_bpm, m_y_new, m_y_diff[3];
	static const float	m_y_name_2;
	static const float	m_x_right, m_x_new, m_x_difflbl, m_y_difflbl;

private:
	int		m_frame;
	float		m_rippleidx;

	RefPtr<StepData>	m_song_info;
	bool			m_new;

	RefPtr<Font>	m_fnt;
	RefPtr<Texture>	m_txr_star;
};

#endif	/* __SONGINFO_H */

