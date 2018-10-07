/*      
   Feet of Fury
        
   gridselector.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __GRIDSELECTOR_H
#define __GRIDSELECTOR_H

class GridSelector : public Drawable {
public:
	GridSelector(int gw, int gh, int giw, int gih, int numsels);
	virtual ~GridSelector();

	// Set a texture pair at the given grid coordinates
	// Note that this transfers ownership of the texture to the
	// GridSelector object.
	void setTextureAt(int x, int y, Texture * sel, Texture * unsel,
		const Color & tint = Color(1,1,1,1));

	// Set a selection's color and overlay texture
	void setSelAttrs(int idx, float r, float g, float b, Texture * txr);

	// Change which grid item we've got selected
	void setSelection(int idx, int x, int y);

	// Retrieve the given selection
	void getSelection(int idx, int * x, int * y);

	// Set the number of active selectors
	void setNumSels(int num);

	// Set the pixel spacing between grid squares
	void setSpacing(float spc);

	// Set whether the selector squares are drawn instead of the border
	// per square, or around the whole thing.
	void setSelectorOutside(bool outside);

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

private:
	// Is the given item selected?
	bool isSelected(int x, int y);
	
	// Selection item
	struct Selector {
		float		r, g, b;	// Color
		int		x, y;		// Selected x,y
		RefPtr<Texture>	txr;		// Overlay texture
	};

	int		m_frame;		// Frame counter
	int		m_gw, m_gh;		// Grid width and height
	int		m_giw, m_gih;		// Grid item width and height
	RefPtr<Texture>	* m_gtxrs_sel;		// Grid icon textures (selected)
	RefPtr<Texture>	* m_gtxrs_unsel;	// Grid icon textures (unselected)
	Color		* m_gtxrs_tint;		// Tint values
	int		m_numsels;		// Number of selectors
	int		m_numsels_active;	// Active selector count
	Selector	* m_sels;		// Selectors
	float		m_spc;			// Pixel spacing
	bool		m_selOutside;		// Selector outside/around?
};

#endif	/* __GRIDSELECTOR_H */

