/*
   Feet of Fury

   unlock.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "unlock.h"


// Unlock texts. We really probably ought to find a better
// way to do this, but... *sigh*
#define DECL(x, c, t) Unlock::Unlockable(x, sizeof(x) / sizeof(x[0]), c, t)
#define DECL_W_INFO(x, x2, c, t) Unlock::Unlockable(x, sizeof(x) / sizeof(x[0]), c, t, NULL, x2, sizeof(x2)/sizeof(x2[0]))
#define DECL_ART(x, x2, c, t) Unlock::Unlockable(x, sizeof(x) / sizeof(x[0]), c, "art", t, x2, sizeof(x2)/sizeof(x2[0]))
#define BLANK Unlock::Unlockable(ul_txt_locked, sizeof(ul_txt_locked) / sizeof(ul_txt_locked[0]), -1, NULL)

static const char *ul_txt_locked[] = {
	"This square is locked.",
	"To unlock it, you must",
	"possess enough unlock",
	"credits. Press A (or",
	"ENTER) on the square",
	"to unlock the item."
};
Unlock::Unlockable Unlock::locked = DECL(ul_txt_locked, -1, NULL);

static const char *ul_junky[] = {
	"Metal Junky. A",
	"mysterious robot",
	"playable character.",
	"This gives you the",
	"Turbo technique."
};

static const char *ul_aquatic[] = {
	"Aquatic, an unreleased",
	"track from DJ Geki",
	"aka Roddy Toomim."
};

static const char *msg_aquatic[] = {
	"Roddy says: \"This is just a short",
	"example of a song I made entirely on",
	"the Korg KARMA (an absolutely AMAZING",
	"musician's tool, created by Stephen Kay).",
	"Yes, those chords are supposed to be",
	"there. Also, this was a very rough cut",
	"of the song. It was never meant to be",
	"heard by anyone except me, but I figured",
	"it would be an interesting listen.\""
};

static const char *ul_dreams2[] = {
	"Dreams2, by Kudos.",
	"A playable in-game",
	"song."
};

static const char *ul_traycard[] = {
	"Design notes:",
	"Tray Card Layout"
};

static const char *msg_traycard[] = {
	"Tray Card Layout",
	" ",
	"This was an sketch Bard did for the",
	"traycard layout. The final layout ended",
	"up being a bit different, but this is",
	"still pretty close."
};

static const char *ul_cereal[] = {
	"Cereal Experiments",
	"(Plain), an unreleased",
	"track from DJ Geki"
};

static const char *msg_cereal[] = {
	"Geki says: This song was my first",
	"foray into using a vocoder (thank you",
	"Korg!) and formant holds. I quite",
	"obviously overused the vocoder, but it",
	"was hella fun. :) Just making freaky",
	"noises into the mic, and coming out",
	"sounding like a drugged up Saturday",
	"morning robot is enough to make a fan",
	"of those old shows weep.\""
};

static const char *ul_charlotte[] = {
	"Charlotte Montabelle.",
	"A playable ghost",
	"character. This gives",
	"you the Fireball",
	"technique."
};

static const char *ul_songmenumath[] = {
	"Design notes:",
	"Song Menu Math"
};

static const char *msg_songmenumath[] = {
	"Song Menu Math",
	" ",
	"Some math scribbles in Bard's notebook",
	"for how to make the CD thumbnails work",
	"right in the song menu. Yeah, go ahead",
	"and laugh all you math-abled people :)"
};

static const char *ul_gameplaylayout[] = {
	"Design notes:",
	"Gameplay Layout"
};

static const char *msg_gameplaylayout[] = {
	"Gameplay Layout",
	" ",
	"The initial scribblings that eventually",
	"led to the gameplay layout we have",
	"today. In this one we assumed we'd have",
	"fully animated characters in the",
	"background, but that didn't happen."
};

static const char *ul_fonky[] = {
	"Fonky Scramble, an",
	"unreleased track from",
	"DJ Geki"
};

static const char *msg_fonky[] = {
	"Geki says: \"This one has had many",
	"names...none of which I'm able to",
	"share with you (we're trying to keep",
	"the game at least PG-rated here,",
	"kiddies). In the same vein as 'Radio",
	"for Help,' this song was composed",
	"using entirely pre-fabricated",
	"samples. I think naming this track",
	"something that wasn't on the list",
	"of profanities was the most difficult",
	"part of producing it.\""
};

static const char *ul_gnc[] = {
	"Got No Clue What I",
	"Should Do, A-Kon Mix,",
	"an unreleased track",
	"from DJ Geki"
};

static const char *msg_gnc[] = {
	"Geki says: \"You all know 'Got No Clue'",
	"by now...it's an in-game song in Feet",
	"of Fury. However, it began life as a",
	"project for the anime convention 'AKon'",
	"that takes place in Dallas, TX every",
	"summer. I was struggling at the time,",
	"trying to come up with something to",
	"compose...and when I finally had",
	"absolutely no clue what to compose,",
	"my mind kinda went blank, and this",
	"is what came out. Kind of epihanic.",
	"Plus, the story helps decode the",
	"name, I suppose.\""
};

static const char *ul_arrows1[] = {
	"Design Notes:",
	"In-game Arrows (1)"
};

static const char *msg_arrows1[] = {
	"In-game Arrows (1)",
	" ",
	"This shows an early design concept for",
	"the Feet of Fury arrows. There are",
	"also some notes on this page about how",
	"to calculate timing and scoring."
};

static const char *ul_radio[] = {
	"Radio For Help,",
	"early mix, an",
	"unreleased track from",
	"DJ Geki."
};

static const char *msg_radio[] = {
	"Geki says: \"Here we have an example of",
	"what tracks sound like in my apartment",
	"BEFORE they are supposed to be",
	"released. :) Listen to this song and",
	"then its counterpart, and you'll have",
	"a slight glimpse into what it's like",
	"to design tracks and improve them over",
	"time. I forced myself to work with lots",
	"of samples for this one, just to make",
	"sure I could do it and get away with it.\""
};

static const char *ul_vengeance[] = {
	"Vengeance. A playable",
	"fish character. This",
	"gives you the Vortex",
	"technique."
};

static const char *ul_dcman[] = {
	"Artwork:",
	"DC Man was a quick",
	"sketch Bard did for",
	"the original MWC",
	"DC flyer."
};

static const char *msg_dcman[] = {
	"DC Man",
	" ",
	"Bard says: \"Paul Boese asked me if I",
	"could help him make a flyer for the",
	"Midwest Classic in 2002, so I went a",
	"little overboard and drew this funny",
	"guy to go at the top. Took only about",
	"30 minutes drawing him, so he's not",
	"terribly impressive.\""
};

static const char *ul_ramp[] = {
	"Ramp by Aaron Marks.",
	"A playable in-game",
	"song."
};

static const char *ul_bf[] = {
	"Nostalgia:",
	"Beat Fighter, in",
	"game screen shot."
};

static const char *msg_bf[] = {
	"Beat Fighter",
	" ",
	"Before there was Feet of Fury, there",
	"was Beat Fighter. This crummy looking",
	"thing was a prototype for the code that",
	"would eventually handle FoF's timing and",
	"main arrows graphics. The song and steps",
	"were hard-coded into the program."
};

static const char *ul_tubemath[] = {
	"Design notes:",
	"Tube Math"
};

static const char *msg_tubemath[] = {
	"Tube Math",
	" ",
	"This was actually back from the DC Tonic",
	"days (which is where the tube effect in",
	"the background of FoF's gameplay came",
	"from, actually). This was for a shooter",
	"game we were working on called",
	"Tryptonite. That never got finished",
	"unfortunately..."
};

static const char *ul_movieguy[] = {
	"Geki as Movie Guy,",
	"A humorous sample of",
	"'voice outtakes' from",
	"a potential plot",
	"intro we were",
	"considering."
};

static const char *msg_movieguy[] = {
	"A long long time ago, we actually had",
	"planned to have a full intro with a",
	"campy movie-guy voiceover (and the plot",
	"was different back then). This is a",
	"sample of Geki recording himself doing",
	"the 'movie guy' voice, with the",
	"assistance of Alan. He probably never",
	"expected it to end up in the public",
	"ear, but here it is!"
};

static const char *ul_bhedu[] = {
	"BHedU by DJ Geki.",
	"A playable in-game",
	"song."
};

static const char *ul_arrows2[] = {
	"Design Notes:",
	"In-game Arrows (2)"
};

static const char *msg_arrows2[] = {
	"In-game Arrows (2)",
	" ",
	"Notes from Bard's notebook on how the",
	"arrows would be rotated with U/V coords",
	"and how timing might be accomplished."
};

static const char *ul_funny[] = {
	"Random Voice Funnies,",
	"A humorous sample of",
	"'voice outtakes' from",
	"Bard and Geki trying",
	"to make a better",
	"intro sample."
};

static const char *msg_funny[] = {
	"Geki wasn't happy with the quality",
	"of the magic intro announcer sample,",
	"so here he is with Bard trying to record",
	"a new one. Unfortunately we could never",
	"quite duplicate the sound of the",
	"original (which remains in place), but",
	"this amusing sound file did come out",
	"of the attempt. This one's pretty",
	"bleepy, so if you have sensitive ears",
	"then tune out! :)"
};

static const char *ul_comeon[] = {
	"Come On Everybody,",
	"an unreleased track",
	"from DJ Geki."
};

static const char *msg_comeon[] = {
	"Geki says: \"If you don't have your",
	"bass turned WAY up before you play",
	"this song, you'll be missing out...",
	"of course, if you turn it up too",
	"loud, you'll blow up your",
	"subwoofer. :) This is just another",
	"example of what one can do with an",
	"analogue synth. I love my MicroKorg!",
	"Hope you enjoy it!\""
};

static const char *ul_bug[] = {
	"Funny Audio Bug, a",
	"bug in an earlier",
	"version of FoF that",
	"seemed funny",
	"enough to record."
};

static const char *msg_bug[] = {
	"Back when Bard was minimizing loading",
	"times, the game would sometimes load",
	"so fast that new sound effects would",
	"overwrite old ones while they were",
	"still playing. This is a sample of",
	"that that we thought was funny",
	"enough to record for posterity."
};

static const char *ul_attackicons[] = {
	"Design notes:",
	"Technique Icons"
};

static const char *msg_attackicons[] = {
	"Technique Icons",
	" ",
	"Quick sketches from Bard's notebook on",
	"how some of the attack/defense icons",
	"might look."
};

static const char *ul_bufflsr[] = {
	"BuffLog Logarithmic",
	"Sine Remix by DJ",
	"Geki. A playable",
	"in-game song."
};

static const char *ul_calogo[] = {
	"Design notes:",
	"Cryptic Allusion's",
	"Logo"
};

static const char *msg_calogo[] = {
	"Cryptic Allusion's Logo",
	" ",
	"The original CA logo concept sketch.",
	"This was done with a compass and a",
	"ruler, and was later re-created in",
	"The Gimp for our texture/web version."
};

static const char *ul_menu[] = {
	"Me N U by Kudos.",
	"A playable in-game",
	"song."
};

static const char *ul_deserted[] = {
	"Deserted Town, an",
	"unreleased track",
	"by DJ Geki."
};

static const char *msg_deserted[] = {
	"Before we started on Feet of Fury,",
	"we were engaged in writing an RPG.",
	"We had a lot of plot written,",
	"character backgrounds, etc, and",
	"Roddy had started writing music with",
	"his (then) new Karma. This song was",
	"to be used for a desert town that",
	"was abandoned thousands of years",
	"before the RPG time setting. This",
	"is a work in progress."
};

static const char *ul_oldmenu[] = {
	"Nostalgia:",
	"Old Main Menu"
};

static const char *msg_oldmenu[] = {
	"Old Main Menu",
	" ",
	"Way back when FoF was just getting",
	"started, the main menu was one of the",
	"first pieces to be created, believe",
	"it or not. This is a screen shot of",
	"that original menu concept. It's",
	"changed quite a lot since then."
};

static const char *ul_noodles[] = {
	"Those Damn Noodles",
	"Late Night Mix by",
	"DJ Geki. A playable",
	"in-game song."
};

static const char *ul_casino[] = {
	"Casino of Fury.",
	"Try your luck at",
	"the slots for more",
	"unlock credits."
};

static const char *ul_yuki[] = {
	"Artwork:",
	"Yuki, the CA mascot,",
	"shows us her opinion",
	"of the DC and its",
	"competitors."
};

static const char *msg_yuki[] = {
	"CA's Mascot: Yuki",
	" ",
	"Bard says:",
	"\"We were brainstorming for t-shirt",
	"ideas a while back and one of the",
	"ideas that was thrown around was",
	"an anime girl clutching her DC.",
	"Our art guy got too busy to actually",
	"do it, so I went ahead and did it.",
	"A different version of this pic is",
	"available on a T-Shirt. See our web",
	"site for more info.\"",
	"",
	"www.cagames.com"
};

static const char *ul_furybattle[] = {
	"Design notes:",
	"Fury Battle"
};

static const char *msg_furybattle[] = {
	"Fury Battle",
	" ",
	"At one point we had discussed adding",
	"a whole new gameplay mode to the",
	"game that would make it much more",
	"like a traditional fighting game.",
	"This silly sketch was created to",
	"explain that. We ended up deciding",
	"to stick to the current core",
	"gameplay and maybe use it in a",
	"sequel if that happens. Behold",
	"Bard's mad stick figure art. Dom",
	"should definitely beware."
};

static const char *ul_house[] = {
	"House by Aaron",
	"Marks. A playable",
	"in-game song."
};

static const char *ul_newgameflow[] = {
	"Design notes:",
	"New Game Flow"
};

static const char *msg_newgameflow[] = {
	"New Game Flow",
	" ",
	"At one point we decided to redo the",
	"flow chart for the game, and this",
	"diagram in Bard's notebook was the",
	"result. This diagram is fairly close",
	"to what the game actually does."
};

static const char *ul_charselect[] = {
	"Design notes:",
	"Character Select"
};

static const char *msg_charselect[] = {
	"Character Select",
	" ",
	"Once we got the basics of the game",
	"down, we started in on menu creation",
	"in general. This is an early sketch",
	"of the character select menu."
};

static const char *ul_loungin[] = {
	"Loungin' by DJ",
	"Geki. A playable",
	"in-game song."
};

static const char *ul_newmenu[] = {
	"Design notes:",
	"New Menu Layout"
};

static const char *msg_newmenu[] = {
	"New Menu Layout",
	" ",
	"Once we decided to do away with the",
	"old menu layout, this was the plan",
	"for the new one. It is fairly close",
	"to what was eventually created. This",
	"menu was also the reason why the",
	"Tsunami scene graph library was",
	"created. It is now an indispensible",
	"tool throughout Feet of Fury."
};

static const char *ul_pico[] = {
	"Pico. A playable",
	"snowman character.",
	"This gives you the",
	"Phasing Arrows",
	"technique."
};

static const char *ul_furycube[] = {
	"Design notes:",
	"Fury Cube"
};

static const char *msg_furycube[] = {
	"Fury Cube",
	" ",
	"Sounds like some kind of weird new",
	"kids' TV show, right? The Fury Cube",
	"was designed as a selector for a",
	"player's 'fury', which was to be a",
	"sort of technique classification.",
	"The idea was that you could only",
	"receive items from your fury during",
	"a round of gameplay. This system may",
	"yet make a reappearance in a sequel",
	"if we make one, but was deemed too",
	"complex and troublesome in FoF."
};

static const char *ul_2ndmix[] = {
	"2ndMix Extended, a",
	"DC Tonic track from",
	"DJ Geki."
};

static const char *msg_2ndmix[] = {
	"Almost exactly two years ago,",
	"Cryptic Allusion released the",
	"DC Tonic CD at E3 in Los Angeles.",
	"This song was the audio track",
	"from that CD."
};

static const char *ul_quickly[] = {
	"Quickly by Chojin.",
	"A playable in-game",
	"song."
};

static const char *ul_extraslayout[] = {
	"Design notes:",
	"Extras Layout"
};

static const char *msg_extraslayout[] = {
	"Extras Layout",
	" ",
	"A basic flowchart describing how the",
	"extras/options menus would be laid out.",
	"The top part of the diagram is an",
	"object heirarchy, and the bottom is",
	"the actual menu layout itself. There's",
	"a hint in this diagram about something",
	"you may not have unlocked yet, if you",
	"look closely. :)"
};

static const char *ul_myheart[] = {
	"My Heart, an",
	"unreleased track",
	"by DJ Geki."
};

static const char *msg_myheart[] = {
	"Before we started on Feet of Fury,",
	"we were engaged in writing an RPG.",
	"We had a lot of plot written,",
	"character backgrounds, etc, and",
	"Geki had started writing music with",
	"his (then) new Karma. This song was",
	"meant to be used for a love scene",
	"between two characters. This is a",
	"work in progress."
};

static const char *ul_bgeffects[] = {
	"Design notes:",
	"Gameplay Background",
	"Effects"
};

static const char *msg_bgeffects[] = {
	"Gameplay Background Effects",
	" ",
	"Another page from Bard's large notebook",
	"of all game design goodness. This page",
	"details a set of background effects that",
	"are used during gameplay. Some of them",
	"actually made it in the game, some didn't.",
	"There is also a description of some",
	"transition effects, but those also didn't",
	"make it in."
};

static const char *ul_trainguy[] = {
	"Japanese train",
	"announcer voices"
};

static const char *msg_trainguy[] = {
	"Members of CA who shall remain",
	"anonymous (ahem) recoded these samples",
	"a while back to sound like the Japanese",
	"train conductors in Tokyo after being",
	"inspired by SuperBellz. The idea was to",
	"let the user select from several",
	"different announcer styles, and this",
	"was one potential one. Needless to say,",
	"we never actually did that. Beware the",
	"silly gaijin with bad accents!"
};

static const char *ul_sedary[] = {
	"Sedary Gatisan. A",
	"playable dragon",
	"character created",
	"by FoxxFire. This",
	"gives you the",
	"Dispel technique."
};

static const char *ul_djgekithanks[] = {
	"A recorded message",
	"from DJ Geki."
};

static const char *msg_djgekithanks[] = {
	"Ok, we admit it, we were just looking",
	"for silly things to put in here that",
	"might amuse all of you. Here is DJ Geki",
	"talking for a while, and getting the",
	"beat down from his roommate Alan."
};

static const char *ul_wavetxr[] = {
	"Design notes:",
	"Wave Banner Math"
};

static const char *msg_wavetxr[] = {
	"Wave Banner Math",
	" ",
	"This effect was originally designed",
	"for a demo called 'Aquatic', but since",
	"that never happened it was made into a",
	"Feet of Fury background effect during",
	"gameplay. The idea was to make it look",
	"like a texture was pasted onto a banner",
	"that is waving in a pool of water. Due",
	"to some issues with the algorithms,",
	"this effect is still not used to its",
	"full potential in FoF, so you may see",
	"it again later."
};


Unlock::Unlockable Unlock::grid[49] = {
	DECL(ul_junky, 2500, "junky"),					// 0
	DECL_W_INFO(ul_aquatic, msg_aquatic, 800, "song"),
	DECL(ul_dreams2, 2000, "song"),
	DECL_ART(ul_traycard, msg_traycard, 800, "dr_03.dat"),
	DECL_W_INFO(ul_cereal, msg_cereal, 800, "song"),
	DECL(ul_charlotte, 2500, "charlotte"),				// 5
	DECL_ART(ul_songmenumath, msg_songmenumath, 800, "dr_06.dat"),
	DECL_ART(ul_gameplaylayout, msg_gameplaylayout, 800, "dr_07.dat"),
	DECL_W_INFO(ul_fonky, msg_fonky, 800, "song"),
	DECL_W_INFO(ul_gnc, msg_gnc, 800, "song"),
	DECL_ART(ul_arrows1, msg_arrows1, 800, "dr_10.dat"),		// 10
	DECL_W_INFO(ul_radio, msg_radio, 800, "song"),
	DECL(ul_vengeance, 2500, "vengeance"),
	DECL_ART(ul_dcman, msg_dcman, 800, "dr_13.dat"),
	DECL(ul_ramp, 2000, "song"),
	DECL_ART(ul_bf, msg_bf, 800, "dr_15.dat"),			// 15
	DECL_ART(ul_tubemath, msg_tubemath, 800, "dr_16.dat"),
	DECL_W_INFO(ul_movieguy, msg_movieguy, 1000, "sound"),
	DECL(ul_bhedu, 2000, "song"),
	DECL_ART(ul_arrows2, msg_arrows2, 800, "dr_19.dat"),
	DECL_W_INFO(ul_funny, msg_funny, 1000, "sound"),		// 20
	DECL_W_INFO(ul_comeon, msg_comeon, 800, "song"),
	DECL_W_INFO(ul_bug, msg_bug, 400, "sound"),
	DECL_ART(ul_attackicons, msg_attackicons, 800, "dr_23.dat"),
	DECL(ul_bufflsr, 2000, "song"),
	DECL_ART(ul_calogo, msg_calogo, 800, "dr_25.dat"),		// 25
	DECL(ul_menu, 2000, "song"),
	DECL_W_INFO(ul_deserted, msg_deserted, 800, "song"),
	DECL_ART(ul_oldmenu, msg_oldmenu, 800, "dr_28.dat"),
	DECL(ul_noodles, 2000, "song"),
	DECL(ul_casino, 5000, "misc"),					// 30
	DECL_ART(ul_yuki, msg_yuki, 800, "dr_31.dat"),
	DECL_ART(ul_furybattle, msg_furybattle, 800, "dr_32.dat"),
	DECL(ul_house, 2000, "song"),
	DECL_ART(ul_newgameflow, msg_newgameflow, 800, "dr_34.dat"),
	DECL_ART(ul_charselect, msg_charselect, 800, "dr_35.dat"),	// 35
	DECL(ul_loungin, 2000, "song"),
	DECL_ART(ul_newmenu, msg_newmenu, 800, "dr_37.dat"),
	DECL(ul_pico, 2500, "pico"),
	DECL_ART(ul_furycube, msg_furycube, 800, "dr_39.dat"),
	DECL_W_INFO(ul_2ndmix, msg_2ndmix, 800, "song"),		// 40
	DECL(ul_quickly, 2000, "song"),
	DECL_ART(ul_extraslayout, msg_extraslayout, 800, "dr_42.dat"),
	DECL_W_INFO(ul_myheart, msg_myheart, 800, "song"),
	DECL_ART(ul_bgeffects, msg_bgeffects, 800, "dr_44.dat"),
	DECL_W_INFO(ul_trainguy, msg_trainguy, 1000, "sound"),		// 45
	DECL(ul_sedary, 2500, "sedary"),
	DECL_W_INFO(ul_djgekithanks, msg_djgekithanks, 1000, "sound"),
	DECL_ART(ul_wavetxr, msg_wavetxr, 800, "dr_48.dat")
};


// Index maps
int Unlock::chars[CHAR_COUNT] = {
	-1, -1, -1, -1,
	0, 5, 12, 38, 46
};

int Unlock::sounds[36] = {
	-2, 18, -2, 33, -2, 26,
	-2, 24, 2, 14, -2, -2,
	-2, -2, 29, -2, -2, 41,
	-2, -2, 36, -2, 1, 4,
	8, 9, 11, 17, 20, 22,
	21, 47, 40, 45, 27, 43
};
