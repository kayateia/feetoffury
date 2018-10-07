/*
   Feet of Fury

   drawables/manualviewer.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "manualviewer.h"
#include "manualfile.h"
#include "res.h"

// This was #if'd out in the "remix" code, so I don't know what was going on here.
// -Kayateia 2018
#if 0

class ManualImage : public RefCnt {
public:
	ManualImage(Texture * src, int w, int h) {
		m_txr = src;
		m_ew = w;
		m_eh = h;
	}
	virtual ~ManualImage() {
	}

	int getW() const { return m_txr->getW(); }
	int getH() const { return m_txr->getH(); }

	RefPtr<Texture>	m_txr;
	int	m_ew, m_eh;
};


ManualViewer::ManualViewer(const char * srcfn) {
	m_manbuf = NULL;
	m_images = NULL;

	m_font = res.getFont("sanserif");

	/* sprintf(fnbuf, "%s/test/%s", fs_getwd(), srcfn);
	printf("opening %s\n", fnbuf);
	file_t f = fs_open(fnbuf, O_RDONLY); */
	file_t f = res.getManPage(srcfn);
	if (f == FILEHND_INVALID)
		assert(false);
	int size = fs_total(f);

	MnlHeader hdr;
	fs_read(f, &hdr, sizeof(hdr));

	if (memcmp(hdr.magic, "FMNL", 4))
		assert( false );

	m_tokcnt = hdr.tokencnt;
	m_height = hdr.height;

	m_images = new RefPtr<ManualImage>[hdr.imgcnt];
	MnlImage img;
	for (int i=0; i<(int)hdr.imgcnt; i++) {
		fs_read(f, &img, sizeof(img));
		if (img.id == -1)
			continue;

		RefPtr<Texture> t = res.getTexture(Res::Manual, img.txrname, img.alpha ? true : false);
		m_images[i] = new ManualImage(t, img.width, img.height);
	}

	size -= sizeof(hdr) + hdr.imgcnt * sizeof(img);

	m_manbuf = new uint8[size];
	fs_read(f, m_manbuf, size);
	fs_close(f);

	m_yoffs = 0.0f;
}

ManualViewer::~ManualViewer() {
	if (m_manbuf)
		delete[] m_manbuf;
	if (m_images)
		delete[] m_images;
}

void ManualViewer::draw(ObjType list) {
	if (list != PLX_LIST_TR_POLY)
		return;

	m_font->setAlpha(1.0f);

	MnlToken * tok = (MnlToken *)m_manbuf;
	int tokcnt = 0;

	// Find our place in the file
	while (tok->y < (m_yoffs - 128) && tokcnt < m_tokcnt) {
		tok = (MnlToken *)( (uint8*)tok + tok->tokenSize );
		tokcnt++;
	}

	Vector p = getPosition(); p.y -= m_yoffs;
	char tmp[64];
	while ((tok->y - m_yoffs) < (480+128) && tokcnt < m_tokcnt) {
		float r = ((tok->color & 0x00ff0000) >> 16) / 255.0f;
		float g = ((tok->color & 0x0000ff00) >> 8) / 255.0f;
		float b = ((tok->color & 0x000000ff) >> 0) / 255.0f;
		if (tok->img != -1) {
			ManualImage * txr = m_images[tok->img];
			plx_cxt_texture(*(txr->m_txr));
			plx_cxt_send(list);

			float uoffs = (txr->getW() - txr->m_ew)/(2.0f*txr->getW());
			float voffs = (txr->getH() - txr->m_eh)/(2.0f*txr->getH());

			plx_vert_ffp(PLX_VERT, p.x+tok->x-txr->m_ew/2, p.y+tok->y+txr->m_eh/2, p.z,
				1.0f, r, g, b, uoffs, 1.0f - voffs);
			plx_vert_ffp(PLX_VERT, p.x+tok->x-txr->m_ew/2, p.y+tok->y-txr->m_eh/2, p.z,
				1.0f, r, g, b, uoffs, voffs);
			plx_vert_ffp(PLX_VERT, p.x+tok->x+txr->m_ew/2, p.y+tok->y+txr->m_eh/2, p.z,
				1.0f, r, g, b, 1.0f - uoffs, 1.0f - voffs);
			plx_vert_ffp(PLX_VERT_EOS, p.x+tok->x+txr->m_ew/2, p.y+tok->y-txr->m_eh/2, p.z,
				1.0f, r, g, b, 1.0f - uoffs, voffs);
		} else {
			m_font->setColor(r, g, b);
			m_font->setSize(tok->size * 1.0f);
			memcpy(tmp, tok->text, tok->textSize);
			tmp[tok->textSize] = 0;

			m_font->draw(p.x+tok->x, p.y+tok->y, p.z, tmp);
		}

		tok = (MnlToken *)( (uint8*)tok + tok->tokenSize );
		tokcnt++;
	}
}

void ManualViewer::nextFrame(uint64 tm) {
}

float ManualViewer::getOffset() const {
	return m_yoffs;
}

void ManualViewer::setOffset(float o) {
	if ((o + 480 - 64) > m_height)
		m_yoffs = m_height - 480 + 64;
	else
		m_yoffs = o;
}

#endif
