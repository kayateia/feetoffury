/*
   Feet of Fury

   title.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"
#include "title.h"
#include "ogg.h"

#include "scrollybg.h"

// Textures
static RefPtr<Texture> txr_logo;

// Scrolly bg
static RefPtr<ScrollyBG> scrolly;

// Font helper
static RefPtr<Font> fnt;

// Frame counter
static int frame;

// Alpha value
static float alpha;

// When did we start? (timeout tracking)
static uint64 start_time;

// Our event listener.
static RefPtr<Hid::EventCollector> collector;

static int checkCont() {
	uint64 tm = Time::gettime() / 1000000;
	if ((tm - start_time) > 60)
		return 2;

	Hid::Event evt;
	
	if (!collector->getEvent(evt))
		return 0;
		
	if (evt.type == Hid::Event::EvtQuit)
		return -1;
	
	if (evt.type == Hid::Event::EvtKeypress || evt.type == Hid::Event::EvtBtnPress)
		return 1;

	return 0;
}

static void drawFrame() {
	if (alpha < 0.0f)
		alpha = 0.0f;

	Frame::begin();
	scrolly->setTint(Color(alpha,1,1,1));
	scrolly->drawAll(Drawable::Opaque);
	scrolly->nextFrame(0);

	Frame::transEnable();

	uint32 color = plx_pack_color(alpha, 1.0f, 1.0f, 1.0f);

	txr_logo->select();
	plx_mat_identity();
	plx_mat_translate(320.0f, 200.f, 15.0f);
	plx_mat_rotate_z( ((alpha * -90.0f) - 270.0f) * 2*M_PI / 360.0f );
	plx_spr_imp(
		2.0f * alpha * 256,
		2.0f * alpha * 256,
		0.0f, 0.0f, 0.0f,
		color);

	// Happy Lucky Dance Party
	fnt->setColor(Color(alpha, 0.85f, 0.85f, 0.85f));
	fnt->setSize(30.0f);
	fnt->smearDrawCentered(Vector(320.0f, 365.0f, 128.0f), _("Mobile Tactical Dancing Action"));

	fnt->setSize(36.0f);
	fnt->setAlpha(alpha * (0.5f + (fsin(frame * 2*M_PI/hz)/2)));
	fnt->smearDrawCentered(Vector(320.0f, 422.0f, 128.0f), _("Press Start!"));

	Frame::finish();
}

// Do the title screen; returns false if it timed out         
//   or true if the user hit START.
Title::Result Title::doTitle() {
	// Load textures

	txr_logo = res.getTexture(Res::Title, "title", true);

	scrolly = new ScrollyBG(res.getTexture(Res::Preload, "scrolly_logo"));
	scrolly->setTranslate(Vector(0,0,10));

	// Load sounds
	RefPtr<Sound> sfx_announce = res.getSound(Res::Title, "FeetOfFury_super_echo");
	RefPtr<Sound> sfx_accept = res.getSound(Res::Preload, "title_accept");

	fnt = res.getFont("axaxax");

	oggVolume(1.0f);
	oggStart("bgm/title.ogg", true, false, false);

	collector = new Hid::EventCollector();
	start_time = Time::gettime() / 1000000;

	frame = 0;
	alpha = 0.0f;
	bool done = false, quitting = false, timedout = false;
	int hitstart;
	while (!done) {
		hitstart = checkCont();
		if (hitstart == -1)
			goto quitOut;
			
		if (!quitting && hitstart == 1)
			sfx_accept->play();
		if (hitstart == 2)
			timedout = true;
		quitting = quitting || hitstart;
		
		drawFrame();

		if (!quitting && alpha < 1.0f) {
			alpha += 0.05f;
			if (alpha >= 1.0f) {
				sfx_announce->play();
				alpha = 1.0f;
			}
		} else if (quitting && alpha > 0.0f)
			alpha -= 0.1f;
		else if (quitting && alpha <= 0.0f)
			done = true;
		frame++;

		if (quitting) {
			oggVolume(alpha);
		}
	}

	alpha = 0.0f;
	oggStop(false);

	drawFrame();
	drawFrame();
	drawFrame();

	// Give a bit of time for the sounds to stop gracefully
	if (!timedout) {
		Debug::printf("being graceful\n");
	//	Time::sleep(1000*500);
	}

quitOut:
	fnt = NULL;
	scrolly = NULL;
	txr_logo = NULL;
	collector = NULL;

	Sound::stopAll();

	if (hitstart == -1)
		return Reset;
	else
		return timedout ? Timeout : Start;
}

