/*      
   Feet of Fury
        
   gamemode.h

   Copyright (C)2002,2003,2005 Cryptic Allusion, LLC
*/

#ifndef __GAMEMODE_H
#define __GAMEMODE_H

class GameMode : public Drawable {
public:
	GameMode();
	virtual ~GameMode();

	virtual void draw(ObjType list) {
		if (list != Trans)
			return;

		drawGamemode();
	}

	virtual void nextFrame(uint64 tm);

	// Change modes
	void setMode(int mode);

private:
	void drawGamemode();

	float	m_xpos, m_ypos;
	float	m_xvel;
	int	m_mode, m_modeto;
	
	RefPtr<Texture>		m_txrs[2];
};

#endif	/* __GAMEMODE_H */

