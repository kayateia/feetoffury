/*      
   Feet of Fury
        
   tube.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __TUBE_H
#define __TUBE_H

#define TUBE_STEPS 16
#define TUBE_RINGS 30

class Tube : public Drawable {
public:
	Tube(int list, Texture * txr);
	virtual ~Tube();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	void	palInit();
	void	palStep();

	void	circlesGenRingPnts();
	void	circlesMoveFwd();
	void	circlesNewFrame();
	void	circlesNext();
	
	void	circlesPolyize(const Color & tint);

	int		m_list;

	float		m_tubeSize;
	float		m_lastTubeSize;

	RefPtr<Texture>	m_txr;

	Color		m_palette[256];
	int		m_palpos;
	float		m_hsv;

	point_t		m_plrPos;
	point_t		m_plrVel;
	float		m_plrSpin;

	point_t		m_view;
	int		m_viewXr, m_viewYr, m_viewZr;
	float		m_txrUOffs, m_txrVOffs;

	int		m_controlPos;
	float		m_trackPos;
	int		m_cirPos;

	point_t		m_ringPnts[TUBE_RINGS];
};

#endif	/* __TUBE_H */

