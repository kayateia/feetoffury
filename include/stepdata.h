/*      
   Feet of Fury
        
   stepdata.h

   Copyright (C)2002 Cryptic Allusion, LLC

   $Id: stepdata.h,v 1.21 2003/05/09 00:07:18 bard Exp $
*/

#ifndef __STEPDATA_H
#define __STEPDATA_H

class WordList;
class StepBomb;
class Step;

// Holds info about one BPM change in the song.
struct BPMChange {
	TAILQ_ENTRY(BPMChange)	listent;

	// Provided values
	float	beat;		// What beat does it happen on?
	float	bpm;		// New BPM value

	// Calculated values
	uint32	timeoffset;	// What millisecond in the game does it happen?
	float	timeunit;	// New timeunit value

	BPMChange() {
		timeoffset = 0;
		beat = bpm = timeunit = 0.0f;
		memset(&listent, 0, sizeof(listent));
	}
};

TAILQ_HEAD(BPMChangeList, BPMChange);

// This class will contain the basic step data for a song, and will also be
// used during gameplay for scoring calculations and such.
class StepData : public RefCnt {
public:
	enum Difficulty {
		Easy = 0,
		Medium = 1,
		Hard = 2,
		Count = 3,
		Invalid = 3
	};

	StepData(const char *fn = NULL, const char * songfn = NULL);
	virtual ~StepData();

	// Setup a blank gameplay data set of length N steps
	bool	setupBlank(int n);

	// Load the step data from a DWI file
	bool	loadFromFile(const char *fn);

	// Save the step data to a DWI file
	bool	saveToFile(const char *fn);

	// Calculate timeunit from bpm
	static float	calcBpm(float bpm);

	// Parse the loaded step data into a format suitable for gameplay.
	// If wl is non-NULL, then it will be used to form a Typing of Fury
	// set of steps instead of a standard DDR-style set.
	bool	parseData(Difficulty d, WordList * wl = NULL);

	// Compile the gameplay data back into the raw step data format
	bool	compileData(Difficulty d);

	// Clear out any step data we have
	bool	clearData();

	// Go through and process steps for time intervals. If wl is non-NULL,
	// then it will be used to form a Typing of Fury set of steps instead
	// of a standard DDR-style set.
	bool	parseIntervals(Difficulty d, bool initial, WordList * wl = NULL);

	// Make a deep copy of this StepData object 
	StepData * deepCopy();

	// Throw a step bomb into the arrows at the given time offset
	bool	combineBomb(int offset, const StepBomb *sb);

	// Add random bomb steps to the existing step data (do this once when
	// loading the data initially)
	bool	addBombs(bool typing, bool useAll /*, int fury*/);

	// Remove N arrows at position Y
	void	rmArrows(int y, int n);

	// Insert N blank arrows at position Y
	void	insArrows(int y, int n);

	// The song title and artist (avail. at load time)
	char	song_name[64];		// Human-readable song title
	char	song_artist[64];	// Human-readable artist name
	char	song_ogg[64];		// Ogg name (for editing)
	char	song_fn[64];		// Filename (e.g., djg_bufflog)

	// The DWI-style step data for the current song (avail. at load time)
	char 	* step_data[Count];
	int	step_level[Count];

	// Bool for each step on whether we hit it or not
	Step	* steps;
	int	step_cnt;
	int	step_arrsize;

	// How many actual steps are there in the song?
	int	step_used_cnt;

	// The BPM for the current song (avail. at load time)
	float	bpm;

	// The string version of the BPM, since all our float processing
	// seems somewhat inaccurate.
	char	bpm_string[16];

	// The current "time unit", based on BPM; 60000/(BPM*4)
	// This is basically one "sixteenth note".
	float	timeunit;

	// How many pixels do we need to scroll the arrows up the screen
	// per VBL (1/60th sec) in order to have proper timing? We need to
	// have one full arrow scroll by in (timeunit * 4), so this number
	// is calculated as follows:
	//  pxl_delta = N * 1000 / (60 * timeunit * 4)
	// where N is the height of a single quarter-beat arrow
	// float	m_pxl_delta;
	// Note: the above is no longer needed, but I kept the info because
	// it maybe useful later...
	
	// Initial "GAP" wait (avail. at load time)
	int	gap;

	// What difficulty level are we looking at currently?
	Difficulty	diff;

	// What index is the first step at?
	int	firstStep;

	// Pointer to the list of BPM changes
	BPMChangeList	bpm_changes;
};

#endif	/* __STEPDATA_H */

