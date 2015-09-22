/*!
 * \file Timer.h
 *
 * \author Sam
 * \date September 2014
 *
 *  High performance timer
 */
#pragma once
#include <Windows.h>
class Timer
{
public:
	void setTimer() {
		__int64 countsPerSecs = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecs);
		m_fSecsPerCnt = 1.0f / (float)countsPerSecs;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lPrevTime);
	}

	float getDeltaTime() {
		m_lCurrTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lCurrTime);
		return (m_lCurrTime - m_lPrevTime) * m_fSecsPerCnt;
	}

	void updateTime() { m_lPrevTime = m_lCurrTime; }

	Timer() :
	m_lCurrTime(0),
	m_lPrevTime(0),
	m_fSecsPerCnt(0.0f)
	{}

	~Timer() {
		if (instance) { delete(instance); instance = nullptr; }
	}

	static Timer* Get() 
	{
		if (nullptr == instance)
			instance = new Timer;
		return instance;
	}
private:
		__int64	m_lPrevTime,
					m_lCurrTime;
		float		m_fSecsPerCnt;

	static Timer* instance;
};

