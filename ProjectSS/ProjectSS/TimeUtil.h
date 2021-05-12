#pragma once
#include <profileapi.h>

class TimeUtil
{
public:
	static TimeUtil& Get()
	{
		static TimeUtil sInstance;
		return sInstance;
	}

	float GetDeltaTimef()
	{
		return m_fDeltaTime;
	}

	double GetDeltaTimed()
	{
		return m_dDeltaTime;
	}

	double GetDeltaTime() {return m_dDeltaTime;}

	float GetFrameStartTime() { return mFrameStartTimef; }

	void Update()
	{
		QueryPerformanceCounter(&m_CurrentCounter);
		
		//delta time = current time - previous time
		//두 프레임 사이의 카운터에 주기를 곱하면 두 프레임 사이의 시간을 구할 수 있음.
		m_dDeltaTime = (static_cast<double>(m_CurrentCounter.QuadPart) - static_cast<double>(m_PrevCounter.QuadPart)) * m_CountDuration;
		
		m_fDeltaTime = static_cast<float>(m_dDeltaTime);
		m_PrevCounter = m_CurrentCounter;

		//프레임 시작 시간 설정
		mFrameStartTimef = static_cast<float>(GetTime());
	};

	double GetTime()
	{
		LARGE_INTEGER CurTime, TimeSinceStart;
		QueryPerformanceCounter(&CurTime);

		//시작한지로부터의 카운터 수를 구하고
		TimeSinceStart.QuadPart = CurTime.QuadPart - m_StartCounter.QuadPart;
		
		//주기를 곱해서 현재 시간을 구함
		return TimeSinceStart.QuadPart * m_CountDuration;
	}

	float GetTimef()
	{
		return static_cast<float>(GetTime());
	}

private:
	TimeUtil()
	{
		Reset();
	}

	void Reset()
	{
		//이전 프레임과 현재 프레임 사이의 시간을 정확히 계산하기 위한 함수
		QueryPerformanceFrequency(&m_CountFreq); //성능 카운터의 빈도를 돌려줌. 시스템 부팅시 고정, 모든 프로세서에서 일관됨
		m_CountDuration = 1.0 / m_CountFreq.QuadPart;

		QueryPerformanceCounter(&m_CurrentCounter); //고해상도(1us) 타임 스탬프인 성능 카운터의 현재 값을 돌려줌.
		QueryPerformanceCounter(&m_PrevCounter);

		QueryPerformanceCounter(&m_StartCounter); //시작 카운터값 저장
	}

	LARGE_INTEGER m_CountFreq;
	LARGE_INTEGER m_CurrentCounter;
	LARGE_INTEGER m_PrevCounter;
	LARGE_INTEGER m_StartCounter;

	double m_CountDuration;

	float mFrameStartTimef;

	//각 타입별 델타타임
	double m_dDeltaTime;
	float m_fDeltaTime;

};