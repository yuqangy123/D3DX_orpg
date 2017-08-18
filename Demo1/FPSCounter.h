#pragma once
#include <windows.h>//<>������׼��Ŀ¼������������Ŀ¼

#define ELAPSEDTIMER		50
class CFPSCounter
{
public:
	CFPSCounter(void);
	~CFPSCounter(void);

	void start();
	double GetFPS(int &nFPS);

private:
	_LARGE_INTEGER	m_time_start;    /*��ʼʱ��*/
	_LARGE_INTEGER	m_time_over;        /*����ʱ��*/
	double					m_dqFreq;                /*��ʱ��Ƶ��*/
	LARGE_INTEGER	m_f;            /*��ʱ��Ƶ��*/
	LONGLONG			m_startTime;
	double					m_timelist[ELAPSEDTIMER];
	INT64						m_nElapsedTimeIndex;
};
