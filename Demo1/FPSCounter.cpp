#include "StdAfx.h"
#include "FPSCounter.h"

CFPSCounter::CFPSCounter(void)
{
	QueryPerformanceFrequency(&m_f);
	m_dqFreq = (double)m_f.QuadPart;
}

CFPSCounter::~CFPSCounter(void)
{
}


void CFPSCounter::start()
{
	QueryPerformanceCounter(&m_time_start);
	memset(m_timelist, 0x0, sizeof(m_timelist));
	m_nElapsedTimeIndex = 0;
}

double CFPSCounter::GetFPS(int &nFPS)
{
	QueryPerformanceCounter(&m_time_over);
	double passtime = (double)(m_time_over.QuadPart-m_time_start.QuadPart)/m_dqFreq;
	m_timelist[(++m_nElapsedTimeIndex)%ELAPSEDTIMER] = passtime;
	
	double CountTime=0.0;
	for (int n=0; n<ELAPSEDTIMER; ++n)
		CountTime+=m_timelist[n];
	
	CountTime = CountTime/(double)ELAPSEDTIMER;
	m_time_start = m_time_over;

	nFPS = (int)(1.0/CountTime);
	return passtime;
}