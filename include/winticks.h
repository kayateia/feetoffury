/*      
   Feet of Fury
        
   winticks.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __WINTICKS_H
#define __WINTICKS_H

class WinTicks : public Drawable {
public:
	WinTicks(Texture * texture, bool left_side);
	virtual ~WinTicks();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 tm);

	int getWins() const { return m_wins; }
	void setWins(int v) { m_wins = v; }

private:
	RefPtr<Texture>	m_texture;
	int		m_wins;
	bool		m_left;
};

#endif	/* __WINTICKS_H */

