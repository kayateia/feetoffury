/*      
   Feet of Fury
        
   wordlist.h

   Copyright (C)2003 Cryptic Allusion, LLC

   $Id: wordlist.h,v 1.2 2003/03/02 07:11:55 bard Exp $
*/

#ifndef __WORDLIST_H
#define __WORDLIST_H

// This class will contain the basic step data for a song, and will also be
// used during gameplay for scoring calculations and such.
struct WordList : public RefCnt {
public:
	WordList(const char *fn = NULL);
	virtual ~WordList();

	// Load the step data from a DWI file
	bool	loadFromFile(const char *fn);

	// Return a pointer to a random word
	const char * getRandom() const;

private:
	// The actual word list data
	char	* m_words;

	// Pointers to individual words
	char	** m_wordPtrs;

	// Number of words
	int	m_wordCnt;
};

#endif	/* __WORDLIST_H */

