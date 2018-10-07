/*      
   Feet of Fury
        
   bubble.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: bubble.h,v 1.13 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __BUBBLE_H
#define __BUBBLE_H

// Small animation class to handle the "pulsating" of the active
// sphere/cube combo.
class BubbleThrobber : public Animation {
public:
	BubbleThrobber() {
		m_frame = 0;
		m_throbbing = false;
		m_deselecting = false;
	}
	virtual ~BubbleThrobber() {
	}

	// Throb the object until it is deselected, then throb until
	// we get back to approximately 0.4f scale and un-select us
	virtual void nextFrame(Drawable *t) {
		if (m_throbbing) {
			float scale = Math::fsin(m_frame * 2*M_PI / 30.0f)*0.05f + 0.4f;
			if (m_deselecting && (Math::fabs(scale - 0.4f) < 0.01f)) {
				scale = 0.4f;
				m_deselecting = m_throbbing = false;
			}
			t->setScale(Vector(scale, scale, scale));
			m_frame++;
		}
	}

	void select() {
		m_frame = 0;
		m_throbbing = true;
	}

	void deselect() {
		m_deselecting = true;
	}

private:
	int	m_frame;
	bool	m_throbbing;
	bool	m_deselecting;
};


class Bubble : public Drawable {
public:
	enum ColorScheme {
		BlueGreen,
		Gray,
		RedGreen
	};

	Bubble(ColorScheme color);
	virtual ~Bubble();

	virtual void draw(ObjType list);

	virtual void nextFrame(uint64 t);

	void select() {
		m_throbber->select();
	}

	void deselect() {
		m_throbber->deselect();
	}

	void setColor(ColorScheme color) {
		m_color = color;
	}

private:
	void drawSphereBlue();
	void drawSphereGray();
	void drawSphereRed();

	RefPtr<BubbleThrobber>	m_throbber;
	float		m_phase;
	float		m_radius;
	int		m_slices, m_stacks;
	/* pvr_poly_cxt_t	m_cxt;
	pvr_poly_hdr_t	m_hdr; */

	ColorScheme	m_color;
};


#endif	/* __BUBBLE_H */

