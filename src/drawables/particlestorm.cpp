/* 
   Feet of Fury
   
   drawables/particlestorm.c
   
   Copyright (C)2003 Cryptic Allusion, LLC
*/

#include "global.h"
#include "particlestorm.h"

SubPart::SubPart() {
	m_color = Color(0, 1, 1, 1);
	m_vx = m_vy = 0.0f;
	m_tx = m_ty = m_tz = 0.0f;
}

SubPart::~SubPart() {
}

// Draw a single twinkley
void SubPart::draw() {
	float s = 64.0f * m_sc;
	plx_dr_state_t dr;

	plx_dr_init(&dr);
	plx_spr_fnd(&dr, s, s, m_tx, m_ty, m_tz, m_color.a, m_color.r, m_color.g, m_color.b);
}

void SubPart::nextFrame(uint64 tm) {
	/* if (m_vx != 0.0f) {
		m_color.a -= 0.01f;
		m_sc += 0.01f;
	} else {
		m_color.a -= 0.05f;
		m_sc -= 0.005f;
	} */
	m_color.a -= 1.0f/(hz/2);
	if (m_color.a < 0.0f)
		m_color.a = 0.0f;
	if (m_sc < 0.0f)
		m_sc = 0.0f;

	m_tx += m_vx;
	m_ty += m_vy;
}

void SubPart::recycle(float x, float y, float z) {
	m_color.a = 1.0f - (rand() % 1000) * 0.001f;
	m_tx = x + (rand() % 12) - 6;
	m_ty = y + (rand() % 12) - 6;
	m_tz = z;
	m_sc = 1.0f;
	explode();
}

void SubPart::explode() {
	m_vx = (rand() % 8) - 4;
	m_vy = (rand() % 8) - 4;
}

void SubPart::setVel(float x, float y) {
	m_vx = x;
	m_vy = y;
}


ParticleStorm::ParticleStorm(Texture * texture, int cnt) {
	m_texture = texture;
	assert(m_texture != NULL);

	// Create some sub-bombs
	for (int i=0; i<cnt; i++)
		m_bombs.insertHead(new SubPart());
	m_exploding = false;
}

ParticleStorm::~ParticleStorm() {
}

void ParticleStorm::draw(ObjType list) {
	if (list != Trans)
		return;

	m_texture->select();

	ListNode<SubPart> * p = m_bombs.getHead();
	while (p) {
		(*p)->draw();
		p = p->getNext();
	}
}

void ParticleStorm::nextFrame(uint64 t) {
	Drawable::nextFrame(t);

	Color col = getColor();

	int i = 0;
	ListNode<SubPart> * p = m_bombs.getHead();
	while (p) {
		(*p)->nextFrame(t);

		if ((*p)->getColor().a <= 0.0f) {
			if (m_exploding) {
				(*p)->setColor(Color(0, 0, 0, 0));
				(*p)->setVel(0.0f, 0.0f);
			} else {
				const Vector & v = getTranslate();
				(*p)->recycle(v.x, v.y, v.z + i/10.0f);
			}
		}
		col.a = (*p)->getColor().a;
		(*p)->setColor(col);

		i++;
		p = p->getNext();
	}
	m_frame++;
}

void ParticleStorm::explode() {
	if (m_exploding)
		return;
		
	m_exploding = true;
	
	ListNode<SubPart> * p = m_bombs.getHead();
	while (p) {
		(*p)->explode();
		p = p->getNext();
	}
}

void ParticleStorm::reset() {
	if (!m_exploding)
		return;

	m_frame = 0;
	m_exploding = false;
}
