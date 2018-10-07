/*
   Feet of Fury

   gameplay/gameplay.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

/*
  This code is UUUUUUUGLY but there's no way I'm touching it at this
  late date. :)
 */

#include "global.h"
#include "gameplay.h"
#include "player.h"
#include "wordlist.h"
#include "bgeffects.h"
#include "plrstate.h"
#include "gpskin.h"
#include "stepdata.h"
#include "ogg.h"
#include "options.h"
#include "res.h"
#include "pak.h"
#include "vs.h"
#include "menu_score.h"
#include "songs.h"

#include "popup.h"
#include "winner.h"

// Constructor / Destructor
Gameplay::Gameplay(StepData * stepsi, PlayerState * p1, PlayerState * p2, bool demomode, bool typing, GameplayState::Mode mode) {
#if 0
	if (!demomode) {
		if (mode != GameplayState::ModePractice)
			VS::start(p1, p2);
		/* else
			loadingStart(); */
	}
#endif

	pakMount("gameplay");

	m_thdStartValid = false;

	m_state = new GameplayState();
	m_state->mode = mode;

	RefPtr<WordList> words;
	if (typing) {
		words = new WordList("gameplay/wordlist.txt");
		m_state->typing = true;
	}

	RefPtr<StepData> steps = stepsi;
	m_demo_mode = demomode;
	m_who_won = 0;

	// Load up our textures, fonts, and sounds for gameplay mode
	m_skin = new GameplaySkin(true, p1->round);
	m_skin->txr_framework->setFilter(Texture::FilterNone);

	char tmp[1024];
	sprintf(tmp, "songs/%s/bkg.jpg", steps->song_fn);
	// XXX
	// sprintf(tmp, "/pd%s/songs/%s/bkg.jpg", fs_getwd(), steps->song_fn);
	m_skin->txr_songbkg = new Texture();
	if (!m_skin->txr_songbkg->loadFromFile(tmp, false)) {
		m_skin->txr_songbkg = res.getTexture(Res::Gameplay, "generic_bkg", false);
	}

	m_bg = new BgEffects(m_skin->txr_songbkg);
	m_state->scene->subAdd(m_bg);

	m_player_cnt = p2 != NULL ? 2 : 1;
	if (p1->ai) {
		switch (randnum(3)) {
		case 0:
			p1->diff = StepData::Easy; break;
		case 1:
			p1->diff = StepData::Medium; break;
		case 2:
			p1->diff = StepData::Hard; break;
		default:	assert(false);
		}
		m_state->pp[0].ai = true;
	}
	if (m_player_cnt == 1)
		m_state->pp[1].ai = true;
	if (m_player_cnt == 2 && p2->ai) {
		switch (randnum(3)) {
		case 0:
			p2->diff = StepData::Easy; break;
		case 1:
			p2->diff = StepData::Medium; break;
		case 2:
			p2->diff = StepData::Hard; break;
		default:	assert(false);
		}
		m_state->pp[1].ai = true;
	}
	m_players = new RefPtr<Player>[m_player_cnt];

	m_psts = new RefPtr<PlayerState>[m_player_cnt];
	m_psts[0] = p1;
	if (m_player_cnt == 2)
		m_psts[1] = p2;

	m_kbstate = new KeyboardState();

	m_state->pp[0].steps = steps->deepCopy();
	m_state->pp[0].steps->parseData(p1->diff, words);
	if (m_player_cnt == 2 && m_state->allowAttacks())
		m_state->pp[0].steps->addBombs(typing, m_demo_mode /*, p1->fury*/);
	m_players[0] = new Player(this, m_skin, m_state, 0, 32.0f, 64.0f, 10.0f, m_state->pp[0].steps, p1, m_kbstate);

	if (m_player_cnt == 2) {
		m_state->pp[1].steps = steps;
		m_state->pp[1].steps->parseData(p2->diff, words);
		if (m_state->allowAttacks())
			m_state->pp[1].steps->addBombs(typing, m_demo_mode /*, p2->fury*/);
		m_players[1] = new Player(this, m_skin, m_state, 1, 640.0f - (256.0f + 32.0f), 64.0f, 10.0f,
			m_state->pp[1].steps, p2, m_kbstate);
	}
	steps = NULL;

	// Setup winner banner
	m_winner = new Winner();
	m_winner->setTranslate(Vector(0, 0, 50));
	m_win_frame = 0;

	m_sfx_win = res.getSound(Res::Gameplay, "winround");

	// Pull some values from one of the steps (doesn't matter, they 
	// should have the same basic values)
	m_state->gap = m_state->pp[0].steps->gap;
	m_state->timeunit = m_state->pp[0].steps->timeunit;

	m_intro_banner = NULL;

	//cacheLoad("gameplay/crowdnoise.ogg", "crowdnoise.ogg");
	pakUnmount("gameplay");

	// cacheLoad(steps->song_ogg, "gp.ogg");
	if (swapcd_enabled)
		sprintf(tmp, "%s/%s/song.ogg", swapcd_path, m_state->pp[0].steps->song_fn);
	else
		sprintf(tmp, "songs/%s/song.ogg", m_state->pp[0].steps->song_fn);
	// XXX
	//	sprintf(tmp, "/pd%s/songs/%s/song.ogg", fs_getwd(), m_state->pp[0].steps->song_fn);
	// cacheLoad(tmp, "gp.ogg");

#if 0
	// If we're in demo mode, we need to stop the throbber still
	if (m_demo_mode || mode == GameplayState::ModePractice)
	{ /*	loadingFinish(); */ }
	else
		VS::finish();
#endif

	// Dig through our songs and find the index of it, if we're not
	// in Swap CD mode.
	if (!swapcd_enabled) {
		const char * sfn = m_state->pp[0].steps->song_fn;
		for (int i=0; i<Songs::count; i++) {
			if (!strcmp(Songs::names[i], sfn)) {
				game_options.setSongPlayed(i);
				break;
			}
		}
	}

	m_result = Main::Ok;
}

Gameplay::~Gameplay() {
	thdCleanup();

	// Free player objects
	delete[] m_players;
	delete[] m_psts;

	// Uncache the song
	//cacheUnload("crowdnoise.ogg");
	//cacheUnload("gp.ogg");

	m_state->scene->subRemoveAll();
}

const char * Gameplay::m_restartOpt = "Restart Song";
const char * Gameplay::m_stageMenuOpt = "Back to Song Select";
const char * Gameplay::m_charMenuOpt = "Back to Character Select";
const char * Gameplay::m_swapMenuOpt = "Back to Swap CD Menu";
const char * Gameplay::m_mainMenuOpt = "Back to Main Menu";

void Gameplay::sysMenuPopup() {
	sysMenuCreate();
	oggVolume(1.0f);	// XXX -- weird noises sometimes here
	oggStop(false);
}

void Gameplay::sysMenuCreate() {
	// We need a GenericMenu to debounce keys
	m_sysmenugm = new GMShell(this);

	// Create the popup itself
	m_sysmenu = new Popup("System Menu");
	m_sysmenu->setCancellable(false);
	m_sysmenu->setTranslate(Vector(320,240,1000));
	m_sysmenu->addOption(m_restartOpt);
	m_sysmenu->addOption(m_stageMenuOpt);
	if (m_state->mode != GameplayState::ModePractice)
		m_sysmenu->addOption(m_charMenuOpt);
	if (swapcd_enabled)
		m_sysmenu->addOption(m_swapMenuOpt);
#ifndef KIOSK_MODE
	m_sysmenu->addOption(m_mainMenuOpt);
#endif

	m_state->scene->animAdd(
		new TintFader(Color(0.5f, 0.5f, 0.5f), Color(-0.5f/(hz/2),-0.5f/(hz/2),-0.5f/(hz/2))));
}

void Gameplay::sysMenuControlPerFrame() {
	m_sysmenugm->cpf();
}

void Gameplay::sysMenuInputEvent(const GenericMenu::Event & evt) {
	m_sysmenu->inputEvent(evt);

	int rv = m_sysmenu->getResult();
	if (rv == -1)
		return;

	exitNow(-1);

	const char * optname = m_sysmenu->getOptionString(rv);
	if (!strcmp(optname, m_restartOpt)) {
		m_result = Main::Gameplay;
	} else if (!strcmp(optname, m_stageMenuOpt)) {
		m_result = Main::MenuStage;
	} else if (!strcmp(optname, m_charMenuOpt)) {
		m_result = Main::MenuChar;
	} else if (!strcmp(optname, m_swapMenuOpt)) {
		m_result = Main::MenuSwapCD;
	} else {
		m_result = Main::MenuMain;
	}
}

void Gameplay::exitNow(int who) {
	m_state->state = GameplayState::StateDead;
	m_who_won = who;
	m_result = Main::Title;
}

void Gameplay::drawGameTime() {
	char str[32];
	uint32 gt, m, s, ms;

	gt = m_state->game_time;
	m = gt / (1000 * 60);
	s = (gt - (m*1000*60))/ 1000;
	ms = (gt - (m*1000*60)) % 1000;

	m_skin->fnt_plain->setColor(Color(1.0f, 1.0f, 1.0f));
	m_skin->fnt_plain->setSize(30);
	sprintf(str, "Time %02ld:%02ld:%03ld", m, s, ms);
	m_skin->fnt_plain->draw(Vector(320.0f, 16.0f + 30.0f, 100.0f), str);
	sprintf(str, "Beat %04d/%d", (int)(gt / (m_state->timeunit*4)),
		(int)(gt / m_state->timeunit) % 4);
	m_skin->fnt_plain->draw(Vector(320.0f, 16.0f + 60.0f, 100.0f), str);
}

// Draw the framework
void Gameplay::drawFramework() {
#define P(x) (x*1.0f/512.0f)

	uint32 WHITE = m_state->scene->getTint();
	m_skin->txr_framework->select();

	plx_dr_state_t dr;
	plx_dr_init(&dr);

	// Chunk 1 -- 0,0 to 32,480 -> 0,0
	plx_vert_ifd(&dr, PLX_VERT, 0, 480, 30, WHITE, P(0), P(480));
	plx_vert_ifd(&dr, PLX_VERT, 0, 0, 30, WHITE, P(0), P(0));
	plx_vert_ifd(&dr, PLX_VERT, 32, 480, 30, WHITE, P(32), P(480));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 32, 0, 30, WHITE, P(32), P(0));

	// Chunk 2 -- 32,0 to 64,74 -> 32,0
	plx_vert_ifd(&dr, PLX_VERT, 32, 74, 30, WHITE, P(32), P(74));
	plx_vert_ifd(&dr, PLX_VERT, 32, 0, 30, WHITE, P(32), P(0));
	plx_vert_ifd(&dr, PLX_VERT, 64, 74, 30, WHITE, P(64), P(74));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 64, 0, 30, WHITE, P(64), P(0));

	// Chunk 3 -- 64,0 to 512,64 -> 64,0
	plx_vert_ifd(&dr, PLX_VERT, 64, 64, 30, WHITE, P(64), P(64));
	plx_vert_ifd(&dr, PLX_VERT, 64, 0, 30, WHITE, P(64), P(0));
	plx_vert_ifd(&dr, PLX_VERT, 512, 64, 30, WHITE, P(512), P(64));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 512, 0, 30, WHITE, P(512), P(0));

	// Chunk 4 -- 288,64 to 352,225 -> 288,64
	plx_vert_ifd(&dr, PLX_VERT, 288, 225, 30, WHITE, P(288), P(225));
	plx_vert_ifd(&dr, PLX_VERT, 288, 64, 30, WHITE, P(288), P(64));
	plx_vert_ifd(&dr, PLX_VERT, 352, 225, 30, WHITE, P(352), P(225));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 352, 64, 30, WHITE, P(352), P(64));

	// Chunk 5 -- 32,480 to 512,512 -> 608,0 (rotated 90' ccw)
	plx_vert_ifd(&dr, PLX_VERT, 608, 480, 30, WHITE, P(32), P(480));
	plx_vert_ifd(&dr, PLX_VERT, 608, 0, 30, WHITE, P(512), P(480));
	plx_vert_ifd(&dr, PLX_VERT, 640, 480, 30, WHITE, P(32), P(512));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 640, 0, 30, WHITE, P(512), P(512));

	// Chunk 6 -- 438,449 to 512,480 -> 577,0 (rotated 90' ccw)
	plx_vert_ifd(&dr, PLX_VERT, 577, 74, 30, WHITE, P(438), P(449));
	plx_vert_ifd(&dr, PLX_VERT, 577, 0, 30, WHITE, P(512), P(449));
	plx_vert_ifd(&dr, PLX_VERT, 608, 74, 30, WHITE, P(438), P(480));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 608, 0, 30, WHITE, P(512), P(480));
	
	// Chunk 7 -- 448,384 to 512,449 -> 512,0 (rotated 90' ccw)
	plx_vert_ifd(&dr, PLX_VERT, 512, 64, 30, WHITE, P(448), P(384));
	plx_vert_ifd(&dr, PLX_VERT, 512, 0, 30, WHITE, P(512), P(384));
	plx_vert_ifd(&dr, PLX_VERT, 577, 64, 30, WHITE, P(448), P(449));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 577, 0, 30, WHITE, P(512), P(449));


	/* // Chunk 1 -- 16,25 to 64,74 -> 16,25
	plx_vert_ifd(&dr, PLX_VERT, 16, 74, 30, WHITE, P(16), P(74));
	plx_vert_ifd(&dr, PLX_VERT, 16, 25, 30, WHITE, P(16), P(25));
	plx_vert_ifd(&dr, PLX_VERT, 64, 74, 30, WHITE, P(64), P(74));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 64, 25, 30, WHITE, P(64), P(25));

	// Chunk 2 -- 64,35 to 320,64 -> 64,35
	plx_vert_ifd(&dr, PLX_VERT, 64, 64, 30, WHITE, P(64), P(64));
	plx_vert_ifd(&dr, PLX_VERT, 64, 35, 30, WHITE, P(64), P(35));
	plx_vert_ifd(&dr, PLX_VERT, 320, 64, 30, WHITE, P(320), P(64));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 320, 35, 30, WHITE, P(320), P(35));
	
	// Chunk 3 -- 288,64 to 320,225 -> 288,64
	plx_vert_ifd(&dr, PLX_VERT, 288, 225, 30, WHITE, P(288), P(225));
	plx_vert_ifd(&dr, PLX_VERT, 288, 64, 30, WHITE, P(288), P(64));
	plx_vert_ifd(&dr, PLX_VERT, 320, 225, 30, WHITE, P(320), P(225));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 320, 64, 30, WHITE, P(320), P(64));
	
	// Chunk 4 -- 272,231 to 320,280 -> 576,25
	plx_vert_ifd(&dr, PLX_VERT, 576, 74, 30, WHITE, P(272), P(280));
	plx_vert_ifd(&dr, PLX_VERT, 576, 25, 30, WHITE, P(272), P(231));
	plx_vert_ifd(&dr, PLX_VERT, 624, 74, 30, WHITE, P(320), P(280));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 624, 25, 30, WHITE, P(320), P(231));
	
	// Chunk 5 -- 15,241 to 272,270 -> 320,35
	plx_vert_ifd(&dr, PLX_VERT, 320, 64, 30, WHITE, P(15), P(270));
	plx_vert_ifd(&dr, PLX_VERT, 320, 35, 30, WHITE, P(15), P(241));
	plx_vert_ifd(&dr, PLX_VERT, 577, 64, 30, WHITE, P(272), P(270));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 577, 35, 30, WHITE, P(272), P(241));
	
	// Chunk 6 -- 15,270 to 47,431 -> 320,64
	plx_vert_ifd(&dr, PLX_VERT, 320, 225, 30, WHITE, P(15), P(431));
	plx_vert_ifd(&dr, PLX_VERT, 320, 64, 30, WHITE, P(15), P(270));
	plx_vert_ifd(&dr, PLX_VERT, 352, 225, 30, WHITE, P(47), P(431));
	plx_vert_ifd(&dr, PLX_VERT_EOS, 352, 64, 30, WHITE, P(47), P(270)); */
	
#undef P
#undef WHITE
}

// Overall visuals per-frame
void Gameplay::visualPerFrame() {
#ifdef RUN_FROM_PC
	char str[64];
// XXX
//	pvr_stats_t stats;
#endif
	
	Frame::begin();
#ifdef RUN_FROM_PC
// XXX
//	pvr_get_stats(&stats);
#endif

	if (m_state->drawBackground())	
		m_bg->draw(Drawable::Opaque);
	else
		glClearColor(0.02f, 0.07f, 0.21f, 1.0f);

	drawFramework();

	if (m_sysmenu)
		m_sysmenu->drawAll(Drawable::Opaque);

	Frame::transEnable();

	if (m_sysmenu)
		m_sysmenu->drawAll(Drawable::Trans);

	if (m_state->drawBackground())	
		m_bg->drawAll(Drawable::Trans);

	if (m_intro_banner) {
		m_intro_banner->drawAll(Drawable::Trans);
		m_intro_banner->nextFrame(0);
	}

	for (int i=0; i<m_player_cnt; i++) {
		m_players[i]->visualPerFrame(Drawable::Trans);
	}

	if (m_state->state == GameplayState::StateWinner) {
		m_winner->drawAll(Drawable::Trans);
		m_winner->nextFrame(0);
	}

#ifdef RUN_FROM_PC
	sprintf(str, "fps %.2f  reg %d  rnd %d",
		(double)stats.frame_rate, (int)stats.reg_last_time,
		(int)stats.rnd_last_time);
	m_skin->fnt_plain->setAlpha(1.0f);
	m_skin->fnt_plain->setSize(16.0f);
	m_skin->fnt_plain->drawAll(20.0f, 40.0f, 400.0f, str);
	// drawGameTime();
#endif

	if (m_demo_mode) {
		m_skin->fnt->setColor(Color(1,1,1));
		m_skin->fnt->setAlpha(fsin(m_state->frame * 2*M_PI/(hz*2)) / 2 + 0.5f);
		m_skin->fnt->setSize(36.0f);
		m_skin->fnt->smearDrawCentered(Vector(320.0f, 300.0f, 2000.0f), "D E M O  P L A Y");
	}

	Frame::finish();

	// m_bg->nextFrame();
	m_state->scene->nextFrame(0);
	m_state->frame++;

	if (m_sysmenu)
		m_sysmenu->nextFrame(0);
}

// Controller polling
void Gameplay::controlPerFrame() {
	m_kbstate->update();

	for (int i=0; i<m_player_cnt; i++) {
		m_players[i]->controlPerFrame();
	}
}

// Do an attack on all other player(s)
void Gameplay::doAttack(Player * from, Attack type, bool sound) {
	for (int i=0; i<m_player_cnt; i++) {
		if (m_players[i] != from) {
			m_players[i]->attackReceive(type, sound);
		}
	}
}

void * Gameplay::thdStartStatic(void *ptr) {
	assert( ptr != NULL );
	((Gameplay *)ptr)->thdMethod();
	return NULL;
}

void Gameplay::thdStart() {
// DirectX is thread stoopid
#if TIKI_PLAT == TIKI_WIN32
	thdMethod();
#else
	thdCleanup();
	m_thdStart = new Thread::Thread(thdStartStatic, this);
	m_thdStartValid = true;
#endif
}

void Gameplay::thdCleanup() {
#if TIKI_PLAT != TIKI_WIN32
	if (m_thdStartValid) {
		m_thdStart->join();
		m_thdStart = NULL;
		m_thdStartValid = false;
	}
#endif
}

void Gameplay::thdMethod() {
	oggStop(false);

	// Queue up the music, but don't start yet
	/* if (m_thd_mode == ThdModeBegin)
		oggStart(CACHE_PREFIX "gp.ogg", false, false, true);
	else
		oggStart(CACHE_PREFIX "crowdnoise.ogg", false, false, true); */
	if (m_thd_mode == ThdModeBegin) {
		char tmp[256];
		if (swapcd_enabled)
			sprintf(tmp, "%s/%s/song.ogg", swapcd_path, m_state->pp[0].steps->song_fn);
		else
			sprintf(tmp, "songs/%s/song.ogg", m_state->pp[0].steps->song_fn);
		oggStart(tmp, false, false, true);
	} else
		oggStart("gameplay/crowdnoise.ogg", false, false, true);
	OggVorbis::queueWait();

	m_is_queued = true;
}
        
// Called to actually do the gameplay
int Gameplay::doGameplay() {
	// Do the round intro first
	m_state->gap_used = false;
	// oggStart(CACHE_PREFIX "crowdnoise.ogg" , false, false, true);
	oggStart("gameplay/crowdnoise.ogg", false, false, true);
	// ogg_start("sfx/crowdnoise.ogg", false, true, true);

	/* m_intro_banner = new WaveTexture(m_skin->txr_rnd, Drawable::Trans);
	m_intro_banner->setTranslate(Vector(320.0f, 240.0f, 100.0f));
	m_intro_banner->setMag(16.0f);
	m_intro_banner->setAlpha(0.0f);
	m_intro_banner->animAdd(new WaveTextureAnim(
		0.0f, -16.0f/45.0f, 1.0f, 1.0f/45.0f)); */
	if (m_state->mode == GameplayState::ModePractice)
		m_intro_banner = new Banner(Drawable::Trans, m_skin->txr_ready);
	else
		m_intro_banner = new Banner(Drawable::Trans, m_skin->txr_rnd);
	m_intro_banner->setTranslate(Vector(320.0f, 240.0f, 100.0f));
	m_intro_banner->setTint(Color(0,1,1,1));
	m_intro_banner->animAdd(new AlphaFader(1.0f, 1.0f/(3.0f*hz/4)));

	OggVorbis::queueWait();
	OggVorbis::queueGo();
	for (int i=0; i<(300*hz/60); i++) {
		visualPerFrame();
		if (i == (30*hz/60) && game_options.announcer) {
			if (m_state->mode == GameplayState::ModePractice)
				m_skin->sfx_practicemode->play();
			else
				m_skin->sfx_announce_rnd[m_psts[0]->round]->play();
		}
		if (i == (hz*2)) {
			m_intro_banner->setTexture(m_skin->txr_ready);
		}
		if (i == (130*hz/60) && game_options.announcer) {
			if (randnum(100) < 50)
				m_skin->sfx_areready->play();
			else
				m_skin->sfx_getready->play();
		}
		if (i == (250*hz/60)) {
			//m_intro_banner->animAdd(new WaveTextureAnim(
			//	16.0f, 16.0f/30.0f, 0.0f, -1.0f/30.0f));
			m_intro_banner->animAdd(new AlphaFader(
				0.0f, -1.0f/(hz/2.0f)));

			if (game_options.announcer) {
				int tr = randnum(100);
				if (m_state->typing) {
					if (tr < 50)
						m_skin->sfx_herewego->play();
					else
						m_skin->sfx_letstype->play();
				} else {
					if (tr < 33)
						m_skin->sfx_herewego->play();
					else if (tr < 66)
						m_skin->sfx_letsdance->play();
					else
						m_skin->sfx_letsstep->play();
				}
			}
		}
		if (i >= (250*hz/60)) {
			oggVolume(((300*hz/60) - i) / (50.0f*hz/60));
		}

// XXX
#if 0
		if (m_demo_mode) {
			MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
				if (st->buttons & (CONT_START|CONT_A|CONT_B))
					goto skipOut;
			MAPLE_FOREACH_END()
			MAPLE_FOREACH_BEGIN(MAPLE_FUNC_KEYBOARD, kbd_state_t, st)
				if (st->matrix[KBD_KEY_ENTER] || st->matrix[KBD_KEY_ESCAPE])
					goto skipOut;
			MAPLE_FOREACH_END()
		}
#endif
	}

	m_is_queued = false;
	m_intro_banner = NULL;
	
	// This will take care of stopping the previous OGG and
	// queueing up the real song OGG without interrupting the
	// visuals.
	m_thd_mode = ThdModeBegin;
	thdStart();

	while (!m_is_queued)
		visualPerFrame();

	// Grab the start time and start the song
	m_state->recordStart();
	OggVorbis::queueGo();

	while (m_state->state != GameplayState::StateDead) {
		// Update the clocks
		if (m_state->state == GameplayState::StateNormal && !m_sysmenu) {
			m_state->syncTime();
			controlPerFrame();
		}
		if (m_sysmenu)
			sysMenuControlPerFrame();

		// Do beat claps if necessary
		if (m_state->beatClaps() && m_state->gap_used) {
			if (m_state->old_game_beat != m_state->game_beat
				&& !(m_state->game_beat % 4))
			{
				if (!(m_state->game_beat % 16))
					m_skin->sfx_clap[0]->play();
				else
					m_skin->sfx_clap[1]->play();
			}
		}
		
		visualPerFrame();

		// If either player's life has hit zero and we're in out-of-health
		// mode for the end of the round, then stop the song so a winner
		// will be declared.
		bool manual_die = false;
		if (game_options.eor_style == 0 && !m_demo_mode && m_state->mode != GameplayState::ModePractice) {
			if (m_state->pp[0].life <= 0 || m_state->pp[1].life <= 0) {
				manual_die = true;
			}
		}

		// Check for OggVorbis death
		if (m_state->state == GameplayState::StateNormal && (!OggVorbis::isPlaying() || manual_die) && !m_sysmenu) {
			if (m_player_cnt == 1 || !m_state->doWinner()) {
				m_state->state = GameplayState::StateDead;
			} else {
				m_state->state = GameplayState::StateWinner;

				m_sfx_win->play();
				if (m_state->pp[0].life > m_state->pp[1].life) {
					m_winner->setSide(0);
					m_who_won = 1;
				} else if (m_state->pp[0].life < m_state->pp[1].life) {
					m_winner->setSide(1);
					m_who_won = 2;
				} else {
					// Check furiosity meters
					if (m_state->pp[0].furiosity > m_state->pp[1].furiosity) {
						m_winner->setSide(0);
						m_who_won = 1;
					} else if (m_state->pp[0].furiosity < m_state->pp[1].furiosity) {
						m_winner->setSide(1);
						m_who_won = 2;
					} else {
						m_winner->setSide(-1);
						m_who_won = 0;
					}
				}
				if (m_who_won == 1)
					m_psts[0]->wins++;
				else if (m_who_won == 2)
					m_psts[1]->wins++;
				for (int i=0; i<m_player_cnt; i++)
					m_players[i]->updateWins();
					
				m_thd_mode = ThdModeWin;
				m_is_queued = false;
				thdStart();
				while (!m_is_queued)
					visualPerFrame();
				OggVorbis::queueGo();

				m_win_frame = 0;
			}
		}

		if (m_state->state == GameplayState::StateWinner) {
			m_win_frame++;
			if (m_win_frame == hz && game_options.announcer) {
				switch (m_who_won) {
				case 1:
					m_skin->sfx_player[0]->play();
					break;
				case 2:
					m_skin->sfx_player[1]->play();
					break;
				}
			}
			if (m_win_frame == (123*hz/60) && game_options.announcer) {
				if (m_who_won == 1 || m_who_won == 2)
					m_skin->sfx_wins->play();
			}
			// if (m_win_frame > 600 || !sndoggvorbis_isplaying())
			if (m_win_frame > (hz*10))
				m_state->state = GameplayState::StateDead;
		}
	}


skipOut:
	// Stop the music
	oggStop(false);

	// Do a couple of blank frames
	for (int i=0; i<3; i++) {
		Frame::begin();
		Frame::finish();
	}

	return 0;
}

int Gameplay::whoWon() const { return m_who_won; }

Main::Result Gameplay::getResult() const { return m_result; }

GameplayState * Gameplay::getState() const {
	return m_state;
}

GameplaySkin * Gameplay::getSkin() const {
	return m_skin;
}

Main::Result doGameplay(StepData * sd, PlayerState * p1, PlayerState * p2, bool demomode, bool typing, GameplayState::Mode mode, int & winner) {
	Gameplay gp(sd, p1, p2, demomode, typing, mode);
	gp.doGameplay();

	winner = gp.whoWon();

	Main::Result r = gp.getResult();
	if (r == Main::Ok && !demomode) {
		// Do the score menu
		RefPtr<MenuScore> ms = new MenuScore(gp.getState(), gp.getSkin(), winner);
		ms->doMenu();
		game_options.save();
	}
	return r;
}

