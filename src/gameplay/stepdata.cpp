/*
   Feet of Fury

   gameplay/stepdata.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "stepdata.h"
#include "step.h"
#include "gpstate.h"
#include "wordlist.h"
#include "stepbomb.h"
#include "options.h"
#include "character.h"
// #include "fury.h"

// Constructor / Destructor
StepData::StepData(const char *fn, const char * songfn) {
	// Set some defaults
	step_data[Easy] = NULL;
	step_data[Medium] = NULL;
	step_data[Hard] = NULL;
	step_level[Easy] = 0;
	step_level[Medium] = 0;
	step_level[Hard] = 0;
	bpm = 0;
	gap = 0;

	memset(song_name, 0, sizeof(song_name));
	memset(song_artist, 0, sizeof(song_artist));
	memset(song_ogg, 0, sizeof(song_ogg));
	memset(song_fn, 0, sizeof(song_fn));
	memset(bpm_string, 0, sizeof(bpm_string));

	if (songfn)
		strcpy(song_fn, songfn);

	steps = NULL;

	TAILQ_INIT(&bpm_changes);

	firstStep = -1;

	if (fn != NULL) {
		if (!loadFromFile(fn)) {
			assert( false );
		}
	}
}

StepData::~StepData() {
	// Free steps
	if (steps != NULL)
		delete[] steps;
	if (step_data[Easy] != NULL)
		delete[] step_data[Easy];
	if (step_data[Medium] != NULL)
		delete[] step_data[Medium];
	if (step_data[Hard] != NULL)
		delete[] step_data[Hard];

	BPMChange * t, * n;
	t = TAILQ_FIRST(&bpm_changes);
	while (t) {
		n = TAILQ_NEXT(t, listent);
		delete t;
		t = n;
	}
}

// Setup a blank data file of length N steps
bool StepData::setupBlank(int n) {
	if (steps != NULL)
		delete[] steps;
	steps = new Step[n];
	step_cnt = n;
	step_arrsize = n;
	step_used_cnt = 0;

	return true;
}

// Save the step data to a DWI file
bool StepData::saveToFile(const char *fn) {
	Debug::printf("StepData::save_to_file(%s)\n", fn);

	// Open up the file
	File f;
	if (!f.open(fn, "wb")) {
		Debug::printf("Can't open step file '%s'\n", fn);
		return false;
	}

	char tmpbuf[256];
	sprintf(tmpbuf, "#FILE:%s;\r\n", song_ogg); f.write(tmpbuf, strlen(tmpbuf));
	sprintf(tmpbuf, "#TITLE:%s;\r\n", song_name); f.write(tmpbuf, strlen(tmpbuf));
	sprintf(tmpbuf, "#ARTIST:%s;\r\n", song_artist); f.write(tmpbuf, strlen(tmpbuf));
	sprintf(tmpbuf, "#GENRE:none;\r\n"); f.write(tmpbuf, strlen(tmpbuf));
	sprintf(tmpbuf, "#BPM:%d;\r\n", (int)bpm); f.write(tmpbuf, strlen(tmpbuf));
	sprintf(tmpbuf, "#GAP:%d;\r\n", gap); f.write(tmpbuf, strlen(tmpbuf));

	const char * diffs[(int)Count] = {
		"BASIC",
		"ANOTHER",
		"MANIAC"
	};
	for (int d=(int)Easy; d<(int)Count; d++) {
		if (step_data[d] != NULL) {
			sprintf(tmpbuf, "#SINGLE:%s:%d:", diffs[d], step_level[d]);
			f.write(tmpbuf, strlen(tmpbuf));

			f.write(step_data[d], strlen(step_data[d]));
			f.write(";\r\n", 3);
		}
	}

	f.write("\r\n\r\n", 4);

	return true;
}

// Load the step data from a DWI file
bool StepData::loadFromFile(const char *fn) {
	// Make sure no one tries to re-use us
	assert( steps == NULL );

	Debug::printf("StepData::load_from_file(%s)\n", fn);

	// Open up the file
	File f;
	if (!f.open(fn, "rb")) {
		Debug::printf("Can't open step file '%s'\n", fn);
		return false;
	}

	// Read the whole thing into a temp buffer (much simpler =)
	int size = f.total();
	char * inp = new char[size];
	f.read(inp, size);
	f.close();

	// Choose one, for debugging the FSM parser
	// #define PARSEDBG(x) Debug::printf x
	#define PARSEDBG(x) do { } while(0)

	// Parse through the data and store the pieces as appropriate
	// Ahh now nice it would be to be able to easily use Bison/Flex here...
	// But then, that might be overkill ^_-
	enum { None, SkipSection, SectionName, ReadToSemicolon, Bpm, Gap, ChangeBpm, StepSub } state = None;
	int i, o;
	char buffer[256], *outp = buffer;
	bool skip_white_space = false;
	int outp_left = 256;
	bpm = 0;
	for (i=0; i<size; /* */) {
		PARSEDBG(("Starting loop: state is %d\n", state));
		if (state == None) {
			/* Skip everything up to the next '#' marker */
			while (inp[i] != '#' && i<size) i++;
			if (i >= size) break;
			i++;
			state = SectionName;
			outp = buffer; outp_left = 256;
		}

		if (state == SkipSection) {
			o = 0;
			while (inp[i] != ';' && i<size) { i++; o++; }
			if (i >= size) break;
			i++;
			state = None;
			PARSEDBG(("   SkipSection: skipped %d bytes of input; next is '%c' at %d\n",
				o, inp[i], i));
		} else if (state >= SectionName) {
			/* Read the next part of text into the buffer */
			PARSEDBG(("   >=SectionName: will read string into buffer %08x (buffer=%08x)\n", outp, buffer));
			for (o=0; inp[i] != ':' && inp[i] != ';' && i<size && o<(outp_left-1); i++, o++) {
				if (skip_white_space
						&& (inp[i] == '\r' || inp[i] == '\n' || inp[i] == '\t' || inp[i] == ' ')) {
					o--;
					continue;
				}
				outp[o] = inp[i];
			}
			outp[o] = '\0';
			if (o < 512)
				PARSEDBG(("   >=SectionName: read string '%s' of length %d at %d; next is '%c'\n",
					outp, o, i - o, inp[i]));
			else
				PARSEDBG(("   >=SectionName: read string of size %d; next is '%c'\n",
					o, inp[i]));

			/* Look for boundary cases */
			if (i >= size) break;
			if (state == ReadToSemicolon && inp[i] == ';') {
				state = None;
			}
			if (inp[i] == ':') {
				i++;
			}

			if (state == SectionName) {
				/* Figure out what the section name was and set the next state */
				if (!strcmp(outp, "TITLE")) {
					PARSEDBG(("    Will read song name next\n"));
					outp = song_name; outp_left = sizeof(song_name);
					state = ReadToSemicolon;
				} else if (!strcmp(outp, "FILE")) {
					PARSEDBG(("    Will read song filename next\n"));
					outp = song_ogg; outp_left = sizeof(song_ogg);
					state = ReadToSemicolon;
				} else if (!strcmp(outp, "ARTIST")) {
					PARSEDBG(("    Will read song artist next\n"));
					outp = song_artist; outp_left = sizeof(song_artist);
					state = ReadToSemicolon;
				} else if (!strcmp(outp, "BPM")) {
					PARSEDBG(("    Will read song BPM next\n"));
					outp = bpm_string; outp_left = 15;
					state = Bpm;
				} else if (!strcmp(outp, "GAP")) {
					PARSEDBG(("    Will read song GAP next\n"));
					outp = buffer; outp_left = 256;
					state = Gap;
				} else if (!strcmp(outp, "SINGLE")) {
					PARSEDBG(("    Will read song steps next\n"));
					outp = buffer; outp_left = 256;
					state = StepSub;
				} else if (!strcmp(outp, "CHANGEBPM")) {
					PARSEDBG(("    Will read BPM change list next\n"));
					outp = buffer; outp_left = 256;
					state = ChangeBpm;
				} else {
					PARSEDBG(("    Will skip this section\n"));
					state = SkipSection;
				}
			} else if (state == Bpm) {
				bpm = (float)strtod(outp, NULL);
				PARSEDBG(("   Read BPM: %.2f\n", bpm));
				state = None;
			} else if (state == Gap) {
				gap = atoi(outp);
				PARSEDBG(("   Read GAP: %d\n", gap));
				state = None;
			} else if (state == ChangeBpm) {
				PARSEDBG(("   Reading CHANGEBPM:\n"));
				char * origoutp = outp;

				do {
					char	* finish;

					// Read the next beat id
					BPMChange * curChange = new BPMChange;
					curChange->beat = strtod(outp, &finish);
					outp = finish;

					// Verify we're still on track
					if (*outp != '=') {
						Debug::printf("   Couldn't parse CHANGEBPM: '%s'\n", origoutp);
						delete curChange;
						break;
					}
					outp++;

					// Read the next BPM value
					curChange->bpm = (float)strtod(outp, &finish);
					outp = finish;

					PARSEDBG(("     At beat %.2f, switch to BPM %.2f\n", (double)curChange->beat,
						(double)curChange->newbpm));

					TAILQ_INSERT_TAIL(&bpm_changes, curChange, listent);
				} while (*outp++ == ',');
				PARSEDBG(("   Finished reading CHANGEBPM\n"));
				outp = origoutp;
				state = None;
			} else if (state == StepSub) {
				/* Figure out which set of steps we're looking at */
				Difficulty w;
				if (!strcmp(outp, "BASIC")) {
					w = Easy;
				} else if (!strcmp(outp, "ANOTHER")) {
					w = Medium;
				} else if (!strcmp(outp, "MANIAC")) {
					w = Hard;
				} else {
					// Unknown (SMANIAC, etc)
					state = SkipSection;
					continue;
				}
				PARSEDBG(("   Will read steps named '%s' (%d), which is now 0x%08x\n", outp, w, (uint32)step_data[w]));

				// Cop out on the size for now...
				assert( step_data[w] == NULL );
				step_data[w] = new char[size];
				outp = step_data[w]; outp_left = size;
				state = ReadToSemicolon;
				skip_white_space = true;

				/* Read and skip the difficulty */
				step_level[w] = inp[i] - '0';
				i += 2;
			}
		}
	}

	// Make sure we parsed the whole thing correctly
	assert( state == None );
	assert( i>=size );

	/* printf("Title: '%s'\n", song_name);
	printf("Artist: '%s'\n", song_artist);
	printf("File: '%s'\n", song_ogg);
	printf("Fn: '%s'\n", song_fn);
	printf("BPM: %.2f (%d)\n", (double)bpm, (int)(bpm * 100));
	printf("GAP: %d\n", gap); */
	/* char diffs[3][16] = {"easy", "medium", "hard"};
	for (int i=0; i<Count; i++) {
		if (step_data[i] != NULL) {
			printf("Steps for '%s' at %08x (%d): ", diffs[i], step_data[i], step_level[i]);
			dbgio_printk(step_data[i]);
			printf("\n");
		}
	}
	printf("done\n"); */
	// assert( false );

	delete inp;
	return true;
}

// Calculate timeunit from bpm if possible
float StepData::calcBpm(float bpm) {
	if (bpm == 0)
		return 1.0f;

	// Calculate timeunit; note that DWI timeunits seem to be off by
	// a little bit, so we'll adjust here... (that or we're off, whatever)
	// return 60 * 1000.0f * 0.997f / (bpm * 4);
	return 60 * 1000.0f * 0.9975f / (bpm * 4);
}

// Parse the loaded step data into a format suitable for gameplay
bool StepData::parseData(Difficulty d, WordList * wl) {
	diff = d;
	assert(d >= Easy && d <= Hard);

	// Calc BPM if possible
	timeunit = calcBpm(bpm);

	// We'll allocate a new step array with room for expansion,
	// in case there are 16th notes in here.
	if (steps != NULL)
		delete[] steps;

	const char * stepdata = step_data[d];
	int arrsize = strlen(stepdata) * 4 + 1;
	steps = new Step[arrsize];

	// Convert the existing steps into our own format; at the same time,
	// we'll patch in the "dead-on" times.
	bool stm = 0;
	int i, o, msd_cnt = strlen(stepdata);
	int last_mark = -1;
	step_used_cnt = 0;
	for (i=0, o=0; i<msd_cnt; i++) {
		int sdi = stepdata[i];
		if (sdi == ' ') continue;

		// Skip over hold arrows -- take the initial pattern for the
		// standard arrow and throw away the hold. Also throw away the
		// hold-end marker (turn to a zero).
		if (sdi == '!') {
			last_mark = stepdata[i+1];
			i += 2;
			continue;
		}
		if (last_mark != -1 && sdi == last_mark) {
			last_mark = -1;
			sdi = '0';
		}

		if (!stm) {
			if (sdi == '(')
				stm = true;
			else {
				if (sdi != '0' && firstStep == -1) { firstStep = o; Debug::printf("firstStep = %d\n", o); }
				steps[o].pattern = sdi;
				steps[o].d = (int)(o * timeunit);
				o++;
				steps[o].blank();
				o++;
			}
		} else {
			if (stepdata[i] == ')')
				stm = false;
			else {
				if (sdi != '0' && firstStep == -1) { firstStep = o; Debug::printf("firstStep = %d\n", o); }
				steps[o].pattern = sdi;
				steps[o].d = (int)(o * timeunit);
				o++;
			}
		}
	}
	assert( o < arrsize );		// Make sure we didn't overflow

	step_cnt = o;
	step_arrsize = arrsize;
	parseIntervals(d, true, wl);

	// Debug
	/* for (i=32; i<64; i++) {
		printf("%d: s=%d\td=%d\te=%d,p=%c\n",
			i, steps[i].s, steps[i].d, steps[i].e, steps[i].pattern);
	} */

	return true;
}

// Compile the gameplay data back into the raw step data format
bool StepData::compileData(Difficulty d) {
	if (step_data[d] != NULL)
		delete step_data[d];
	step_data[d] = new char[step_cnt+3];

	step_data[d][0] = '(';

	int i;
	for (i=0; i<step_cnt; i++) {
		step_data[d][i+1] = steps[i].pattern;
	}
	step_data[d][step_cnt+1] = ')';
	step_data[d][step_cnt+2] = 0;

	return true;
}

// Clear out any step data we have
bool StepData::clearData() {
	return true;
}

// XXX Need to move this somewhere more interesting.
const char *words4[] = {
	"4000200080006000",
	"2000800020008000",
	"40208060",
	"4000600040006000",
	"0040002000800060",
	"4286"
};

const char *words5[] = {
	"40002000200080006000",
	"20008000200080002000",
	"402080604000",
	"40006000400060004000",
	"00400020008000600040",
	"42864000"
};

const char *words6[] = {
	"400040002000200080006000",
	"200080002000800020008000",
	"402080604020",
	"400020004000800040002000",
	"004000200080006000400020",
	"400280064002"
};

const char *words7[] = {
	"4000400020002000800080006000",
	"2000800020008000200080002000",
	"4020806040208000",
	"4000200040008000400020008000",
	"0040002000800060004000200080",
	"4002800640028000"
};

const char *words8[] = {
	"40004000200020008000800060006000",
	"20008000200080002000800020008000",
	"4020806040208060",
	"40002000400080004000200080006000",
	"00400020008000600040002000800060",
	"4002800640028040"
};

const char **words[] ={
	words4, words5, words6, words7, words8
};

// Go through and process steps for time intervals
bool StepData::parseIntervals(Difficulty diff, bool initial, WordList * wl) {
	// Are we doing Typing of Fury?
	if (wl == NULL) {
		// No -- do standard processing
		for (int i=1; i<(step_cnt - 1); i++) {
			// Skip the ones with no arrow pattern anyway
			if (steps[i].pattern == '0')
				continue;
			if (!initial && !(steps[i].flags & STEPFLAG_ADDED))
				continue;
		}
	} else {
		// Yes -- this will be quite different. Basically we'll disregard
		// all of the step data provided in the DWI and just use it to figure
		// out how long the song should be. The steps will be replaced by
		// randomized word patterns.

		// Find the song length
		int songlen = -1;
		for (int i=step_cnt-1; i>=0; i--) {
			if (steps[i].pattern != '0') {
				songlen = i+1;
				break;
			}
		}
		if (songlen == -1) {
			// Song is empty! Screw it... we're outta here
			//  XXX Later on we might want to have a way to pass in song length
			//  by hand for automated generation of steps without a DWI.
			return true;
		}

		// Figure out where to start based on the first step in the DWI file.
		int start = firstStep;
		start = (start + 3) & ~3;

		// Blank out the first 2 measures
		for (int i=0; i<start; i++) {
			steps[i].blank();
		}

		// Go through and generate "steps". We eventually want to modify
		// our parameters based on the user-selected difficulty but that can
		// be done a bit later.
		int i = start, lastfull = start, wordcnt = 0;
		while (i < songlen) {
			// Choose a word for the pattern
			const char * word = wl->getRandom();

			// Find the word's length
			int len = strlen(word);
			assert(len >= 4 && len <= 8);

			// Get our patterns that size
			const char **warray = words[len-4];

			// How many different patterns will we use?
			int ptncnt = 2;
			switch (diff) {
			case Medium: ptncnt = 4; break;
			case Hard: ptncnt = 6; break;
			}

			// Now choose a random pattern that size
			int idx = randnum(ptncnt);
			const char *ptn = warray[idx];

			// Stuff it into the steps
			int ptnlen = strlen(ptn);
			lastfull = i;
			for (int j=0, chi=0; j<ptnlen; j++) {
				if (ptn[j] != '0') {
					steps[i].blank();
					steps[i].pattern = ptn[j];
					steps[i].d = (int)(i*timeunit);
					steps[i].ch = word[chi++];
				} else {
					steps[i].blank();
				}
				i++;
				if (i >= songlen)
					break;
			}
			wordcnt++;

			// Now clear out some breathing room to the next word
			int skip = randnum(2)+1;
			for (int j=0; j<skip*4; j++) {
				steps[i].blank();
				i++;
				if (i >= songlen) {
					lastfull = i;
					break;
				}
			}
		}

		// Back up and erase the last word that didn't fit
		if (i >= songlen) {
			for (i=lastfull; i<songlen; i++) {
				steps[i].blank();
			}
		}

		Debug::printf("StepData::parseIntervals: Setup for Typing of Fury mode, used %d words\n", wordcnt);
	}

	return true;
}

// Make a deep copy of this StepData object; only covers load-time
// data, not generated step/timing data
StepData * StepData::deepCopy() {
	// Copy all the basics over
	StepData * n = new StepData();
	strcpy(n->song_name, song_name);
	strcpy(n->song_artist, song_artist);
	strcpy(n->song_ogg, song_ogg);
	strcpy(n->song_fn, song_fn);
	n->bpm = bpm;
	n->gap = gap;

	// Copy over step data
	for (int i=0; i<Count; i++) {
		if (step_data[i]) {
			n->step_data[i] = new char[ strlen(step_data[i])+1 ];
			strcpy(n->step_data[i], step_data[i]);
		} else
			n->step_data[i] = NULL;
		n->step_level[i] = step_level[i];
	}

	return n;
}

// Throw a step bomb into the arrows at the given time offset
bool StepData::combineBomb(int offset, const StepBomb *sb) {
	const char * pattern = sb->pattern;
	int i, len;

	len = strlen(pattern);
	for (i=0; i<len; i++) {
		// Skip blanks
		//if (pattern[i] == '0') continue;

		// Don't go past the end of the real step data
		if ((offset+i) >= step_cnt || (offset+i) >= step_used_cnt) break;

		// If there's a step there already, then replace it
		/*if (steps[offset+i].pattern != '0') {
			steps[offset+i].flags |= STEPFLAG_ADDED;
		} else { */
			// Insert a new step
			steps[offset+i].pattern = pattern[i];
			steps[offset+i].d = (int)((offset+i) * timeunit);
			steps[offset+i].hit = Step::NotYet;
			steps[offset+i].flags |= STEPFLAG_ADDED;
		//}
	}

	return true;
}

// Add random item steps to the existing step data (do this once when
// loading the data initially)
bool StepData::addBombs(bool typing, bool useAll /*, int fury*/) {
	// Go through and figure out how many steps we really have
	int rscnt = 0;
	/* for (int i=100; i<step_cnt; i++) {
		if (steps[i].pattern != '0')
			rscnt++;
	} */
	for (int i=step_cnt-1; i>=0; i--) {
		if (steps[i].pattern != '0') {
			rscnt = i;
			break;
		}
	}
	step_used_cnt = rscnt;

	// Do we even have enough to start?
	if (rscnt == 0)
		return false;

	// Figure out how many bombs we want as a function
	// of the length of the song (1%)
	// int total = (int)(steps * 0.01f);
	// int total = (int)(strlen(step_data[0]) * 0.01f);
	int total = (int)(rscnt * 0.025f);
	int cnt = 0;

	// Put together a list of attacks we can choose from (based on unlocked
	// characters thus far)
	int atkcnt = 0;
	int attacks[AttackCount];
	if (useAll) {
		// We want all attacks enabled (for demo mode probably), so just
		// fill the array with everything.
		atkcnt = AttackCount;
		for (int i=0; i<AttackCount; i++)
			attacks[i] = i;
	} else {
		// Ok, we want to weed stuff out for real gameplay. Phase one is to
		// only fill the array up with attacks from the selected fury.
		/* atkcnt = Fury::furyAttackCnt[fury];
		for (int i=0; i<atkcnt; i++)
			attacks[i] = Fury::furyAttacks[fury][i]; */
		atkcnt = AttackCount;
		for (int i=0; i<atkcnt; i++)
			attacks[i] = i;

		Debug::printf("--- Pre-lock attacks (%d) ---\n", atkcnt);
		for (int i=0; i<atkcnt; i++)
			Debug::printf("%d ", attacks[i]);

		// Finally, go through and throw out any attacks that aren't
		// unlocked currently. Probably won't leave us with much, but
		// that's ok ;). This algorithm is kinda slow too, but we
		// care more about correctness here than speed.
		for (int i=0; i<CHAR_COUNT; i++) {
			if (Characters::info[i].tech_type == AttackNone)
				continue;
			if (!game_options.char_unlocked[i]) {
				for (int j=0; j<atkcnt; j++) {
					if (Characters::info[i].tech_type == attacks[j]) {
						atkcnt--;
						memcpy(attacks+j, attacks+j+1, sizeof(int)*(atkcnt-j));
						break;
					}
				}
			}
		}
	}

	Debug::printf("--- Final available attacks (%d) ---\n", atkcnt);
	for (int i=0; i<atkcnt; i++)
		Debug::printf("%d ", attacks[i]);

	Debug::printf("\n--- Adding %d items for %d step-potentials (%d actuals)\n", total, step_cnt, rscnt);
	// Go through and add that many random steps
	while (cnt < total) {
		bool foundit = false;
		int offset;
		do {
			// Choose a reasonable location
			offset = (rand() % rscnt);

			for (int i=0, j=0; i<step_cnt; i++) {
				if (steps[i].pattern != '0')
					j++;
				if (j == offset) {
					offset = i;
					foundit = true;
					break;
				}
			}
		} while (!foundit);
		assert( offset >= 0 && offset < step_cnt );

		//printf("trying offset %d\n", offset);

		// Make sure we're replacing an existing arrow
		if (steps[offset].pattern == '0') {
			//printf("  no step here\n");
			continue;
		}

		// Make sure it's not already a bomb
		if (steps[offset].flags & STEPFLAG_ITEM_MASK) {
			//printf("  already a bomb here\n");
			continue;
		}

		// Check for existing bombs in the vicinity
		int i;
		for (i=offset - 10; i<(offset + 10); i++) {
			if (i < 0) { i=0; continue; }
			if (i >= step_cnt) break;
			if (steps[i].flags & STEPFLAG_ITEM_MASK)
				break;
		}
		if (steps[i].flags & STEPFLAG_ITEM_MASK) {
			//printf("  bomb nearby (at %d)\n", i);
			continue;
		}

		//printf("  putting a bomb there for real\n");

		// Ok, it's clear. Now decide what _kind_ of item we want.
		int type = attacks[randnum(atkcnt)];
		if (typing && type == AttackBomb)	// XXX Hack
			type = AttackTurbo;
		switch (type) {
		case AttackBomb:		type = STEPFLAG_ITEM_BOMB; break;
		case AttackDrunkenArrow:	type = STEPFLAG_ITEM_DRUNKEN; break;
		case AttackVortex:		type = STEPFLAG_ITEM_VORTEX; break;
		case AttackTurbo:		type = STEPFLAG_ITEM_TURBO; break;
		case AttackPhasingArrows:	type = STEPFLAG_ITEM_PHASING; break;
		case AttackFireball:		type = STEPFLAG_ITEM_FIREBALL; break;
		case AttackHealth:		type = STEPFLAG_ITEM_HEALTH; break;
		case AttackEraser:		type = STEPFLAG_ITEM_ERASER; break;
		case AttackDispel:		type = STEPFLAG_ITEM_DISPEL; break;
		case AttackReverse:		type = STEPFLAG_ITEM_REVERSE; break;
		}
		steps[offset].flags |= type;

		cnt++;
	}
	//printf("bomb adding done\n");

	return true;
}

// Remove N arrows at position Y
void StepData::rmArrows(int cp, int cnt) {
	int i;

	// Make sure we don't run over
	if ((cp + cnt) >= step_cnt)
		cnt = step_cnt - cp;

	// Scoot everything back
	for (i=0; i<step_cnt - cnt; i++) {
		steps[cp+i] = steps[cp+cnt+i];
	}

	// Blank the ends
	for (i=step_cnt - cnt; i<step_cnt; i++) {
		steps[i] = Step();
	}

	step_cnt -= cnt;
}

// Insert N blank arrows at position Y
void StepData::insArrows(int cp, int cnt) {
	int i;

	// If we'll run over, then reallocate the array
	if ((step_cnt+cnt) > step_arrsize) {
		Step * ns = new Step[step_cnt + step_cnt/2];
		for (i=0; i<step_cnt; i++)
			ns[i] = steps[i];
		delete steps;
		steps = ns;
		step_arrsize = step_cnt + step_cnt/2;
	}

	// Scoot everything forward
	for (i=step_cnt; i>cp; i--)
		steps[i] = steps[i - cnt];

	// Blank the inserted space
	for (i=cp; i<(cp+cnt); i++)
		steps[i] = Step();

	step_cnt += cnt;
}
