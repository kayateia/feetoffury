/*
   Feet of Fury

   menu_swapcd/textset.h

   Copyright (C)2002,2003 Cryptic Allusion, LLC

   $Id: textset.h,v 1.2 2003/03/02 07:11:56 bard Exp $
*/

#ifndef __TEXTSET_H
#define __TEXTSET_H


////////////////////////////////////////////////////////////////////////////

// A single string wrapper
class Text : public RefCnt {
public:
	Text(char * c) {
		m_c = strdup(c);
	}
	virtual ~Text() { free(m_c); }

	operator char * () const { return m_c; }
	operator const char * () const { return m_c; }

private:
	char * m_c;
};

// A little text set wrapper
class TextSet : public RefCnt {
public:
	TextSet(char * src) {
		m_src = src;
		texts = NULL;
		breakUp();
	}
	virtual ~TextSet() {
		delete[] texts;
		delete[] m_src;
	}

	const char ** texts;
	int texts_cnt;
private:
	void breakUp() {
		// Get the number of line breaks
		int lcnt = 0;
		char *tmp = m_src;
		while ( (tmp = index(tmp, '\n')) ) {
			tmp++;
			lcnt++;
		}

		if (lcnt == 0) lcnt++;

		// Allocate pointers into the string
		texts = new const char * [lcnt];
		memset(texts, 0, sizeof(char *) * lcnt);
		texts_cnt = lcnt;

		// Now go through and break it up
		texts[0] = m_src;
		lcnt = 1;
		tmp = m_src;
		while ( (tmp = index(tmp, '\n')) && lcnt < texts_cnt ) {
			*tmp = 0;
			texts[lcnt] = tmp+1;
			lcnt++;
			tmp++;
		}

		// Clean up any line breaks dangling around
		for (int i=0; i<texts_cnt; i++) {
			char *t = index(texts[i], '\r');
			if (t) *t = 0;
			t = index(texts[i], '\n');
			if (t) *t = 0;
		}

		for (int i=0; i<texts_cnt; i++) {
			uint32 t = (uint32)texts[i];
			assert( t >= 0x8c010000 && t < 0x8d000000 );
		}
	}

	char	* m_src;
};

#endif	// __TEXTSET_H
