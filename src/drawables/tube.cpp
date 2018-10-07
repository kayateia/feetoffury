/* 
   Feet of Fury

   about/tube.cc

   Copyright (C)2000-2002 Cryptic Allusion, LLC

   Based on the "Tube" level engine for Tryptonite
*/

#include "global.h"
#include "tube.h"
#include "hsv.h"
#include <Tiki/bspline.h>

static float minz = 1000.0f, maxz = 0.0f;

BSpline bsp;

Tube::Tube(int list, Texture * txr) {
	m_list = list;

	m_tubeSize = m_lastTubeSize = 2.0f;
	
	m_txr = txr;
	m_txrUOffs = m_txrVOffs = 0.0f;

	m_palpos = 0;
	m_hsv = 0.0f;

	m_controlPos = 1;
	m_trackPos = 0;

	Vector z( 0.0f, 0.0f, 0.0f );
	m_plrPos = z;
	m_plrVel = z;
	m_plrVel.z = +0.3f;
	
	m_viewXr = 0;
	m_viewYr = 0;
	m_viewZr = 0;

	palInit();
}

Tube::~Tube() {
	// printf("minz was %f, maxz was %f\n", (double)minz, (double)maxz);
}

/**************************************************************************/
/* Tube color lighting effects */

/* Initialize the palette tables */
void Tube::palInit() {
	int x;
	
	/* for (x=0; x<64; x++) {
		m_palette[x].a = 1.0f;
		m_palette[x].r = x/64.0f;
		m_palette[x].g = 0.0f;
		m_palette[x].b = x/128.0f;
	}
	for (x=0; x<64; x++) {
		m_palette[64+x].a = 1.0f;
		m_palette[64+x].r = 1.0f - (x/64.0f);
		m_palette[64+x].g = 0.0f;
		m_palette[64+x].b = (x+64)/128.0f;
	}
	for (x=0; x<64; x++) {
		m_palette[128+x].a = 1.0f;
		m_palette[128+x].r = 0.0f;
		m_palette[128+x].g = x/64.0f;
		m_palette[128+x].b = 1.0f - (x/128.0f);
	}
	for (x=0; x<64; x++) {
		m_palette[192+x].a = 1.0f;
		m_palette[192+x].r = 0.0f;
		m_palette[192+x].g = 1.0f - (x/64.0f);
		m_palette[192+x].b = 1.0f - ((x+64)/128.0f);
	} */

	for (x=0; x<64; x++) {
		m_palette[x].a = 1.0f;
		m_palette[x].r = x/64.0f;
		m_palette[x].g = x/64.0f;
		m_palette[x].b = x/64.0f;
	}
	for (x=0; x<64; x++) {
		m_palette[x+64].a = 1.0f;
		m_palette[x+64].r = 1.0f - x/64.0f;
		m_palette[x+64].g = 1.0f - x/64.0f;
		m_palette[x+64].b = 1.0f - x/64.0f;
	}
	memcpy(m_palette+128, m_palette, sizeof(m_palette[0])*128);
}

/* Step to the next color increment */
void Tube::palStep() {
	m_palpos+=8;
	if (m_palpos >= 256)
		m_palpos = 0;
}

/**************************************************************************/
/* Tube path generation (uses bspline module) */

/* Path points (distance between points is 1.0f) */
#define PATH_PNTS 16
static Vector path_pnts[PATH_PNTS] = {
	Vector( -1.0f, -1.0f, 0.1f ),	Vector(  2.0f, -1.0f, 1.1f ),
	Vector( -2.0f,  1.0f, 0.1f ),	Vector(  1.0f,  2.0f, 1.1f ),
	Vector( -1.0f, -1.0f, 0.1f ),	Vector( -0.5f,  0.5f, 1.1f ),
	Vector(  4.0f, -2.0f, 0.1f ),	Vector( -3.0f,  2.0f, 1.1f ),
	Vector(  2.0f,  2.0f, 0.1f ),	Vector( -1.0f,  1.0f, 1.1f ),
	Vector( -1.0f, -1.0f, 0.1f ),	Vector(  1.0f,  1.0f, 1.1f ),
	Vector( -1.0f, -1.0f, 0.1f ),
					Vector(  2.0f, -1.0f, 0.1f ),
	Vector( -2.0f,  1.0f, 0.1f ),	Vector(  1.0f,  2.0f, 1.1f ),
};

/* Generate the ring points for a given frame */
void Tube::circlesGenRingPnts() {
	float	t;
	int	rpi, cpi;
	
	/* Generate the co-efficients for the first points */
	bsp.setPoints(path_pnts+m_controlPos);
	
	t = m_trackPos; cpi = m_controlPos;
	for (rpi=0; rpi<TUBE_RINGS; rpi++) {
		/* Check for new co-efficients needed */
		while (t >= 1.0f) {
			t -= 1.0f;
			cpi++;
			if (cpi >= (PATH_PNTS - 3)) cpi = 1;
			bsp.setPoints(path_pnts+cpi);
		}
		
		/* Generate a point */
		m_ringPnts[rpi] = bsp.getPoint(t);
		/* m_ringPnts[rpi].x *= 1.5f;
		m_ringPnts[rpi].y *= 1.5f; */
		m_ringPnts[rpi].z = - rpi*30.0f / TUBE_RINGS;
		
		/* Move forwards */
		t += 1.0f / 16.0f;
	}
}

/* "Move forward" along the track */
void Tube::circlesMoveFwd() {
	/* Move up in the B-spline curve */
	m_trackPos += m_plrVel.z / 16.0f;
	
	/* If we went over, scoot up in the control point array */
	while (m_trackPos >= 1.0f) {
		m_trackPos -= 1.0f;
		m_controlPos++;
		if (m_controlPos >= (PATH_PNTS - 3)) m_controlPos = 1;
	}
}

/* Initialize circle creation (per frame) */
void Tube::circlesNewFrame() {
	/* Initialize circle position */
	m_cirPos = 1;

	/* Tunnel movement */
	circlesMoveFwd();
	
	/* Generate ring data */
	circlesGenRingPnts();
}

/* Create the next circle; save the previous one in cir_lx and cir_ly */
void Tube::circlesNext() {
	m_cirPos++;
}


/**************************************************************************/
/* Tube polygon generation */

// This cheats a little and smashes everything back as far as it can go in the
// Z direction so it appears on bottom of everything visibility-wise.
static inline void plx_vert_ifdm3_l(plx_dr_state_t * state, int flags, float x, float y, float z,
	uint32 color, float u, float v)
{
	plx_mat_tfip_3d(x, y, z);
	if (z <= 0.0331125) z = 0.033f;
	z = z - 0.033f;
	z = z * 0.002f / (0.45f - 0.033f);
	z += 0.001f;
	// if (z <= 0.0f) z = 0.001f;
	/* if (z < minz) minz = z;
	if (z > maxz) maxz = z; */
	plx_vert_ifd(state, flags, x, y, z, color, u, v);
}
        

/* Polygonize the current tube ring (using circle parameters above) */
void Tube::circlesPolyize(const Color & tint) {
	int		theta;
	Color		pals[2];
	uint32		cols[2];
	float		cir_x, cir_y, cir_z;
	float		cir_lx, cir_ly, cir_lz;
	float		poffs[2];
	
	memcpy(pals, m_palette + (m_palpos+(m_cirPos-1)*16) % 256, sizeof(Color));
	memcpy(pals+1, m_palette + (m_palpos+(m_cirPos)*16) % 256, sizeof(Color));

	pals[0].r *= (1.0f - (m_cirPos-1)/40.0f) * tint.r;
	pals[0].g *= (1.0f - (m_cirPos-1)/40.0f) * tint.g;
	pals[0].b *= (1.0f - (m_cirPos-1)/40.0f) * tint.b;
	pals[1].r *= (1.0f - m_cirPos/40.0f) * tint.r;
	pals[1].g *= (1.0f - m_cirPos/40.0f) * tint.g;
	pals[1].b *= (1.0f - m_cirPos/40.0f) * tint.b;
	cols[0] = plx_pack_color(1.0f, pals[0].r, pals[0].g, pals[0].b);
	cols[1] = plx_pack_color(1.0f, pals[1].r, pals[1].g, pals[1].b);
	
	//pals[0].r = pals[0].g = pals[0].b = 1.0f - cirpos/40.0f;
	//pals[1].r = pals[1].g = pals[1].b = 1.0f - (cirpos+1)/40.0f;

	cir_lx = m_ringPnts[m_cirPos-1].x;
	cir_ly = m_ringPnts[m_cirPos-1].y;
	cir_lz = m_ringPnts[m_cirPos-1].z;
	cir_x = m_ringPnts[m_cirPos].x;
	cir_y = m_ringPnts[m_cirPos].y;
	cir_z = m_ringPnts[m_cirPos].z;

	poffs[0] = ((m_cirPos-1) * 4 / TUBE_RINGS + m_palpos / 256.0f) * 2*M_PI;
	poffs[1] = ((m_cirPos) * 4 / TUBE_RINGS + m_palpos / 256.0f) * 2*M_PI;

	plx_dr_state_t dr;
	plx_dr_init(&dr);
	
	/* Use STEPS steps (STEPS polys per segment) */
	// int toggle = 0;
	for (theta=0; theta<=1024; theta+=(1024/TUBE_STEPS)) {
		// float u = toggle ? 1.0f : 0.0f; toggle ^= 1;
		float u = 0.0f;
		plx_vert_ifdm3_l(&dr, PLX_VERT,
			fcos(theta * 2*M_PI / 1024)*m_tubeSize+cir_x,
			fsin(theta * 2*M_PI / 1024)*m_tubeSize+cir_y,
			cir_z,
			// cols[1],
			plx_pack_color(1.0f,
				pals[1].r * (fsin(poffs[1] + 4*theta*2*M_PI/1024)/4.0f+0.75f),
				pals[1].g * (fsin(poffs[1] + 4*theta*2*M_PI/1024)/4.0f+0.75f),
				pals[1].b * (fsin(poffs[1] + 4*theta*2*M_PI/1024)/4.0f+0.75f)),
			u + m_txrUOffs, 1.0f + m_txrVOffs);

		plx_vert_ifdm3_l(&dr, theta == 1024 ? PLX_VERT_EOS : PLX_VERT,
			fcos(theta * 2*M_PI / 1024)*m_lastTubeSize+cir_lx,
			fsin(theta * 2*M_PI / 1024)*m_lastTubeSize+cir_ly,
			cir_lz,
			// cols[0],
			plx_pack_color(1.0f,
				pals[0].r * (fsin(poffs[0] + 4*theta*2*M_PI/1024)/4.0f+0.75f),
				pals[0].g * (fsin(poffs[0] + 4*theta*2*M_PI/1024)/4.0f+0.75f),
				pals[0].b * (fsin(poffs[0] + 4*theta*2*M_PI/1024)/4.0f+0.75f)),
			u + m_txrUOffs, 0.0f + m_txrVOffs);
	#if 0
		plx_vert_ifdm3(&dr, PLX_VERT, -3.0f, 3.0f, -10.0f,
			0xffffffff,
			0.0f + txr_u_offs, 1.0f + txr_v_offs);
		plx_vert_ifdm3(&dr, PLX_VERT, -3.0f, -3.0f, -10.0f,
			0xffffffff,
			0.0f + txr_u_offs, 0.0f + txr_v_offs);
		plx_vert_ifdm3(&dr, PLX_VERT, 3.0f, 3.0f, -10.0f,
			0xffffffff,
			1.0f + txr_u_offs, 1.0f + txr_v_offs);
		plx_vert_ifdm3(&dr, PLX_VERT_EOS, 3.0f, -3.0f, -10.0f,
			0xffffffff,
			1.0f + txr_u_offs, 0.0f + txr_v_offs);
	#endif
	}
}

/**************************************************************************/
/* Main tube draw routine -- call once per frame */
/* float tube_constrict_phase = 0.0f; */
void Tube::draw(ObjType list) {
	if (list != m_list)
		return;

	// plx_cxt_texture(*m_txr);
	plx_cxt_texture(NULL);
	// plx_cxt_culling(PLX_CULL_NONE);
	plx_cxt_send(m_list);
	/* m_txr->setFilter(Texture::FilterBilinear);
	m_txr->setUVClamp(Texture::UVRepeat, Texture::UVRepeat);
	m_txr->sendHdr(m_list); */

	/* Grab next palette index */
	palStep();
	
	/* Initialize circle generation routine */
	circlesNewFrame();

	/* Center the view on the first circle and add player position */
	m_view.x = -m_ringPnts[m_cirPos].x;
	m_view.y = -m_ringPnts[m_cirPos].y;
	m_view.z = -0.2f;

	// tube_constrict_phase += 0.1f;

	/* Setup transformation matrix */
	plx_mat3d_identity();
	// plx_mat3d_translate(0.0f, 0.4f*m_tubeSize, 0.0f);
	plx_mat3d_rotate(1.0f, m_viewXr, m_viewYr, m_viewZr);
	plx_mat3d_translate(m_view.x, m_view.y, m_view.z);

	plx_mat3d_translate(0.0f, 0.0f, -2.0f);
	
	plx_mat_identity();
	plx_mat3d_apply(PLX_MAT_SCREENVIEW);
	plx_mat3d_apply(PLX_MAT_PROJECTION);
	plx_mat3d_apply(PLX_MAT_MODELVIEW);

	float r, g, b;
	hsvSpectrum(m_hsv, r, g, b);

	Color tint = getColor() * Color(r, g, b);

	/* Draw RINGS segments (count points) */
	for (int i=0; i<TUBE_RINGS-2; i++) {
		//m_lastTubeSize = m_tubeSize;
		//m_tubeSize = sin(tube_constrict_phase + i * 2 * 3.1415 / RINGS) / 2.0f + 1.5f;
		circlesPolyize(tint);
		circlesNext();
	}

#if 0
	/* Rotational texture motion */
	m_txrUOffs += m_plrSpin;
	if (m_txrUOffs >= 1.0f)
		m_txrUOffs -= ((int)m_txrUOffs);
	
	/* Velocity texture motion */
	m_txrVOffs = m_plrPos.z;
	if (m_txrVOffs > 1.0f)
		m_txrVOffs -= ((int)m_txrVOffs);

	m_plrPos.z += m_plrVel.z;
#endif

	m_hsv += 1.0f / (8*hz);
	while (m_hsv >= 1.0f)
		m_hsv -= 1.0f;
}

void Tube::nextFrame(uint64 tm) {
}

/**************************************************************************/
/* Player controls */

#if 0
static void plr_do_motion(const cont_cond_t *cond) {
	int x, y;
	x = y = 0;

	/* If the player is pressing left, then rotate the tunnel
	   in the counter-clockwise direction. */
	if (!(cond->buttons & CONT_DPAD_LEFT))
		x -= 32;
	
	/* If the player is pressing right, then rotate the tunnel
	   in the clockwise direction. */
	if (!(cond->buttons & CONT_DPAD_RIGHT))
		x += 32;

	/* Calculate in analog control */
	x += (((int)cond->joyx) - 128) / 4;
	
	/* Apply the spin parameter */
	/* plr_spin = (x * 0.2f) / 256.0f; */
	plr_vel.x = (x * 0.1f) / 256.0f;
	view_zr -= (int)(plr_vel.x*768);
	while (view_zr < 0) view_zr += 1024;
	while (view_zr >= 1024) view_zr -= 1024;
	
	/* Apply constant player velocity */
	plr_vel.z = 0.3f;
	plr_pos.z += plr_vel.z;
}
#endif

