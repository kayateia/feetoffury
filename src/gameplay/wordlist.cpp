/*
   Feet of Fury

   gameplay/wordlist.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "wordlist.h"

// Constructor / Destructor
WordList::WordList(const char *fn) {
	m_words = NULL;
	m_wordPtrs = NULL;
	m_wordCnt = 0;

	if (fn != NULL)
		loadFromFile(fn);
}

WordList::~WordList() {
	if (m_words)
		delete[] m_words;
	if (m_wordPtrs)
		delete[] m_wordPtrs;
}

bool WordList::loadFromFile(const char *fn) {
	// Try to open the source file
	File f;
	if (!f.open(fn, "rb")) {
		Debug::printf("WordList::loadFromFile(%s): can't open source!\n", fn);
		return false;
	}

	// Load up the whole thing into our internal buffer
	int size = f.total();
	m_words = new char[size];
	f.read(m_words, size);
	f.close();

	// Go through once to count the words
	m_wordCnt = 0;
	for (int i=0; i<size; i++) {
		if (m_words[i] == '\0')
			m_wordCnt++;
	}

	// Now go through again and setup pointers
	m_wordPtrs = new char*[m_wordCnt + 1];
	m_wordPtrs[0] = m_words;
	int wc = 0;
	for (int i=0; i<size; i++) {
		if (m_words[i] == '\0') {
			wc++;
			m_wordPtrs[wc] = m_words+i+1;
		}
	}
	assert( wc == m_wordCnt );

	Debug::printf("WordList::loadFromFile(%s): loaded %d words\n", fn, m_wordCnt);

	return true;
}

const char * WordList::getRandom() const {
	if (m_words == NULL) {
		assert( false );
		return NULL;
	}

	// Pick a random index
	int idx = randnum(m_wordCnt);

	// Return it
	return m_wordPtrs[idx];
}
