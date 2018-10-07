/*
   Feet of Fury

   menu_score/menu_score.cc

   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "menu_score.h"
#include "res.h"
#include "pak.h"
#include "gpstate.h"
#include "gpskin.h"
#include "stepdata.h"
#include "step.h"
#include "options.h"

#include "scrollybg.h"
#include "screenblinds.h"
#include "finishall.h"
#include "topbar.h"

MenuScore::MenuScore(GameplayState * gps, GameplaySkin * sk, int winner) {
	m_gps = gps;
	m_skin = sk;
	m_winner = winner;

	// A nice background
	m_sf = new ScrollyBG(res.getTexture(Res::Preload, "scrolly_logo2"), true);
	m_sf->setTranslate(Vector(0,0,0.001));
	m_scene->subAdd(m_sf);

	// Topbar
	m_topbar = new Topbar(20, 2, Topbar::Horizontal, Topbar::Barcode);
	m_topbar->setTranslate(Vector(0,70,5));
	m_scene->subAdd(m_topbar);

	// Banner
	m_banner = new Banner(Drawable::Trans, m_skin->txr_score_banner);
	m_banner->setTranslate(Vector(320,70,10));
	m_scene->subAdd(m_banner);

	// Players
	m_plr[0] = new Plr(this, 0);
	m_plr[0]->setTranslate(Vector(160, 230, 15));
	m_scene->subAdd(m_plr[0]);

	m_plr[1] = new Plr(this, 1);
	if (m_gps->mode != GameplayState::ModePractice || !m_gps->pp[1].ai) {
		m_plr[1]->setTranslate(Vector(320+160 - 32, 230, 15));
		m_scene->subAdd(m_plr[1]);
	} else {
		m_plr[1]->m_changed = false;
	}

	// In-transition
	ScreenBlinds * sb = new ScreenBlinds(false);
	sb->setTint(Color(1,0,0,0));
	m_scene->subAdd(sb);

	// It will have calculated the unlock points per player. Take
	// the totals for both sides, add them up, and append them to the
	// player's total.
	int up = m_gps->pp[0].unlock_pts;
	up += m_gps->pp[1].unlock_pts;
	if (up > 0) {
		game_options.unlock_pts += up;
		if (game_options.unlock_pts > 50000000)
			game_options.unlock_pts = 50000000;
		game_options.dirty = true;
	}
	m_plr[0]->initOne(7, game_options.unlock_pts, 8);
	m_plr[1]->initOne(7, 0, 8);

	// Time out after a bit
	setTimeout(60);

	m_frame = 0;
}

MenuScore::~MenuScore() {
}

void MenuScore::startExit() {
	FinishAll * fa = new FinishAll();
	fa->addTarget(m_sf);
	fa->addTarget(m_banner);
	fa->addTarget(m_topbar);
	fa->addTarget(m_plr[0]);
	fa->addTarget(m_plr[1]);

	ScreenBlinds * sb = new ScreenBlinds(true);
	sb->setTint(Color(1,0,0,0));
	sb->triggerAdd(fa);
	m_scene->subAdd(sb);

	GenericMenu::startExit();
}

void MenuScore::inputEvent(const GenericMenu::Event & evt) {
	if (evt.type == Event::EvtKeypress || evt.type == Event::EvtTimeout)
		startExit();
}

void MenuScore::visualPerFrame() {
	GenericMenu::visualPerFrame();

	if (m_plr[0]->m_changed || m_plr[1]->m_changed) {
		if (!(m_frame % 3)) {
			m_skin->sfx_score_click->play();
		}
	}
	m_frame++;
}

///////////////////////////////////////////////////////////////////////

MenuScore::Plr::Plr(MenuScore * parent, int idx) {
	m_idx = idx;
	m_parent = parent;

	memset(m_bufs, 0, 8*64);
	memset(m_bufs_dst, 0, 8*64);

	bool iswinner = false;
	if (m_parent->m_gps->mode != GameplayState::ModePractice &&
		m_parent->m_winner == (m_idx+1))
	{
		iswinner = true;
	}
                        

	// Setup our step type banners
	float y = -100.0f;
	static const float scoots[5] = {
		0.0f,
		-11.0f,
		-28.0f,
		-16.0f,
		-22.0f
	};
	for (int i=0; i<5; i++) {
		float v1 = i/5.0f;
		m_banner_types[i] = new Banner(Drawable::Trans, m_parent->m_skin->txr_judge);
		m_banner_types[i]->setUV(0.0f, v1+1.0f/5.0f, 0.0f, v1, 1.0f, v1+1.0f/5.0f, 1.0f, v1);
		m_banner_types[i]->setSize(128, 128/5.0f);	// orig = 256
		m_banner_types[i]->setTranslate(Vector(-64 + scoots[i], y, 1));
		y += 128/5.0f + 8.0f;
		subAdd(m_banner_types[i]);
	}

	// And then a label for a couple of misc other things
	m_lbl_types[0] = new Label(m_parent->m_skin->fnt, "Score", 20, false, false);
	m_lbl_types[0]->setTranslate(Vector(-64 - 48, y+24, 1));
	m_lbl_types[0]->setTint(Color(0.3f,0.85f,0.3f));
	subAdd(m_lbl_types[0]); y += 128/5.0f + 8.0f;

	RefPtr<Label> winbonus = new Label(m_parent->m_skin->fnt, "Win Bonus", 20, false, false);
	winbonus->setTranslate(Vector(-64 - 48, y+24, 1));
	winbonus->setTint(Color(0.3f,0.85f,0.3f));
	subAdd(winbonus); y += 128/5.0f + 8.0f;

	m_lbl_types[1] = new Label(m_parent->m_skin->fnt, "Unlock Pts", 20, false, false);
	m_lbl_types[1]->setTranslate(Vector(-64 - 48, y+24, 1));
	m_lbl_types[1]->setTint(Color(0.3f,0.85f,0.3f));
	subAdd(m_lbl_types[1]); y += 128/5.0f + 8.0f;

	if (idx == 0) {
		m_lbl_types[2] = new Label(m_parent->m_skin->fnt, "Total U.P.", 20, false, false);
		m_lbl_types[2]->setTranslate(Vector(-64 - 48, y+24, 1));
		m_lbl_types[2]->setTint(Color(0.3f,0.85f,0.3f));
		subAdd(m_lbl_types[2]); y += 128/5.0f + 8.0f;
	}

	// Now the data labels for each one
	y = -100.0f;
	for (int i=0; i<9; i++) {
		if (idx != 0 && i == 8)
			break;

		float ys = y+24;
		if (i < 5)
			ys -= 24 - 7;   // hehe

		int is = i;
		if (i == 6) {
			RefPtr<Label> win2x = new Label(m_parent->m_skin->fnt, iswinner ? "2.5x" : "--", 20, false, false);
			win2x->setTranslate(Vector(30, ys, 1));
			win2x->setTint(Color(0.85f,0.85f,0.85f));
			subAdd(win2x); y += 128/5.0f + 8.0f;
			continue;
		} else if (i > 6) {
			is--;
		}
		
		m_lbl_scores[is] = new Label(m_parent->m_skin->fnt_plain, m_bufs[is], 20, false, false);
		m_lbl_scores[is]->setTranslate(Vector(30, ys, 1));
		m_lbl_scores[is]->setTint(Color(0.85f,0.85f,0.85f));
		subAdd(m_lbl_scores[is]);
		y += 128/5.0f + 8.0f;
	}

	// Where do the steps end? i.e., what's the last step really used?
	StepData * sd = m_parent->m_gps->pp[m_idx].steps;
	/* for (int i=sd->step_cnt-1; i>=0; i--) {
		if (sd->steps[i].pattern != '0') {
			uc = i;
			break;
		}
	} */

	// Count steps of each type
	int uc = 0;
	int types[5] = { 0 };
	for (int i=0; i<sd->step_cnt; i++) {
		Step * st = sd->steps + i;
		if (st->pattern == '0')
			continue;
		switch (st->hit) {
		case Step::Perfect:
			types[0]++; break;
		case Step::Great:
			types[1]++; break;
		case Step::Good:
			types[2]++; break;
		case Step::Boo:
			types[3]++; break;
		case Step::Miss:
			types[4]++; break;
		case Step::NotYet:
		case Step::Added:
			break;		// Just count it in the total
		default:
			printf("unknown step hit type '%d'\n", st->hit);
			break;
		}
		uc++;
	}
	if (!uc) uc++;

	// Calculate % complete
	int totals = types[0] + types[1] + types[2] + types[3] + types[4];
	// printf("totals = %d, cnt = %d, compl = %d\n", totals, uc, compl);

	// Calculate the score
	int score = types[0] * 3000 +
		types[1] * 1000 +
		types[2] * 250 +
		types[3] * 50;

	// Adjust it by the % complete, if it's not 100%
	/* if (uc != totals) {
		if (!totals) totals++;
		score = score * uc / totals;
	} */

	// If this player was the winner, give them a third more points.
	if (m_parent->m_gps->mode != GameplayState::ModePractice &&
		m_parent->m_winner == (m_idx+1))
	{
		score = (int)(score * 2.5f);
	}

	// Calculate the unlock points they get. Every time they play a song,
	// the user gets at least 10 points. After that, they get sqrt(pts)/2.
	int up = 10;
	if (score > 0) {
		float sq = fsqrt(score) / 2;
		up += (int)sq;
	}

	// If this player was an AI, then take those points back! ;)
	if (m_parent->m_gps->pp[m_idx].ai) {
		up = 0;
	}

	// If they were in practice mode, only give a fraction of the standard
	// unlock points. Battles are harder and deserve more points.
	if (m_parent->m_gps->mode == GameplayState::ModePractice) {
		up = (int)(up * 0.5f);
	}

	// Fill in the unlock points for this player
	m_parent->m_gps->pp[m_idx].unlock_pts = up;

	// Init the buffers
	initOne(0, types[0], 5);
	initOne(1, types[1], 5);
	initOne(2, types[2], 5);
	initOne(3, types[3], 5);
	initOne(4, types[4], 5);
	initOne(5, score, 8);
	initOne(6, up, 5);
	// initOne(7, game_options.unlock_pts, 7);

	m_changed = true;
}

MenuScore::Plr::~Plr() {
}

void MenuScore::Plr::initOne(int i, int value, int digits) {
	char fmt[16];
	sprintf(fmt, "%%0%dd", digits);
	sprintf(m_bufs_dst[i], fmt, value);


	// strcpy(m_bufs[i], m_bufs_dst[i]);
	int len = strlen(m_bufs_dst[i]);
	for (int j=0; j<len; j++) {
		m_bufs[i][j] = '1' + j;
		/* m_bufs[i][j] += j+1;
		while (m_bufs[i][j] > '9')
			m_bufs[i][j] -= 10; */
	}
	m_bufs[i][len] = 0;
	
	// printf("Player %d: initializing %d to '%s' ('%s', %d)\n", m_idx, i, m_bufs[i], m_bufs_dst[i], value);
}

void MenuScore::Plr::nextFrame(uint64 tm) {
	Drawable::nextFrame(tm);

	if (!m_changed)
		return;

	m_changed = false;
	for (int i=0; i<8; i++) {
		int len = strlen(m_bufs[i]);
		char * buf = m_bufs[i], *bd = m_bufs_dst[i];
		for (int j=0; j<len; j++) {
			if (buf[j] != bd[j]) {
				for (int k=j; k<len; k++) {
					buf[k]++;
					if (buf[k] > '9')
						buf[k] = '0';
				}
				m_changed = true;
				break;
			}
		}
		if (m_lbl_scores[i])
			m_lbl_scores[i]->setText(m_bufs[i]);
	}

	if (!m_changed) {
		Debug::printf("Player %d final values:\n", m_idx);
		for (int i=0; i<8; i++) {
			Debug::printf("%d: '%s' (vs '%s')\n", i, m_bufs[i], m_bufs_dst[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////////

void testMenuScore() {
	RefPtr<StepData> sd = loadSteps("djg_gotnoclue");

	RefPtr<GameplayState> gps = new GameplayState();
	gps->mode = GameplayState::ModePractice;
	gps->pp[0].ai = false;
	gps->pp[0].steps = sd->deepCopy();
	gps->pp[0].steps->parseData(StepData::Hard);
	gps->pp[1].ai = false;
	gps->pp[1].ai_accuracy = 0.8f;
	gps->pp[1].steps = sd;
	gps->pp[1].steps->parseData(StepData::Medium);

	RefPtr<GameplaySkin> sk = new GameplaySkin(true, 0);
	
	for (int plr=0; plr<2; plr++) {
		sd = gps->pp[plr].steps;
		for (int i=0; i<sd->step_cnt; i++) {
			if (sd->steps[i].pattern == '0')
				continue;

			int rnd = randnum(100);
			if (rnd < 20)
				sd->steps[i].hit = Step::Perfect;
			else if (rnd < 40)
				sd->steps[i].hit = Step::Great;
			else if (rnd < 60)
				sd->steps[i].hit = Step::Good;
			else if (rnd < 80)
				sd->steps[i].hit = Step::Boo;
			else
				sd->steps[i].hit = Step::Miss;
		}
	}

	RefPtr<MenuScore> mps = new MenuScore(gps, sk, 0);

	mps->doMenu();
}
