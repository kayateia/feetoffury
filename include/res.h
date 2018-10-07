/*      
   Feet of Fury
        
   res.h

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#ifndef __RES_H
#define __RES_H

// Defines a generic resource acquisition class. This class
// will look at the state of the game (Swap CD mode enabled and
// various other things if necessary) and pull a named resource
// from an appropriate source. It handles the following resources:
//
// - Textures (loaded from PNGs, JPGs, or KMGs)
// - Sound effects (loaded from WAVs)
// - Fonts (loaded from TXFs)
// - Menu BGM (streamed from OGGs)
// - Song sub-resources (OGG streams for the song and samples, textures, etc)
// - Obtaining a listing of available songs
//
// Note that resource names don't necessarily directly map to filenames
// in the underlying filesystem. It's just an abstraction.
//

class Res {
public:
	Res();
	virtual ~Res();

	// Song list. This will be returned by the query function.
	class SongList : public RefCnt {
	public:
		SongList() {
			songs = NULL;
			songsSpace = NULL;
		}

		virtual ~SongList() {
			if (songs)
				delete[] songs;
			if (songsSpace)
				delete[] songsSpace;
		}

		char	**songs;
		char	*songsSpace;
		int	songCount;
	};

	// Texture/Sound domains
	enum Domain {
		Preload,
		CaIntro,
		Title,
		MenuMain,
		About,
		MenuOptions,
		MenuChar,
		MenuStage,
		Gameplay,
		Unlock,
		Jukebox,
		Slots,
		Manual
	};

	// Load a texture by name
	Texture * getTexture(Domain dom, const char * name, bool useAlpha = false);

	// Load a sound effect by name
	Sound * getSound(Domain dom, const char * name);

	// Load a font by name
	Font * getFont(const char * name);

	// Return an opened OGG stream for the named BGM
	FILE * getBgm(const char * name);

	// Get a named manual page
	FILE * getManPage(const char * name);

	// Return an opened OGG stream for the named song's main song
	FILE * getSongMain(const char * name);

	// Return an opened OGG stream for the named song's short sample
	FILE * getSongSample(const char * name);

	// Return a texture for the named song's thumbnail
	Texture * getSongThumbnail(const char * name);

	// Return a texture for the named song's background image
	Texture * getSongBackground(const char * name);
	
	// Return an allocated char * array with the contents of the step
	// file for the named song. This array must be freed after you are
	// done using it.
	char * getSongSteps(const char * name);

	// Returns a list of the available song IDs (to be passed into the
	// above functions).
	SongList * getSongList();


private:
	// foo
};

extern Res res;


#endif	/* __RES_H */

