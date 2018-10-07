/*      
   Feet of Fury
        
   scrollybg.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __SCROLLYBG_H
#define __SCROLLYBG_H

class ScrollyBG : public Drawable {
public:
	ScrollyBG(Texture * txr, bool right = false, float speed = 1.0f) {
		m_txr = txr;
		m_txr->setUVClamp(Texture::UVRepeat, Texture::UVRepeat);

		m_right = right;
		m_uoffs = m_voffs = 0;
		m_speed = speed;
	}
	virtual ~ScrollyBG() { }

	virtual void draw(ObjType list) {
		if (list != Opaque)
			return;

		// Draw the scrolling background
		float uoffs = m_uoffs, voffs = m_voffs;
			
		float usize = 4.0f, vsize = 3.0f;
		Color c = getColor();
		c.r *= c.a; c.g *= c.a; c.b *= c.a; c.a = 1.0f;
		uint32 color = c;
		float z = getTranslate().z;

		m_txr->select();
		plx_vert_ifp(PLX_VERT,       0.0f, 480.0f, z, color, uoffs + 0.0f, voffs + vsize);
		plx_vert_ifp(PLX_VERT,       0.0f,   0.0f, z, color, uoffs + 0.0f, voffs + 0.0f);
		plx_vert_ifp(PLX_VERT,     640.0f, 480.0f, z, color, uoffs + usize, voffs + vsize);
		plx_vert_ifp(PLX_VERT_EOS, 640.0f,   0.0f, z, color, uoffs + usize, voffs + 0.0f);
	}

	virtual void nextFrame(uint64 t) {
		if (!m_right) {
			m_uoffs += m_speed / hz;
			m_voffs += m_speed / hz;
		} else {
			m_uoffs -= m_speed / hz;
			m_voffs += m_speed / hz;
		}

		int ig = (int)m_uoffs;
		m_uoffs -= ig;
		ig = (int)m_voffs;
		m_voffs -= ig;
		
		Drawable::nextFrame(t);
	}

private:
	RefPtr<Texture>	m_txr;
	float		m_uoffs, m_voffs;
	bool		m_right;
	float		m_speed;
};

#endif	/* __SCROLLYBG_H */

