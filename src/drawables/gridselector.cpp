/* 
   Feet of Fury
   
   drawables/gridselector.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "gridselector.h"

GridSelector::GridSelector(int gw, int gh, int giw, int gih, int numsels) {
	m_frame = 0;
	m_gw = gw;
	m_gh = gh;
	m_giw = giw;
	m_gih = gih;
	m_gtxrs_sel = new RefPtr<Texture>[m_gw * m_gh];
	m_gtxrs_unsel = new RefPtr<Texture>[m_gw * m_gh];
	m_gtxrs_tint = new Color[m_gw * m_gh];

	m_numsels = m_numsels_active = numsels;
	m_sels = new Selector[m_numsels];
	for (int i=0; i<m_numsels; i++) {
		m_sels[i].x = m_sels[i].y = 0;
		m_sels[i].r = m_sels[i].g = m_sels[i].b = 0.0f;
	}

	m_spc = 10.0f;
	m_selOutside = true;
}

GridSelector::~GridSelector() {
	delete[] m_sels;
	delete[] m_gtxrs_tint;
	delete[] m_gtxrs_unsel;
	delete[] m_gtxrs_sel;
}

void GridSelector::setTextureAt(int x, int y, Texture * sel, Texture * unsel, const Color & tint) {
	assert( x < m_gw && y < m_gh );

	m_gtxrs_sel[y*m_gw+x] = sel;
	m_gtxrs_unsel[y*m_gw+x] = unsel;
	m_gtxrs_tint[y*m_gw+x] = tint;
}

void GridSelector::setSelAttrs(int idx, float r, float g, float b, Texture * txr) {
	assert( idx < m_numsels );
	m_sels[idx].r = r;
	m_sels[idx].g = g;
	m_sels[idx].b = b;
	m_sels[idx].txr = txr;
}

void GridSelector::setSelection(int idx, int x, int y) {
	assert( x < m_gw && y < m_gh && idx < m_numsels );

	m_sels[idx].x = x;
	m_sels[idx].y = y;
}

void GridSelector::getSelection(int idx, int * x, int * y) {
	assert( idx < m_numsels );

	*x = m_sels[idx].x;
	*y = m_sels[idx].y;
}

void GridSelector::setNumSels(int num) {
	assert( num <= m_numsels );
	m_numsels_active = num;
}

void GridSelector::setSpacing(float spc) {
	m_spc = spc;
}

void GridSelector::setSelectorOutside(bool outside) {
	m_selOutside = outside;
}

void GridSelector::draw(ObjType list) {
	if (list != Trans)
		return;

	Vector pos = getPosition();
	float spc = m_spc;
	float tw = (spc+m_giw) * m_gw + spc, th = (spc+m_gih) * m_gh + spc;
	float x1 = pos.x - tw/2.0f + spc, y1 = pos.y - th/2.0f + spc, z1 = pos.z;
	Color c = getColor();
	plx_dr_state_t dr;

	plx_cxt_texture(NULL);
	plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(list);

	plx_dr_init(&dr);

	/*plx_spr_fnd(&dr, tw, th, pos.x, pos.y, pos.z,
		0.8f, 0.2f, 0.2f, 0.2f); */

	for (int y=0; y<m_gh; y++) {
		for (int x=0; x<m_gw; x++) {
			int selected = isSelected(x, y);

			z1 = pos.z;

			// Draw any active selectors behind it
			int pull = 0;
			for (int i=0; i<m_numsels_active; i++) {
				if (m_sels[i].x == x && m_sels[i].y == y) {
					float r1 = c.r*m_sels[i].r;
					float g1 = c.g*m_sels[i].g;
					float b1 = c.b*m_sels[i].b;
					float r2 = c.r*m_sels[i].r * 0.5f;
					float g2 = c.g*m_sels[i].g * 0.5f;
					float b2 = c.b*m_sels[i].b * 0.5f;
					if (!m_selOutside)
						pull = -2;

					// Draw the selector itself
					Texture::deselect();
					plx_vert_fnd(&dr, PLX_VERT, x1-4.0f-pull, y1+4.0f+m_gih+pull, z1, c.a, r1, g1, b1);
					plx_vert_fnd(&dr, PLX_VERT, x1-4.0f-pull, y1-4.0f-pull, z1, c.a, r2, g2, b2);
					plx_vert_fnd(&dr, PLX_VERT, x1+4.0f+m_giw+pull, y1+4.0f+m_gih+pull, z1, c.a, r2, g2, b2);
					plx_vert_fnd(&dr, PLX_VERT_EOS, x1+4.0f+m_giw+pull, y1-4.0f-pull, z1, c.a, r1, g1, b1);
					
					z1++;
				}
			}

			// Draw a shaded square behind it
			if (m_selOutside) {
				Texture::deselect();
				plx_vert_fnd(&dr, PLX_VERT, x1-2.0f, y1+2.0f+m_gih, z1, c.a, c.r*0.2f, c.g*0.2f, c.b*0.2f);
				plx_vert_fnd(&dr, PLX_VERT, x1-2.0f, y1-2.0f, z1, c.a, c.r*0.1f, c.g*0.1f, c.b*0.1f);
				plx_vert_fnd(&dr, PLX_VERT, x1+2.0f+m_giw, y1+2.0f+m_gih, z1, c.a, c.r*0.2f, c.g*0.2f, c.b*0.2f);
				plx_vert_fnd(&dr, PLX_VERT_EOS, x1+2.0f+m_giw, y1-2.0f, z1, c.a, c.r*0.1f, c.g*0.1f, c.b*0.1f);
				z1++;
			}

			// Draw the image itself
			bool drawit = false;
			if (selected) {
				if (m_gtxrs_sel[y*m_gw+x]) {
					m_gtxrs_sel[y*m_gw+x]->select();
					drawit = true;
				}
			} else {
				if (m_gtxrs_unsel[y*m_gw+x]) {
					m_gtxrs_unsel[y*m_gw+x]->select();
					drawit = true;
				}
			}
			if (drawit) {
				Color ct = c * m_gtxrs_tint[y*m_gw+x];
				plx_spr_fnd(&dr, m_giw, m_gih, x1+m_giw/2, y1+m_gih/2, z1,
					ct.a, ct.r, ct.g, ct.b);
				z1++;
			}

			// Now do all that crap again to draw overlays.
			pull = 0;
			for (int i=0; i<m_numsels_active; i++) {
				if (m_sels[i].x == x && m_sels[i].y == y) {
					float r1 = c.r*m_sels[i].r;
					float g1 = c.g*m_sels[i].g;
					float b1 = c.b*m_sels[i].b;
					float r2 = c.r*m_sels[i].r * 0.5f;
					float g2 = c.g*m_sels[i].g * 0.5f;
					float b2 = c.b*m_sels[i].b * 0.5f;
					float z = z1 - (1.0f + 0.5f * (i+1));
					if (!m_selOutside)
						pull = -2;

					// And draw the overlay texture (if any)
					if (m_sels[i].txr) {
						float a = 0.5f + fsin(m_frame*(60.0f/hz)*2*M_PI/hz) / 4.0f;
						m_sels[i].txr->select();
						plx_spr_fnd(&dr, m_giw, m_gih, x1+m_giw/2, y1+m_gih/2, z1,
							c.a * a , c.r, c.g, c.b);
					}
					
					z1++;
					pull+=2;
				}
			}

			x1 += m_giw + spc;
		}

		x1 -= (m_giw+spc) * m_gw;
		y1 += m_gih+spc;
	}
}

void GridSelector::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);
	m_frame++;
}

bool GridSelector::isSelected(int x, int y) {
	for (int i=0; i<m_numsels_active; i++) {
		if (m_sels[i].x == x && m_sels[i].y == y)
			return true;
	}

	return false;
}

