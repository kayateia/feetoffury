/* 
   Feet of Fury
   
   drawables/combobomb.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "combobomb.h"

ComboSubBomb::ComboSubBomb() {
	m_color = Color(0, 1, 1, 1);
	m_vx = m_vy = 0.0f;
	m_tx = m_ty = m_tz = 0.0f;
}

ComboSubBomb::~ComboSubBomb() {
}

// Draw a single twinkley
void ComboSubBomb::draw() {
	float s = 64.0f * m_sc;
	plx_dr_state_t dr;

	plx_dr_init(&dr);
	plx_spr_fnd(&dr, s, s, m_tx, m_ty, m_tz, m_color.a, m_color.r, m_color.g, m_color.b);
}

void ComboSubBomb::nextFrame(uint64 tm) {
	if (m_vx != 0.0f) {
		m_color.a -= 0.01f;
		m_sc += 0.01f;
	} else {
		m_color.a -= 0.05f;
		m_sc -= 0.005f;
	}
	if (m_color.a < 0.0f)
		m_color.a = 0.0f;
	if (m_sc < 0.0f)
		m_sc = 0.0f;

	m_tx += m_vx;
	m_ty += m_vy;
}

void ComboSubBomb::recycle(float x, float y, float z) {
	m_color.a = 1.0f - (rand() % 1000) * 0.001f;
	m_tx = x + (rand() % 48) - 24;
	m_ty = y + (rand() % 48) - 24;
	m_tz = z;
	m_sc = 1.0f;
}

void ComboSubBomb::explode() {
	m_vx = (rand() % 16) - 8;
	m_vy = (rand() % 16) - 8;
}

void ComboSubBomb::setVel(float x, float y) {
	m_vx = x;
	m_vy = y;
}


ComboBomb::ComboBomb(int index, Texture * texture) {
	m_index = index;
	m_texture = texture;
	assert(m_texture != NULL);

	// Create some sub-bombs
	for (int i=0; i<24; i++)
		m_bombs.insertHead(new ComboSubBomb());
	m_exploding = true;
}

ComboBomb::~ComboBomb() {
}

void ComboBomb::draw(ObjType list) {
	if (list != Trans)
		return;

	m_texture->select();

	ListNode<ComboSubBomb> * p = m_bombs.getHead();
	while (p) {
		(*p)->draw();
		p = p->getNext();
	}
}

void ComboBomb::nextFrame(uint64 tm) {
	// Do our own animation first
	if (!m_exploding) {
		if (m_index == 1) {
			setTranslate(Vector(
				600.0f - fcos(m_frame * (2*M_PI/4) / (hz/2.0f)) * 300.0f,
				240.0f + fsin(m_frame * (2*M_PI/4) / (hz/2.0f)) * 240.0f,
				0));
		} else {
			setTranslate(Vector(
				 40.0f + fcos(m_frame * (2*M_PI/4) / (hz/2.0f)) * 300.0f,
				240.0f + fsin(m_frame * (2*M_PI/4) / (hz/2.0f)) * 240.0f,
				0));
		}
		if (m_frame >= (25*hz/60))
			explode();
	}
	
	Drawable::nextFrame(tm);

	Color col = getColor();

	int i = 0;
	ListNode<ComboSubBomb> * p = m_bombs.getHead();
	while (p) {
		(*p)->nextFrame(tm);

		if ((*p)->getColor().a <= 0.0f) {
			if (m_exploding) {
				(*p)->setColor(Color(0, 0, 0, 0));
				(*p)->setVel(0.0f, 0.0f);
			} else {
				const Vector & v = getTranslate();
				(*p)->recycle(v.x, v.y, v.z + i/100.0f);
			}
		}
		col.a = (*p)->getColor().a;
		(*p)->setColor(col);

		i++;
		p = p->getNext();
	}
	m_frame++;
}

void ComboBomb::explode() {
	if (m_exploding)
		return;
		
	m_exploding = true;
	
	ListNode<ComboSubBomb> * p = m_bombs.getHead();
	while (p) {
		(*p)->explode();
		p = p->getNext();
	}
}

void ComboBomb::reset() {
	if (!m_exploding)
		return;

	m_frame = 0;
	m_exploding = false;
}
