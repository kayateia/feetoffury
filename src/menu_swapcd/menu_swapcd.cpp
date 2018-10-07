/*
   Feet of Fury

   menu_swapcd/menu_swapcd.cc

   Copyright (C)2002,2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_swapcd.h"

#if 0
#include "genmenu.h"
#include "textset.h"
#include "main_cdtray.h"
#include "res.h"

#include "starfield.h"
#include "gridselector.h"
#include "rotbanner.h"
#include "textdisplay.h"
#include "selbar.h"
#include "popup.h"
#include "messagebox.h"

CVSID("$Id: menu_swapcd.cc,v 1.22 2003/05/13 06:32:33 bard Exp $");

// Put everything in a class wrapper to make the ref counting easier
class MenuSwapCD : public GenericMenu {
public:
	MenuSwapCD(bool swapInFoF);
	virtual ~MenuSwapCD();

	virtual void startExit();
	virtual void inputEvent(const Event & evt);
	virtual void visualPerFrame();

	// Check to see if we've got the FoF CD in the drive
	static bool isFoFCD();

	bool getResults(char * outname);
	int getMode() const { return mode_option; }

	void cdCheck(int last_status, int cur_status);
private:
	// Methods /////////////////////////////////////////////////////////
	// Part 1: swap CD selection
	void forceUpdate();
	void getFileList();
	void setFoFCD();
	void setOtherCD();
	void setFoFCommon(bool reallyfof);
	void setNoCD();
	bool checkFile(const char *fn);

	// Part 2: mode selection
	void switchToModeSelect();
	void inputModeSelect(const Event & evt);
	
	// Variables ///////////////////////////////////////////////////////
	// Mutex for modifying any of the below
	mutex_t	* mutex;

	// Starfield in the background
	RefPtr<StarField> sf;

	// The grid selector for the set selection
	RefPtr<GridSelector> gs;

	// Textures for the Swap CD sets
	RefPtr<Texture> gs_txrs[12];

	// Info texts for Swap CD sets
	RefPtr<TextSet> gs_texts[12], ts_missing;
	RefPtr<Text> gs_names[12];

	// Our font
	RefPtr<Font> font;

	// Rotating banner for the CD thumbnail
	RefPtr<RotBanner> rb;
	RefPtr<Texture> rb_txr;

	// Texture for the FoF CD itself
	RefPtr<Texture>	fofcd_txr;

	// Menu label
	RefPtr<Label> label;

	// Text display for option descriptions
	RefPtr<TextDisplay> td;

	// Message box for keyboard/controller warning popup
	RefPtr<MessageBox> kbmb;

	// System menu popup (if applicable)
	RefPtr<Popup> sysmenu;
	RefPtr<Scene> subscene;

	// Sound effects
	RefPtr<Sound> sfx_select, sfx_cursor, sfx_invalid;

	// Mode Selection Stuff ////////////////////////////////////////////////
	// Selection bar for mode selection
	RefPtr<SelBar>		sel_bar;

	int			mode_option;

	// Misc ////////////////////////////////////////////////////////////////
	// Selected menu option
	int cur_option;

	// Is the FoF CD in the tray?
	volatile bool fof_in_tray;

	// What mode are we in?
	enum Mode {
		ModeSelectCD,
		ModeInsertFoF,
		ModeSelectMode
	};
	Mode	mode;
};

#define GRIDW	3
#define GRIDH	4

////////////////////////////////////////////////////////////////////////////

void MenuSwapCD::forceUpdate() {
	Event evt;
	evt.type = Event::EvtKeypress;
	evt.key = Event::KeyPgup;
	inputEvent(evt);
}

// Call to set that we're on the FoF CD itself
void MenuSwapCD::setFoFCommon(bool reallyfof) {
	const char * txtorig = "The Feet of Fury CD\n"
				 "or some other non-SwapCD\n"
				 "is in the drive. Please\n"
				 "swap in the CD you want\n"
				 "to play songs from.\n";
	char * txt = new char[strlen(txtorig)+1];
	strcpy(txt, txtorig);
	
	TextSet * ts = new TextSet(txt);
	Text * t = new Text("");

	mutex_lock(mutex);
	
	for (int i=0; i<GRIDW*GRIDH; i++) {
		gs_txrs[i] = NULL;
		if (gs)
			gs->setTextureAt(i % GRIDW, i / GRIDW, gs_txrs[i], gs_txrs[i]);
		gs_texts[i] = ts;
		gs_names[i] = t;
	}

	rb_txr = fofcd_txr;
	rb->setTexture(rb_txr);

	forceUpdate();

	mutex_unlock(mutex);

	fof_in_tray = reallyfof;
}

void MenuSwapCD::setFoFCD() {
	setFoFCommon(true);
}

void MenuSwapCD::setOtherCD() {
	setFoFCommon(false);
}

// Call to set that there's no CD available
void MenuSwapCD::setNoCD() {
	const char * txtorig = "No CD is in the drive.\n"
				"Please insert the CD you\n"
				"want to play songs from.\n";
	char * txt = new char[strlen(txtorig)+1];
	strcpy(txt, txtorig);

	RefPtr<TextSet> ts = new TextSet(txt);
	RefPtr<Text> t = new Text("");

	mutex_lock(mutex);

	// In swap-in-FoF mode, we don't want to remove the texture
	if (gs) {
		for (int i=0; i<GRIDW*GRIDH; i++) {
			gs_txrs[i] = NULL;
			if (gs)
				gs->setTextureAt(i % GRIDW, i / GRIDW, gs_txrs[i], gs_txrs[i]);
			gs_texts[i] = ts;
			gs_names[i] = t;
		}

		rb_txr = NULL;
		rb->setTexture(NULL);
	}

	forceUpdate();

	mutex_unlock(mutex);
}

// Check for the presence of a file
bool MenuSwapCD::checkFile(const char *fn) {
	file_t f = fs_open(fn, O_RDONLY);
	if (f == FILEHND_INVALID) {
		return false;
	} else {
		fs_close(f);
		return true;
	}
}

bool MenuSwapCD::isFoFCD() {
	file_t f;

	f = fs_open("/cd/1st_read.bin", O_RDONLY);
	if (f == FILEHND_INVALID) {
		return false;
	}
	fs_close(f);

	f = fs_open("/cd/fof/bgm/about.ogg", O_RDONLY);
	if (f == FILEHND_INVALID) {
		return false;
	}
	fs_close(f);

	f = fs_open("/cd/fof/pakfiles/gameplay.pak", O_RDONLY);
	if (f == FILEHND_INVALID) {
		return false;
	}
	fs_close(f);

	return true;
}

// Read the Swap CD to get the list of available sets
void MenuSwapCD::getFileList() {
	// First of all, do some checks to see if it's an FoF CD.. if so, we
	// don't want to allow its usage as a Swap CD (if that's even possible).
	if (isFoFCD()) {
		Debug::printf("detected FoF CD, not allowing\n");
		setFoFCD();
		return;
	}

	// If we get here, there's a fairly decent chance this isn't
	// the FoF CD in the drive. If it is, we've been outsmarted by
	// the hackers.. *sigh* That would take some doing though.

	// Check to see if it's a SwapCD
	file_t f = fs_open("/cd/SwapCD", O_RDONLY | O_DIR);
	if (f == FILEHND_INVALID) {
		Debug::printf("detected Other CD, not allowing\n");
		setOtherCD();
		return;
	}
	fs_close(f);

	// Open up the CD's directory listing
	f = fs_open("/cd", O_RDONLY | O_DIR);
	if (f == FILEHND_INVALID) {
		setNoCD();
		return;
	}

	// Load up the overall CD texture
	Texture * nrbtxr = NULL;
	if (checkFile("/cd/SwapCD/thumb.png"))
		nrbtxr = new Texture("/cd/SwapCD/thumb.png", true);
	mutex_lock(mutex);
	rb_txr = nrbtxr;
	rb->setTexture(rb_txr);
	mutex_unlock(mutex);

	// Clean out all the initial data
	mutex_lock(mutex);
	for (int i=0; i<12; i++) {
		gs_txrs[i] = NULL;
		if (gs)
			gs->setTextureAt(i % GRIDW, i / GRIDW, gs_txrs[i], gs_txrs[i]);
		gs_texts[i] = NULL;
		gs_names[i] = NULL;
	}
	forceUpdate();
	mutex_unlock(mutex);

	// Read through and load up all the pieces we want
	dirent_t *d;
	int cnt = 0;
	char tmp[256];
	while ( (d = fs_readdir(f)) ) {
		Debug::printf("dirent: '%s'\n", d->name);

		if (d->name[0] == '_')
			continue;
		if (!stricmp(d->name, "SwapCD"))
			continue;

		// Load up the thumbnail pic
		sprintf(tmp, "/cd/%s/thumb.png", d->name);
		Texture * new_gs_txr;

		if (!checkFile(tmp))
			new_gs_txr = NULL;
		else
			new_gs_txr = new Texture(tmp, true);
		TextSet * new_gs_texts = NULL;

		// Load up the info text
		sprintf(tmp, "/cd/%s/info.txt", d->name);
		file_t itf = fs_open(tmp, O_RDONLY);
		if (itf == FILEHND_INVALID)
			new_gs_texts = ts_missing;
		else {
			char * tmp2 = new char[fs_total(itf)];
			fs_read(itf, tmp2, fs_total(itf));
			fs_close(itf);

			new_gs_texts = new TextSet(tmp2);
		}

		mutex_lock(mutex);
		gs_txrs[cnt] = new_gs_txr;
		if (gs)
			gs->setTextureAt(cnt % GRIDW, cnt / GRIDW, gs_txrs[cnt], gs_txrs[cnt]);
		gs_texts[cnt] = new_gs_texts;
		gs_names[cnt] = new Text(d->name);
		forceUpdate();
		mutex_unlock(mutex);

		cnt++;
	}
	fs_close(f);
}

void MenuSwapCD::cdCheck(int last_status, int status) {
	Debug::printf("CD status change to %d\n", status);

	switch (last_status) {
	// The CD drive was spinning up
	case CD_STATUS_BUSY:
		getFileList();
		break;

	// The CD drive was ready to go
	default:
		if (status == CD_STATUS_NO_DISC || status == CD_STATUS_OPEN)
			setNoCD();
		break;
	}
}

// Start the exodus for when the user has selected something
void MenuSwapCD::startExit() {
	m_scene->setFinished();
	GenericMenu::startExit();
}

// Do one frame of video goodness
void MenuSwapCD::visualPerFrame() {
	// Normal user interaction
	if (mode == ModeInsertFoF) {
		if (fof_in_tray)
			startExit();
	}

	m_scene->subRemoveFinished();

	// We really just have to do this by hand to handle the mutex
	// gracefully...
	pvr_wait_ready();
	pvr_scene_begin();

	mutex_lock(mutex);
	
	pvr_list_begin(PLX_LIST_OP_POLY);
	GenericMenu::visualOpaqueList();

	pvr_list_begin(PLX_LIST_TR_POLY);
	GenericMenu::visualTransList();
	
	mutex_unlock(mutex);

	pvr_scene_finish();

	m_scene->nextFrame();
}


// Check for control stuff
void MenuSwapCD::inputEvent(const Event & evt) {
	if (sysmenu) {
		sysmenu->inputEvent(evt);

		int rv = sysmenu->getResult();
		switch (rv) {
		case -2:
		case 1:
			subscene->animRemoveAll();
			subscene->animAdd(
				new TintFader(Color(1,1,1), Color(0.5f/(hz/2),0.5f/(hz/2),0.5f/(hz/2))));
			sysmenu = NULL;
			break;

		case 0:
			sysmenu = NULL;
			cur_option = -1;
			startExit();
			break;
		}

		return;
	}

	if (evt.type != Event::EvtKeypress)
		return;

	if (kbmb && evt.key != Event::KeyReset) {
		kbmb->startExit();
		kbmb = NULL;
		return;
	}

	// Not much to do here if we're in insert-fof mode
	if (mode == ModeInsertFoF) {
		switch (evt.key) {
		case Event::KeyReset:
#ifndef RUN_FROM_PC
			// Reset w/o the FoF CD .. nothing to do but bail
			arch_menu();
#else
			cur_option = -1;
			startExit();
#endif
			break;
		}

		return;
	} else if (mode == ModeSelectMode) {
		inputModeSelect(evt);
		return;
	}
	

	int x, y;
	gs->getSelection(0, &x, &y);

	bool updateGrid = false;
	switch (evt.key) {

	case Event::KeyReset:
		cur_option = -1;
		startExit();
		return;

	case Event::KeySelect:
		if (gs_txrs[y*GRIDW+x] == NULL) {
			sfx_invalid->play();
		} else {
			sfx_select->play();
			cur_option = y*GRIDW+x;
			switchToModeSelect();
		}
		break;

	case Event::KeyLeft:
		x--;
		updateGrid = true;
		break;

	case Event::KeyRight:
		x++;
		updateGrid = true;
		break;

	case Event::KeyUp:
		y--;
		updateGrid = true;
		break;

	case Event::KeyDown:
		y++;
		updateGrid = true;
		break;

	case Event::KeyStart:
		sysmenu = new Popup("System Menu", true);
		sysmenu->setTranslate(Vector(320,240,1000));
		sysmenu->addOption("Back to Main Menu");
		sysmenu->addOption("Cancel");
		m_scene->subAdd(sysmenu);

		subscene->animAdd(
			new TintFader(Color(0.5f, 0.5f, 0.5f), Color(-0.5f/(hz/2),-0.5f/(hz/2),-0.5f/(hz/2))));

		return;
	}

	if (updateGrid) {
		sfx_cursor->play();
		if (x < 0) x += GRIDW;
		if (x >= GRIDW) x -= GRIDW;
		if (y < 0) y += GRIDH;
		if (y >= GRIDH) y -= GRIDH;
		gs->setSelection(0, x, y);
	}

	TextSet * ts = gs_texts[y*GRIDW+x];
	if (ts != NULL)
		td->setText(ts->texts, ts->texts_cnt);
	else {
		static const char *blank[] = { "Not Available" };
		td->setText(blank, 1);
	}

	//resetTimeout();
}
////////////////////////////////////////////////////////////////////////////

void MenuSwapCD::switchToModeSelect() {
	// Swap out scene components
	subscene->subRemove(rb);
	subscene->subRemove(gs);
	subscene->subRemove(td);

	label->setText("Select Game Mode");

	Label * l = new Label(font, "Item Battle", 26, true, false);
	l->setTranslate(Vector(320.0f, 150.0f, 15.0f));
	subscene->subAdd(l);

	l = new Label(font, "Typing of Fury", 26, true, false);
	l->setTranslate(Vector(320.0f, 200.0f, 15.0f));
	subscene->subAdd(l);

	l = new Label(font, "Practice", 26, true, false);
	l->setTranslate(Vector(320.0f, 250.0f, 15.0f));
	subscene->subAdd(l);

	sel_bar->setTranslate(Vector(320.0f, 150.0f + 15.0f, 14.0f));
	subscene->subAdd(sel_bar);

	mode = ModeSelectMode;
	mode_option = 0;
}

void MenuSwapCD::inputModeSelect(const Event & evt) {
	switch (evt.key) {

	case Event::KeyReset:
		cur_option = -1;
		startExit();
		break;

	case Event::KeyUp:
		sfx_cursor->play();
		mode_option--;
		if (mode_option < 0)
			mode_option = 2;
		break;
		
	case Event::KeyDown:
		sfx_cursor->play();
		mode_option++;
		if (mode_option > 2)
			mode_option = 0;
		break;

	case Event::KeySelect: {
		bool ok = true;
		if (mode_option == 0 || mode_option == 2) {
			ok = false;
			for (int i=0; i<4; i++) {
				if (m_contTypes[i] & MAPLE_FUNC_CONTROLLER) {
					ok = true;
					break;
				}
			}

			if (!ok) {
				static const char *kbmsg[] = {
					"Item Battle and Practice Mode",
					"require at least one attached",
					"control pad or dance mat. We",
					"currently do not detect any",
					"such devices. If you wish to",
					"play this mode, attach one of",
					"them and try again."
		
				};
				kbmb = new MessageBox(18.0f);
				kbmb->setTranslate(Vector(320,240,1000));
				kbmb->setText(kbmsg, 7);
				m_scene->subAdd(kbmb);
			}
		} else if (mode_option == 1) {
			ok = false;
			for (int i=0; i<4; i++) {
				if (m_contTypes[i] & MAPLE_FUNC_KEYBOARD) {
					ok = true;
					break;
				}
			}

			if (!ok) {
				static const char *kbmsg[] = {
					"Typing of Fury requires at least one",
					"attached keyboard. We currently do",
					"not detect any keyboards. If you",
					"wish to play this mode, attach a",
					"keyboard and try again."
				};
				kbmb = new MessageBox(18.0f);
				kbmb->setTranslate(Vector(320,240,1000));
				kbmb->setText(kbmsg, 5);
				m_scene->subAdd(kbmb);
			}
		}
		if (ok) {
			sfx_select->play();
			startExit();
		}
		break;
	}

	}

	float locs[3] = { 150.0f, 200.0f, 250.0f };
	sel_bar->setTranslate(Vector(320.0f, locs[mode_option]+15.0f, 14.0f));
}

////////////////////////////////////////////////////////////////////////////

const char * text[] = {
	"[loading]"
};

void cdtcb(void *token, int a, int b) {
	((MenuSwapCD*)token)->cdCheck(a, b);
}

MenuSwapCD::MenuSwapCD(bool swapInFoF)
	: mode(swapInFoF ? ModeInsertFoF : ModeSelectCD)
{
	// Setup
	mutex = mutex_create();

	subscene = new Scene();
	m_scene->subAdd(subscene);
	
	sf = new StarField(512);
	sf->setTranslate(Vector(0,0,1));
	subscene->subAdd(sf);

	if (!swapInFoF)
		cur_option = 0;
	else
		cur_option = -1;
	fof_in_tray = !swapInFoF;
	cdtrayAllowOpen(true);

	// FoF CD cover
	fofcd_txr = new Texture("preload/gfx_misc/fofcd_preview.png", true);

	// Font
	font = new Font("preload/fonts/axaxax.txf");

	// Rotating banner
	rb = new RotBanner(NULL, 256, 256);
	rb->setTranslate(Vector(320.0f, 240.0f, 5.0f));
	subscene->subAdd(rb);

	if (swapInFoF)
		rb->setTexture(fofcd_txr);

	// Menu option label
	if (!swapInFoF)
		label = new Label(font, "Swap CD Setup", 36, true, false);
	else
		label = new Label(font, "Insert Game Disc", 36, true, false);
	label->setTranslate(Vector(320.0f, 80.0f, 15.0f));
	subscene->subAdd(label);

	const char * txtmissing = "Undescribed Set\n";
	char * txtmissingtmp = new char[strlen(txtmissing)+1];
	strcpy(txtmissingtmp, txtmissing);
	ts_missing = new TextSet(txtmissingtmp);

	if (!swapInFoF) {
		// Text display
		td = new TextDisplay(font, 20);
		td->setTranslate(Vector(640.0f - (128.0f + 64.0f), 270.0f, 20.0f));
		td->setText(text, 1);
		td->setBGAlpha(0.5f);
		subscene->subAdd(td);

		// Grid selector
		gs = new GridSelector(GRIDW, GRIDH, 64, 64, 1);
		gs->setSelAttrs(0, 1.0f, 0.3f, 0.3f, NULL);
		gs->setSelection(0, 0,0);
		gs->setTranslate(Vector(160, 270, 12));
		subscene->subAdd(gs);

		// Sound effects
		sfx_select = res.getSound(Res::Preload, "menu_accept");
		sfx_cursor = res.getSound(Res::Preload, "menu_cursor");
		sfx_invalid = res.getSound(Res::Preload, "invalid");

		getFileList();

		// Selection bar for mode (we'll use this later)
		sel_bar = new SelBar(560.0f, 26.0f, 4.0f);
	}

	// Yay C++!
	cdtraySetCallback(cdtcb, (void *)this);

	setBg(0.04f, 0.0f, 0.17f);

	// Start Ogg playing
	//oggStart("bgm/menu_main.ogg", true, false, false);

	resetTimeout();
}

MenuSwapCD::~MenuSwapCD() {
	cdtraySetCallback(NULL, NULL);
	cdtrayAllowOpen(false);
	mutex_destroy(mutex);
}

bool MenuSwapCD::getResults(char * outname) {
	if (cur_option >= 0) {
		strcpy(outname, *gs_names[cur_option]);
		return true;
	} else
		return false;
}

////////////////////////////////////////////////////////////////////////////
#endif	// 0

int doMenuSwapCD(char * outname, bool swapInFoF, int & modeOut) {
#if 0
	// Short-circuit this if there's already a FoF CD in the drive
	if (swapInFoF && MenuSwapCD::isFoFCD())
		return 0;

	MenuSwapCD m(swapInFoF);
	m.setPostDelay(500);
	m.doMenu();

	modeOut = m.getMode();

	return m.getResults(outname) ? 0 : -1;
#endif	// 0
	return 0;
}

