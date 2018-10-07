/*
   Feet of Fury

   res.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "res.h"

// Our one and only Res object
Res res;

Res::Res() {
}

Res::~Res() {
}


Texture * Res::getTexture(Domain dom, const char * name, bool useAlpha) {
	const char * prefix;
	// XXX
	// char tmp[strlen(name) + 64];
	char tmp[1024];

	switch (dom) {
	case MenuOptions:
	case Preload:		prefix = "preload/gfx_misc/"; break;
	
	case CaIntro:		prefix = "titles/ca_intro/"; break;
	case Title:		prefix = "titles/title/"; break;
	case MenuMain:		prefix = "titles/menu_main/"; break;
	case About:		prefix = "titles/about/"; break;
	case Unlock:		prefix = "titles/unlock/"; break;
	case Jukebox:		prefix = "titles/jukebox/"; break;
	case Slots:		prefix = "titles/slots/"; break;
	case Manual:		prefix = "manual/"; break;
	
	case MenuChar:		prefix = "menu_char/"; break;
	case MenuStage:		prefix = "menu_stage/"; break;
	case Gameplay:		prefix = "gameplay/"; break;

	default:
		assert(false);
		prefix = "";
		break;
	}

	Texture * t = new Texture();

	// First, try to load the texture as a KMG
	/* sprintf(tmp, "%s%s.kmg", prefix, name);
	if (t->loadFromFile(tmp, useAlpha, false))
		return t; */

	// Second, try to load the texture as a PNG
	sprintf(tmp, "%s%s.png", prefix, name);
	if (t->loadFromFile(tmp, useAlpha))
		return t;

	// Ok, try loading as a JPG
	sprintf(tmp, "%s%s.jpg", prefix, name);
	if (t->loadFromFile(tmp, false))
		return t;

	// Everything failed.. return failure
	RefPtr<Texture> tr = t;
	return NULL;
}

Sound * Res::getSound(Domain dom, const char * name) {
	const char * prefix;
	// XXX
	// char tmp[strlen(name) + 64];
	char tmp[1024];

	switch (dom) {
	case Preload:
	case CaIntro:
	case MenuMain:
	case About:
	case Jukebox:
	case MenuOptions:
	case MenuStage:	
	case Manual:
				prefix = "preload/sfx_misc/"; break;

	case MenuChar:
				prefix = "menu_char/"; break;
	
	case Unlock:
				prefix = "titles/unlock/"; break;

	case Slots:
				prefix = "titles/slots/"; break;

	case Title:		prefix = "titles/title/"; break;

	case Gameplay:		prefix = "gameplay/"; break;

	default:
		assert(false);
		prefix = "";
		break;
	}

	sprintf(tmp, "%s%s.wav", prefix, name);
	return new Sound(tmp);
}

Font * Res::getFont(const char * name) {
	// XXX
	// char tmp[strlen(name) + 32];
	char tmp[1024];

	sprintf(tmp, "preload/fonts/%s.txf", name);

	Font * f = new Font();
	if (f->loadFromFile(tmp))
		return f;

	RefPtr<Font> fr = f;
	return NULL;
}

FILE * Res::getBgm(const char * name) {
	// XXX
	// char tmp[strlen(name) + 16];
	char tmp[1024];

	sprintf(tmp, "bgm/%s.ogg", name);
	return fopen(tmp, "rb");
}

FILE * Res::getManPage(const char *name) {
	// XXX
	// char tmp[strlen(name) + 16];
	char tmp[1024];

	sprintf(tmp, "manual/%s.mnl", name);
	return fopen(tmp, "rb");
}

FILE * Res::getSongMain(const char * name) {
	// XXX
	// char tmp[strlen(name) + 32];
	char tmp[1024];

	sprintf(tmp, "songs/%s/song.ogg", name);
	return fopen(tmp, "rb");
}

FILE * Res::getSongSample(const char * name) {
	// XXX
	// char tmp[strlen(name) + 32];
	char tmp[1024];

	sprintf(tmp, "songs/%s/sample.ogg", name);
	return fopen(tmp, "rb");
}

Texture * Res::getSongThumbnail(const char * name) {
	return NULL;
}

Texture * Res::getSongBackground(const char * name) {
	return NULL;
}

char * Res::getSongSteps(const char * name) {
	return NULL;
}

Res::SongList * Res::getSongList() {
	return NULL;
}

