/* 
   Feet of Fury
   
   drawables/bubble_selector.cc
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "bubble_selector.h"
#include "bubble.h"
#include "cube.h"

BubbleSelector::BubbleSelector(int numchoices, int choicecur) {
	// Pull in all our defaults
	m_choice_cnt = numchoices;
	m_bubbles = new RefPtr<Bubble>[numchoices];
	m_cubes = new RefPtr<Cube>[numchoices];
	m_textures = new RefPtr<Texture>[numchoices];
	m_enables = new bool[numchoices];
	m_rots = new float[numchoices * 3];
	m_dist = 0.0f;
	m_frame = 0;
	m_finalized = false;

	for (int i=0; i<m_choice_cnt; i++) {
		m_rots[i*3+0] = (float)(rand()) / (float)(1UL<<31);
		m_rots[i*3+1] = (float)(rand()) / (float)(1UL<<31);
		m_rots[i*3+2] = (float)(rand()) / (float)(1UL<<31);
		m_enables[i] = true;
	}

	m_choice_cur = choicecur;
	/* m_choice_rot = (int)(choicecur * (360.0f / numchoices));
	m_choice_rot_dir = 0;
	m_choice_rot_dst = m_choice_rot; */
}

BubbleSelector::~BubbleSelector() {
	delete[] m_enables;
	delete[] m_textures;
	delete[] m_bubbles;
	delete[] m_cubes;
	delete[] m_rots;
}

void BubbleSelector::setTexture(int idx, Texture * txr) {
	assert(idx < m_choice_cnt);
	m_textures[idx] = txr;
}

void BubbleSelector::setEnabled(int index, bool e) {
	m_enables[index] = e;
}

void BubbleSelector::finalize() {
	// Now create our bubbles and cubes
	for (int i=0; i<m_choice_cnt; i++) {
		m_bubbles[i] = new Bubble(i == m_choice_cur ? Bubble::RedGreen
			: (m_enables[i] ? Bubble::BlueGreen : Bubble::Gray));
		m_cubes[i] = new Cube(Drawable::Trans, m_textures[i]);
		m_cubes[i]->setScale(Vector(0.5f, 0.5f, 0.5f));
		m_bubbles[i]->subAdd(m_cubes[i]);
		m_bubbles[i]->setScale(Vector(0.4f, 0.4f, 0.4f));
		subAdd(m_bubbles[i]);

		if (i == m_choice_cur)
			m_bubbles[i]->select();
	}

	m_finalized = true;
}

void BubbleSelector::draw(ObjType list) {
	assert( m_finalized );

	plx_mat3d_identity();
	pushTransformMatrix();
	Drawable::draw(list);
	popTransformMatrix();
}

void BubbleSelector::nextFrame(uint64 t) {
	assert( m_finalized );

	Drawable::nextFrame(t);
	
	// Update basic visual stuff
	for (int i=0; i<m_choice_cnt; i++) {
		//m_bubbles[i]->setScale(Vector(0.4f, 0.4f, 0.4f));
		m_bubbles[i]->setTranslate(Vector(m_dist * fcos(-i * 2*M_PI / m_choice_cnt), m_dist * fsin(-i * 2*M_PI / m_choice_cnt), 0.0f));
		m_bubbles[i]->setRotate(Vector(m_rots[i*3+0], m_rots[i*3+1], m_rots[i*3+2], m_frame*(60.0f/hz)*2));
		/* m_bubbles[i]->setAlpha(getAlpha());
		m_cubes[i]->setAlpha(getAlpha()); */
	}

	// Update choice selection animation (if any)
	/* if (m_choice_rot != m_choice_rot_dst) {
		m_choice_rot = (m_choice_rot + m_choice_rot_dir) % 360;
		if (m_choice_rot < 0)
			m_choice_rot += 360;
	} else {
		m_choice_rot = m_choice_rot_dst;
		m_choice_rot_dir = 0;
	} */

	m_frame++;
}

void BubbleSelector::choiceSwitch(int which_way) {
	assert( m_finalized );

	int nc = m_choice_cur + which_way;
	if (nc < 0)
		nc += m_choice_cnt;
	if (nc >= m_choice_cnt)
		nc -= m_choice_cnt;
	setChoice(nc);
}

void BubbleSelector::setChoice(int which) {
	assert( m_finalized );

/*	// Are we already moving? If so, ignore the request
	if (m_choice_rot_dir)
		return;

	// Move to the new choice
	m_bubbles[m_choice_cur]->deselect();
	m_choice_cur = (m_choice_cur + which_way) % m_choice_cnt;
	if (m_choice_cur < 0)
		m_choice_cur += m_choice_cnt;
	m_bubbles[m_choice_cur]->select();

	// Calculate its angle
	m_choice_rot_dst = (360/m_choice_cnt) * m_choice_cur;
	// m_choice_rot_dst = (m_choice_rot_dst + 90) % 360;
	if (m_choice_rot_dst < 0)
		m_choice_rot_dst += 360;
	m_choice_rot_dir = 3*which_way; */

	if (m_enables[m_choice_cur])
		m_bubbles[m_choice_cur]->setColor(Bubble::BlueGreen);
	else
		m_bubbles[m_choice_cur]->setColor(Bubble::Gray);
	//m_bubbles[m_choice_cur]->setScale(Vector(0.4f, 0.4f, 0.4f));
	m_bubbles[m_choice_cur]->deselect();
	m_choice_cur = which;
	m_bubbles[m_choice_cur]->setColor(Bubble::RedGreen);
	//m_bubbles[m_choice_cur]->setScale(Vector(0.5f, 0.5f, 0.5f));
	m_bubbles[m_choice_cur]->select();
}

void BubbleSelector::setDist(float d) {
	m_dist = d;
}

void BubbleSelector::update() {
	for (int i=0; i<m_choice_cnt; i++) {
		if (!m_enables[i])
			m_bubbles[i]->setColor(Bubble::Gray);
		else if (i == m_choice_cur)
			m_bubbles[i]->setColor(Bubble::RedGreen);
		else
			m_bubbles[i]->setColor(Bubble::BlueGreen);
	}
}
