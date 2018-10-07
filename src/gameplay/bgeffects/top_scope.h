/*      
   Feet of Fury
        
   gameplay/bgeffects/top_scope.h

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#ifndef __TOP_SCOPE_H
#define __TOP_SCOPE_H

#include "bgeffects.h"

#define SCOPE_WAVECNT 64

// A 3D oscilliscope showing a morphing approximation of the
// sound pumping out the speakers.
class TopScope : public TopDrawable {
public:
	TopScope();
	virtual ~TopScope();

	virtual void draw(ObjType list);
	virtual void nextFrame(uint64 tm);

	static void drawCube(float p1[3], float p2[3], uint32 colors[6]);

	void callback(int hz, int channels, void * buffer, int samplecnt);

private:
	class FilterCB : public Stream::Filter {
	public:
		FilterCB(TopScope * us);
		virtual ~FilterCB();
		
		virtual void filter(int freq, int chncount, void * buffer, int smpcnt);
		
		TopScope * m_us;
	};

	RefPtr<Thread::Mutex>	m_mutex;
	int16	m_buffer[3][16384];
	int	m_buffercnt[3];
	uint64	m_lasttime;
	int	m_takeframes;
	bool	m_newbuffer;
	RefPtr<FilterCB>	m_filter;

	typedef struct {
		float	pcur, pdelta;
	} wavepnt_t;
	wavepnt_t	m_wave[SCOPE_WAVECNT];

	float	m_rot;
};

#endif	/* __TOP_SCOPE_H */

