/* 
   Feet of Fury
   
   drawables/cubetunnel.c
   
   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "cubetunnel.h"
#include "cube.h"

CubeTunnel::CubeTunnel(Texture * txr, bool out_of_screen) {
	m_txr = txr;
	assert(m_txr != NULL);

	m_cc = 16;
	m_cubes = new RefPtr<Cube>[m_cc];
	m_out = out_of_screen;

	for (int i=0; i<m_cc; i++) {
		m_cubes[i] = new Cube(Opaque, m_txr);
		m_cubes[i]->setTranslate(Vector(
			((float)randnum(46)) - 23.0f,
			((float)randnum(46)) - 23.0f,
			-(((float)randnum(300) - 150) + 200.0f)));
		m_cubes[i]->setRotate(Vector(randnum(360*2)-360, randnum(360*2)-360, randnum(360*2)-360, 1.0f));

		// Do this so prelative colors work
		subAdd(m_cubes[i]);
	}
}

CubeTunnel::~CubeTunnel() {
	delete[] m_cubes;
}

void CubeTunnel::draw(ObjType list) {
	if (list != Opaque)
		return;

	for (int i=0; i<m_cc; i++) {
		const Vector & t = m_cubes[i]->getTranslate();
		const Vector & r = m_cubes[i]->getRotate();
	
		plx_mat3d_identity();
		plx_mat3d_translate(t.x, t.y, t.z);
		plx_mat3d_rotate(r.w, r.x, r.y, r.z);
		plx_mat3d_scale(2.0f, 2.0f, 2.0f);
		m_cubes[i]->drawCube();
	}
}

void CubeTunnel::nextFrame(uint64 tm) {
	for (int i=0; i<m_cc; i++) {
		Vector r = m_cubes[i]->getRotate();
		Vector t = m_cubes[i]->getTranslate();
		
		r.w += 4.0f;
		if (r.w >= 360.0f)
			r.w -= 360.0f;
		if (m_out) {
			t.z += 0.8f;
			if (t.z >= 4.0f)
				t.z = -(((float)randnum(300) - 150) + 200.0f);
		} else {
			t.z -= 0.8f;
			if (t.z <= -300.0f)
				t.z = 4.0f;
		}

		m_cubes[i]->setRotate(r);
		m_cubes[i]->setTranslate(t);
	}
}

