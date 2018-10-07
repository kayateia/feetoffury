/*
   Feet of Fury

   gameplay/bgeffects/bgeffects.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "bgeffects.h"
#include "res.h"

#define BASE_Z 0.00011f

#include "base_blank.h"
#include "base_static.h"
#include "base_ripple.h"
#include "base_plasma.h"
#include "base_tube.h"
#include "base_starfield.h"
#include "top_bubblefield.h"
#include "top_hoops.h"
#include "top_cubetunnel.h"
#include "top_scope.h"

BgEffects::BgEffects(Texture * txr) {
	m_txr = txr;

	m_base_cur = new BaseStatic(txr);
	subAdd(m_base_cur);
	m_base_cnt = 5*hz;
	m_base_idx = 0;

	// m_base_cur = new BasePlasma();
	
	//m_base_cur = new BaseStatic(txr);
	//m_base_cnt = 5*hz;
	//m_base_idx = 1;
	m_top_cur = NULL;
	m_top_cnt = 7*hz;
	m_top_idx = -1;
	m_txr_cube = res.getTexture(Res::Gameplay, "scrolly_logo", false);
	m_txr_bubble = res.getTexture(Res::Preload, "bubble", true);

	m_frame = 0;
}

BgEffects::~BgEffects() {
}

void BgEffects::draw(ObjType list) {
	if (m_base_cur != NULL)
		m_base_cur->drawAll(list);
	if (m_top_cur != NULL)
		m_top_cur->drawAll(list);
}

void BgEffects::nextFrame(uint64 tm) {
	m_frame++;
	if (m_base_cur)
		m_base_cur->nextFrame(tm);
	if (m_top_cur)
		m_top_cur->nextFrame(tm);
	if (m_base_cnt > 0) {
		m_base_cnt--;
		if (m_base_cnt == 0) {
			m_base_cnt = 4*hz;
			if (m_base_cur)
				subRemove(m_base_cur);
			m_base_cur = getBaseEffect();
			if (m_base_cur)
				subAdd(m_base_cur);
		}
	}
	if (m_top_cnt > 0) {
		m_top_cnt--;
		if (m_top_cnt == 0) {
			m_top_cnt = 6*hz;
			if (m_top_cur)
				subRemove(m_top_cur);
			m_top_cur = getTopEffect();
			if (m_top_cur)
				subAdd(m_top_cur);
		}
	}
}

Drawable * BgEffects::getBaseEffect() {
	int idx;
	do {
		// Choose an effect
		idx = randnum(5);

		// Can't have cubes or load hoops and wavetxr at once (frame drops)
		//if (idx == 2 && (m_top_idx == 0 || m_top_idx == 1))
		//	idx = m_base_idx;
	} while (idx == m_base_idx);
	m_base_idx = idx;

	switch (idx) {
	case 0:
		//printf("get_base_effect: switching to base_blank\n");
		// return new BaseBlank();
		//return new BaseStatic(m_txr);
		m_base_idx = 1;
	case 1:
		//printf("get_base_effect: switching to base_static\n");
		return new BaseTube(m_txr_cube);
	case 2:
		//printf("get_base_effect: switching to base_ripple\n");
		return new BaseRipple(m_txr);
	case 3:
		//printf("get_base_effect: switching to base_plasma\n");
		return new BasePlasma();
	case 4:
		return new BaseStarField();
	default:
		assert(false);
		return new BasePlasma();
	}
}

TopDrawable * BgEffects::getTopEffect() {
	int idx;
	do {
		// Choose an effect
		idx = randnum(5);

		// Can't have cubes and wavetxr at once (frame drops)
		//if ((idx == 0 || idx == 1) && (m_base_idx == 2))
		//	idx = m_top_idx;
	} while (idx == m_top_idx);
	m_top_idx = idx;

	switch (idx) {
	case 0:
		//printf("get_top_effect: switching to top_cube_tunnel(false)\n");
		return new TopCubeTunnel(m_txr_cube, false);
	case 1:
		//printf("get_top_effect: switching to top_cube_tunnel(true)\n");
		return new TopCubeTunnel(m_txr_cube, true);
	case 2:
		//printf("get_top_effect: switching to top_bubble_field\n");
		return new TopBubbleField(m_txr_bubble);
	case 3:
		//printf("get_top_effect: switching to top_hoops\n");
		return new TopHoops();
	case 4:
		m_top_cnt = 10*hz;
		return new TopScope();
	default:
		assert(false);
		return new TopCubeTunnel(m_txr_cube, false);
	}
}
        
