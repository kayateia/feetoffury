/*
   Feet of Fury

   about/about.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "about.h"
#include "ogg.h"

// Our event listener.
using Hid::Event;
static RefPtr<Hid::EventCollector> events;

// The "about" text:

static const char * abt_text[] = {
// First a few blank lines to give the user a chance to see the tunnel
	"", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "",

// Basic credits (note lines starting with \a get a bigger font,
// and lines with \b are bigger and bright red)
	"\aFeet Of Fury",
	"\aCopyright (C) 2002,2003",
	"\aCryptic Allusion, LLC",
	"www.feetoffury.com",
	"",
	"",
	"",
	"\aGame Idea",
	"Cryptic Allusion",
	"",
	"\aGame Programming",
	"Bard",
	"",
	"\aLibraries",
	"The SourceForge KOS Group",
	"",
	"Xiph.Org and",
	"Thorsten Titze for",
	"libtremor",
	"",
	"Jeffrey McBeth and others",
	"for libpng",
	"",
	"Jean-loup Gailly and Jeffrey",
	"McBeth for zlib",
	"",
	"\aFonts",
	"Ray Larabie",
	"www.larabiefonts.com",
	"",
	"\aCharacter Art Coloring",
	"\aand Sedary Artwork",
	"FoxxFire",
	"foxx@foxxfire.com",
	"",
	"\aAll Other Characters",
	"\aand Line Art",
	"RX",
	"http://www.geocities.com/",
	"          RX_79_V/Main.htm",
	"",
	"\aMisc Textures",
	"Maximilian Scharl",
	"",
	"\aMan In The Moon",
	"\aThumbnail",
	"Kudos",
	"",
	"\aAll Other Graphics",
	"\aand Design",
	"Bard",
	"",
	"\aMusic and Sound Effects",
	"DJ Geki",
	"",
	"Aaron Marks",
	"On Your Mark Music",
	"www.onyourmarkmusic.com",
	"",
	"Chojin",
	"www.finalplanet.net",
	"www.terminalpress.com",
	"",
	"Kudos",
	"Kudosmix@hotmail.com",
	"",
	"\aVoices",
	"DJ Geki",
	"Bard",
	"",
	"\aSpecial Thanks",
	"- European Support -",
	"Maximilian Scharl",
	"",
	"- IGF Entry Sponsors -",
	"Brian Peek",
	"DJ Geki",
	"Roberto Camama",
	"",
	"- Beta Testers -",
	"Brian Peek",
	"Jim Ursetto",
	"Paul Boese",
	"DJ Geki",
	"Roger Cattermole",
	"Sam Steele",
	"Alex Aguila",
	"Kain Minter",
	"Phil Cordova",
	"Roberto Camama",
	"Louis Barbour",
	"Maximilian Scharl",
	"Richard Russo",
	"",
	"Jacob Alberty for the",
	"replacement IP.BIN",
	"",
	"Sega(tm), for creating the",
	"Dreamcast(tm) console and",
	"for being cool while it was",
	"still in production",
	"",
	"Konami(tm) for creating DDR,",
	"without which most of CA",
	"might actually do something",
	"\"socially productive\" on",
	"weekend nights =)",
	"",
	"The creators of GNU, Linux,",
	"The Gimp, GCC, Audacity, and",
	"all the other wonderful Free",
	"Software which made the",
	"creation of Feet of Fury",
	"and KOS possible",
	"",
	"\aMisc Hellos and",
	"\aGreets from Bard",
	"Ganksoft",
	"Tursi and Binky",
	"AndrewK",
	"Dementia 7",
	"All the cool people on the",
	"KOS and \"dcdev\" lists",
	"The Yoders (incl. Jessica)",
	"My homies back in Texas,",
	"Alex, Phil, Kain, Roddy,",
	"Roberto, Mike, Greg, Mark,",
	"David, Tim, Stephen, et al",
	"Friends in Tucson, Adam",
	"Guy, Andrew, Aaron,",
	"Andy, Warner, Cia,",
	"Jenn, et al",
	"The MegaTokyo Crew",
	"Michael Poe of E.N.",
	"RK of Something Positive",
	"Akira of Tsunami Channel",
	"Merekat",
	"And a bunch of other people",
	"who I will probably forget",
	"and go DOH! when I realize",
	"who I left off",
	"",
	"\aMisc Hellos and",
	"\aGreets from Geki",
	"First, many thanks to my room-",
	"mate Alan for putting up w/",
	"my loud-ass room, and then",
	"willingly giving an overall",
	"favorable rating to my music!",
	"Thanks to Korg (who will pro-",
	"bably never see this) for",
	"making cool dance music tools.",
	"Last, a big HUGE \"thank you\"",
	"to Bard",
	"Yer coding saved our asses (of",
	"fire), buddy.  :-)",
	"",
	"\aLike us?",
	"Cryptic Allusion, LLC is an",
	"active game development",
	"group on the DC console, and",
	"may eventually be moving to",
	"other consoles as well. If",
	"you like what you saw here,",
	"then visit us on the web:",
	"",
	"www.feetoffury.com",
	"www.cagames.com",
	"gamedev.allusion.net",
	"",
	"\aWant to publish?",
	"GOAT Store Publishing",
	"provides homebrew developers",
	"for discontinued consoles",
	"a viable way to sell their",
	"best creations. Come visit",
	"us on the web:",
	"",
	"www.goatstore.com",
	"",
	"\aOwari Da",
	"Thanks for playing our game.",
	"We hope you enjoy it.",
	"", "", "", "",
	"Build 20030528",
#ifdef KIOSK_MODE
	"(KIOSK MODE)",
#endif
	"", "", "", "",	"", "", "", "",
	"", "", "", "", ""
};
static const int abt_text_cnt = sizeof(abt_text) / sizeof(char *);
static int abt_top_line;
static float abt_line_offset;

// Font renderer
static RefPtr<Font> fnt;

static float alpha = 0.0f;

static float loops = 0;

static void textFrame() {
	int i;
	float y;

	y = 60.0f - abt_line_offset;
	for (i=abt_top_line; y<480.0f; i++) {
		i = (i % abt_text_cnt);

		// Get the line and its size
		const char *l = abt_text[i];
		float ptsize = 30.0f, skip = 32;
		float r = 1.0f, g = 1.0f, b = 1.0f, a;
		if (l[0] == '\a') {
			ptsize = 36.0f;
			l++;
			skip = 38;
			r = 0.4f; g = 0.4f;
		} else if (l[0] == '\b') {
			ptsize = 36.0f;
			l++;
			skip = 38;
			r = 1.0f; g = 0.0f; b = 0.0f;
		}
		if (y < 98.0f) {
			a = (y - 60.0f) / 38.0f;
		} else if (y >= 420.0f) {
			a = 1.0f - (y - 420.0f) / 38.0f;
		} else
			a = 1.0f;
		if (a < 0.0f)
			a = 0.0f;
		else if (a >= 1.0f)
			a = 1.0f;

		// Draw the line
		fnt->setColor(Color(a*alpha, r, g, b));
		fnt->setSize(ptsize);
		fnt->drawCentered(Vector(320.0f, y - 32.0f, 128.0f), l);

		y += skip;
	}

	abt_line_offset += 0.5f * (60.0f/hz);
	const char *l = abt_text[abt_top_line];
	if (l[0] == '\a') {
		if (abt_line_offset >= 38.0f) {
			abt_line_offset = 0.0f;
			abt_top_line++;
			if (abt_top_line >= abt_text_cnt)
				loops++;
			abt_top_line = (abt_top_line % abt_text_cnt);
		}
	} else {
		if (abt_line_offset >= 32.0f) {
			abt_line_offset = 0.0f;
			abt_top_line++;
			if (abt_top_line >= abt_text_cnt)
				loops++;
			abt_top_line = (abt_top_line % abt_text_cnt);
		}
	}
}

static enum { CubeNone, CubeL, CubeU, CubeR, CubeD, CubeX, CubeY, CubeDisplay } cubeState = CubeNone;
static bool checkControl() {
	Event evt;

	if (!events->getEvent(evt))
		return false;

	if (evt.type == Event::EvtQuit)
		return true;

	if (evt.type == Event::EvtBtnPress)
		return true;

	if (evt.type == Event::EvtKeypress) {
		if (evt.key != Event::KeyLeft && evt.key != Event::KeyRight &&
			evt.key != Event::KeyUp && evt.key != Event::KeyDown)
		{
			return true;
		}

		int old = cubeState;
		switch (cubeState) {
		case CubeNone:
			if (evt.key == Event::KeyLeft)
				cubeState = CubeL;
			break;
		case CubeL:
			if (evt.key == Event::KeyUp)
				cubeState = CubeU;
			else
				cubeState = CubeNone;
			break;
		case CubeU:
			if (evt.key == Event::KeyRight)
				cubeState = CubeR;
			else
				cubeState = CubeNone;
			break;
		case CubeR:
			if (evt.key == Event::KeyDown)
				cubeState = CubeD;
			else
				cubeState = CubeNone;
			break;
		case CubeD:
			if (evt.key == Event::KeyLeft)
				cubeState = CubeX;
			else
				cubeState = CubeNone;
			break;
		case CubeX:
			if (evt.key == Event::KeyUp)
				cubeState = CubeDisplay;
			else
				cubeState = CubeNone;
			break;
		case CubeDisplay:
			break;
		}
		/* if (old != cubeState) {
			printf("Switched from state %d to state %d\n", old, cubeState);
		} */
	}

	return false;
}

/* void tubeInit();
void tubeFrame();
void tubeShutdown(); */
void bubblesInit();
void bubblesDraw(float a);
void cubeInit();
void cubeDraw(float a);
void cubeShutdown();

static void doFrame() {
	Frame::begin();

	if (alpha > 0.0f) {
	// XXX
	//	pvr_fog_table_linear(4.0f, 20.0f);
	//	pvr_fog_table_color(alpha, 0.2f, 0.2f, 0.3f);
	}

	//vid_border_color(255, 0, 0);
	// tubeFrame();
	if (alpha > 0.0f) {
		bubblesDraw(alpha);
		if (cubeState == CubeDisplay)
			cubeDraw(alpha);
	}
	/* tunnelDraw(alpha); */
	//vid_border_color(0, 0, 0);

	Frame::transEnable();

	textFrame();

	Frame::finish();
}

void doAbout(bool demo_mode) {
	abt_top_line = 0;
	abt_line_offset = 0.0f;
	cubeState = CubeNone;

	// tunnelLoadTxrs();

	events = new Hid::EventCollector();
	fnt = new Font("preload/fonts/axaxax.txf");

	// tunnelInit();
	// tubeInit();
	bubblesInit();
	cubeInit();

	// Start the music
	oggStart("bgm/about.ogg", true, false, false);

	bool dieing = false;
	bool done = false;
	loops = 0;
	while (!done) {
		dieing = dieing || checkControl();
		if (demo_mode && loops >= 1)
			dieing = true;

		if (!dieing && !done && alpha < 1.0f)
			alpha += 0.05f;
		if (dieing && !done)
			alpha -= 0.1f;
		if (alpha >= 1.0f)
			alpha = 1.0f;
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			if (dieing)
				done = true;
		}
		if (dieing)
			oggVolume(alpha);

		doFrame();
	}

	// Stop the music
	oggStop(false);

	// A few more to clear out the buffers
	doFrame();
	doFrame();
	doFrame();

	cubeShutdown();

	events = NULL;

	// tubeShutdown();
	//tunnelShutdown();
	fnt = NULL;
}
