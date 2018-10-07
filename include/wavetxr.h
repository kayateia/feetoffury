#ifndef __WAVETXR_H
#define __WAVETXR_H

class WaveTexture : public Drawable {
public:
	// Note: this object will take control of the passed texture
	WaveTexture(Texture * txr, int list);
	virtual ~WaveTexture();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	void setMag(float m) { m_mag = m; }
	float getMag() { return m_mag; }

	void setTexture(Texture *t);

private:
	RefPtr<Texture>	m_txr;		// Our texture
	float		m_phase;	// Phase value for the ripples
	float		m_mag;		// Effect magnitude
	int		m_list;		// Which list do we display in?
};

// An animator for the WaveTexture object to modify the waviness and
// alpha blending value.
class WaveTextureAnim : public Animation {
public:
	WaveTextureAnim(float wave_to, float dw, float alpha_to, float da) {
		m_wave_to = wave_to;
		m_dw = dw;
		m_alpha_to = alpha_to;
		m_da = da;
	}
	virtual ~WaveTextureAnim() { }

	virtual void nextFrame(Drawable *t) {
		WaveTexture *wt = (WaveTexture *)t;

		Color c = wt->getTint();
		c.a += m_da;
		wt->setTint(c);
		wt->setMag(wt->getMag() + m_dw);

		if (m_dw < 0.0f) {
			if (wt->getMag() <= m_wave_to) {
				wt->setMag(m_wave_to);
				m_dw = 0.0f;
			}
		} else if (m_dw > 0.0f) {
			if (wt->getMag() >= m_wave_to) {
				wt->setMag(m_wave_to);
				m_dw = 0.0f;
			}
		}

		if (m_da < 0.0f) {
			if (c.a <= m_alpha_to) {
				c.a = m_alpha_to;
				wt->setTint(c);
				complete(t);
			}
		} else {
			if (c.a >= m_alpha_to) {
				c.a = m_alpha_to;
				wt->setTint(c);
				complete(t);
			}
		}
	}

private:
	float	m_wave_to, m_dw, m_alpha_to, m_da;
};

#endif	// __WAVETXR_H

