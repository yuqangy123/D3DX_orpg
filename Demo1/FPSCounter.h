#pragma once
#include <windows.h>//<>搜索标准库目录，“”搜索根目录

#define ELAPSEDTIMER		50
class CFPSCounter
{
public:
	CFPSCounter(void);
	~CFPSCounter(void);

	void start();
	double GetFPS(int &nFPS);

private:
	_LARGE_INTEGER	m_time_start;    /*开始时间*/
	_LARGE_INTEGER	m_time_over;        /*结束时间*/
	double					m_dqFreq;                /*计时器频率*/
	LARGE_INTEGER	m_f;            /*计时器频率*/
	LONGLONG			m_startTime;
	double					m_timelist[ELAPSEDTIMER];
	INT64						m_nElapsedTimeIndex;
};
