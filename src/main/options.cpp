/*
   Feet of Fury

   options.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "options.h"
#include "saving.h"
#include "unlock.h"
#include "ogg.h"

// The one and only...
Options game_options;

#define CURRENT_VERSION 0x01070000

Options::Options() {
	// Some reasonable defaults
	auto_save = true;
	cont_mode[0] = cont_mode[1] = 0;
	vol_music = 80;
	vol_sounds = 80;
	announcer = true;
	snd_mono = false;
	ai_skill = 85;
	eor_style = 1;
	attack_duration = 4;

	// Start off with half the characters locked
	for (int i=0; i<CHAR_COUNT; i++)
		if (i < 4 || i == (CHAR_COUNT-1))
			char_unlocked[i] = true;
		else
			char_unlocked[i] = false;

	unlock_pts = 0;
	unlocked[0] = unlocked[1] = 0;

	song_played[0] = song_played[1] = 0;

	practiceBG = false;
	practiceBeatBars = true;
	practiceClapTrack = true;

	// XXX
/* 	buttonSettings[Up1] = CONT_DPAD_UP;
	buttonSettings[Up2] = CONT_Y;
	buttonSettings[Left1] = CONT_DPAD_LEFT;
	buttonSettings[Left2] = CONT_X;
	buttonSettings[Right1] = CONT_DPAD_RIGHT;
	buttonSettings[Right2] = CONT_B;
	buttonSettings[Down1] = CONT_DPAD_DOWN;
	buttonSettings[Down2] = CONT_A;
	buttonSettings[UseItem] = CONT_RTRIG; */

	allowSystemMenu = true;

	dirty = true;
	version = CURRENT_VERSION;
	// m_save_done = true;
}

Options::~Options() {
}

static uint32 pal[16] = {
	0x00000000,
	0xff020202,
	0xff02fe32,
	0xfffe0292,
	0xff06fe16,
	0xffbede02,
	0xfffefefe,
	0xff0e0afe,
	0xffd202fe,
	0xff02eec6,
	0xff027afe,
	0xff6e02fe,
	0xfffe160a,
	0xff0252fe,
	0xfffe4102,
	0xff0226fe
};

bool Options::save(ScrollyBG * oldbg) {
#if 0
	// Don't save needlessly
	if (!dirty || !auto_save)
		return true;

	// Create something for the user to look at while they're waiting
	Saving::start(oldbg);

	// Find a VMU to save on
	maple_device_t * dev;
	if (!saveableVmuPresent(NULL, &dev)) {
		Saving::fail();
		return false;
	}

	uint8 buffer[512] = { 0 };
	int op = 0;

	// Version code
	version = CURRENT_VERSION;
	memcpy(buffer+op, &version, 4); op += 4;

	// Controller modes
	memcpy(buffer+op, &game_options.cont_mode[0], 4); op += 4;
	memcpy(buffer+op, &game_options.cont_mode[1], 4); op += 4;

	// Volumes
	memcpy(buffer+op, &game_options.vol_music, 4); op += 4;
	memcpy(buffer+op, &game_options.vol_sounds, 4); op += 4;

	// Announcer enable/disable
	int bt = game_options.announcer ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;

	// Sound mono
	bt = 0; 	// game_options.snd_mono ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;

	// AI skill
	memcpy(buffer+op, &game_options.ai_skill, 4); op += 4;

	// End-of-round style
	memcpy(buffer+op, &game_options.eor_style, 4); op += 4;

	// Attack duration
	memcpy(buffer+op, &game_options.attack_duration, 4); op += 4;

	// Unlocked character flags
	for (int i=0; i<CHAR_COUNT; i++) {
		buffer[op+i] = game_options.char_unlocked[i] ? 1 : 0;
	}
	op += CHAR_COUNT;

	// Unlock points
	memcpy(buffer+op, &game_options.unlock_pts, 4); op += 4;

	// General unlocked items
	memcpy(buffer+op, &unlocked[0], 4); op += 4;
	memcpy(buffer+op, &unlocked[1], 4); op += 4;

	// Song played flags
	memcpy(buffer+op, &song_played[0], 4); op += 4;
	memcpy(buffer+op, &song_played[1], 4); op += 4;

	// Practice mode settings
	bt = practiceBG ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;
	bt = practiceBeatBars ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;
	bt = practiceClapTrack ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;

	// Key settings
	for (int i=0; i<Count; i++) {
		memcpy(buffer+op, &buttonSettings[i], 4); op += 4;
	}

	bt = allowSystemMenu ? 1 : 0;
	memcpy(buffer+op, &bt, 4); op += 4;

	Debug::printf("Options::save: current save size is %d bytes\n", op);

	assert( op <= 512 );

	// Come up with a VMU header
	vmu_pkg_t	pkg;
	uint8		*pkg_out;
	int		pkg_size;

	memset(&pkg, 0, sizeof(pkg));
	strcpy(pkg.desc_short, "FOF System");
	strcpy(pkg.desc_long, "Feet of Fury System Data");
	strcpy(pkg.app_id, "Feet of Fury");
	pkg.icon_cnt = 3;
	pkg.icon_anim_speed = 0x1f;
	pkg.eyecatch_type = VMUPKG_EC_NONE;
	pkg.eyecatch_data = NULL;
	pkg.data_len = op;
	pkg.data = buffer;

	// Write out the palette
	for (int i=0; i<16; i++) {
		int a = (pal[i] >> 28) & 0x0f,
		    r = (pal[i] >> 20) & 0x0f,
		    g = (pal[i] >> 12) & 0x0f,
		    b = (pal[i] >> 4) & 0x0f;
		pkg.icon_pal[i] =
			(a << 12) |
			(r << 8) |
			(g << 4) |
			(b << 0);
	}

	// Write out the icon data
	pkg.icon_data = new uint8[512 * 3];
	op = 0;
	for (int i=0; i<3; i++) {
		for (int j=0; j<32*32; j+=2) {
			int p1 = chtopal(vmu_icons[i][j]);
			int p2 = chtopal(vmu_icons[i][j+1]);
			pkg.icon_data[op] = (p1 << 4) | p2;
			op++;
		}
	}
	assert( op == 512*3 );

	// Build the full VMU package
	vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

	// Commit the changes
	if (vmufs_write(dev, "FOF_SYS", pkg_out, pkg_size, VMUFS_OVERWRITE) < 0) {
		Saving::fail();
	}

	delete[] pkg.icon_data;
	free(pkg_out);

	game_options.dirty = false;

	// Ok, all done
	Saving::finish();
#endif

	return true;
}

#if 0
bool Options::saveableVmuPresent(char *name, maple_device_t ** devout) {
	for (int p=0; p<MAPLE_PORT_COUNT; p++) {
		for (int u=0; u<MAPLE_UNIT_COUNT; u++) {
			maple_device_t * dev = maple_enum_dev(p, u);
			if (dev && (dev->info.functions & MAPLE_FUNC_MEMCARD)) {
				if (vmufs_free_blocks(dev) >= 2) {
					if (name) {
						name[0] = 'A' + p;
						name[1] = '0' + u;
					}
					if (devout) {
						*devout = dev;
					}
					return true;
				}
			}
		}
	}

	return false;
}
#endif

bool Options::load() {
#if 0
	Debug::printf("Options::load: looking for a system file\n");

	// Open the VMU directory
	file_t vd = fs_open("/vmu", O_RDONLY | O_DIR);
	if (vd == FILEHND_INVALID) {
		printf("Options::load: couldn't open /vmu\n");
		return false;
	}

	// Look through each attached VMU for an FoF system file
	dirent_t * d;
	file_t f = FILEHND_INVALID;
	while ( (d = fs_readdir(vd)) ) {
		char name[256]; sprintf(name, "/vmu/%s/FOF_SYS", d->name);
		f = fs_open(name, O_RDONLY);
		if (f != FILEHND_INVALID)
			break;
	}
	fs_close(vd);
	if (f == FILEHND_INVALID) {
		printf("Options::load: no attached VMUs with system file\n");
		return false;
	}

	// Ok, we found one -- skip the header and load the data from it
	fs_seek(f, 128 + 512*3, SEEK_SET);

	fs_read(f, &version, 4);
	if ((version & 0xff000000) != 0x01000000) {
		printf("Options::load: system file found, but incompatible version\n");
		fs_close(f);
		return false;
	}
	printf("Options::load: system file version %08lx found\n", version);

	fs_read(f, &cont_mode[0], 4);
	fs_read(f, &cont_mode[1], 4);
	fs_read(f, &vol_music, 4);
	fs_read(f, &vol_sounds, 4);

	oggVolume(1.0f);
	Sound::setDefaultVolume(vol_sounds * 255 / 100);

	int bt;
	fs_read(f, &bt, 4); announcer = bt ? true : false;
	fs_read(f, &bt, 4); // snd_mono = bt ? true : false;
	fs_read(f, &ai_skill, 4);
	fs_read(f, &eor_style, 4);
	fs_read(f, &attack_duration, 4);

	if ((version & 0x00ff0000) >= 0x00010000) {
		char p;

		// Unlocked character flags
		for (int i=0; i<CHAR_COUNT; i++) {
			fs_read(f, &p, 1);
			if (p)
				char_unlocked[i] = true;
			else
				char_unlocked[i] = false;
		}
	}
	if ((version & 0x00ff0000) >= 0x00020000) {
		fs_read(f, &unlock_pts, 4);
	} else if ((version & 0x00ff0000) == 0x00000000) {
		printf("Options::load: old-style FoF save detected, bonus points awarded!\n");
		unlock_pts = 500;
	}
	printf("Options::load: user has %lu unlock points\n", unlock_pts);

	if ((version & 0x00ff0000) >= 0x00030000) {
		fs_read(f, &unlocked[0], 4);
		fs_read(f, &unlocked[1], 4);
	}

	// Setup unlocked character array as well
	for (int i=0; i<Unlock::count; i++) {
		if (isUnlocked(i))
			unlock(i);
	}

	if ((version & 0x00ff0000) >= 0x00040000) {
		fs_read(f, &song_played[0], 4);
		fs_read(f, &song_played[1], 4);
	}

	if ((version & 0x00ff0000) >= 0x00050000) {
		fs_read(f, &bt, 4);	practiceBG = bt ? true : false;
		fs_read(f, &bt, 4);	practiceBeatBars = bt ? true : false;
		fs_read(f, &bt, 4);	practiceClapTrack = bt ? true : false;
	}

	if ((version & 0x00ff0000) >= 0x00060000) {
		for (int i=0; i<Count; i++) {
			fs_read(f, &buttonSettings[i], 4);
		}
	}

	if ((version & 0x00ff0000) >= 0x00070000) {
		fs_read(f, &bt, 4);	allowSystemMenu = bt ? true : false;
	}

	printf("Options::load: system file loaded\n");

	fs_close(f);

	// Grab the mono/stereo setting from the BIOS, not save file
	flashrom_syscfg_t cfg;
	if (flashrom_get_syscfg(&cfg) >= 0) {
		snd_mono = !cfg.audio;
	}

	if (version != CURRENT_VERSION)
		game_options.dirty = true;
	else
		dirty = false;

	//dirty = true;
#endif

	return true;
}

bool Options::isLocked(int idx) {
	return (unlocked[idx / 32] & (1 << (idx % 32))) ? false : true;
}

bool Options::isUnlocked(int idx) {
	return !isLocked(idx);
}

void Options::unlock(int idx) {
	unlocked[idx / 32] |= 1 << (idx % 32);

	// Also unlock the specific character on those
	switch (idx) {
	case Unlock::CharMetalJunky:
		char_unlocked[4] = true; break;
	case Unlock::CharCharlotte:
		char_unlocked[5] = true; break;
	case Unlock::CharVengeance:
		char_unlocked[6] = true; break;
	case Unlock::CharPico:
		char_unlocked[7] = true; break;
	case Unlock::CharSedary:
		char_unlocked[8] = true; break;
	}

	dirty = true;
}

bool Options::isSongPlayed(int idx) {
	return (song_played[idx / 32] & (1 << (idx % 32))) ? false : true;
}

void Options::setSongPlayed(int idx) {
	song_played[idx / 32] |= 1 << (idx % 32);

	dirty = true;
}
