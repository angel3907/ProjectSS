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
		//�� ������ ������ ī���Ϳ� �ֱ⸦ ���ϸ� �� ������ ������ �ð��� ���� �� ����.
		m_dDeltaTime = (static_cast<double>(m_CurrentCounter.QuadPart) - static_cast<double>(m_PrevCounter.QuadPart)) * m_CountDuration;
		
		m_fDeltaTime = static_cast<float>(m_dDeltaTime);
		m_PrevCounter = m_CurrentCounter;

		//������ ���� �ð� ����
		mFrameStartTimef = static_cast<float>(GetTime());
	};

	double GetTime()
	{
		LARGE_INTEGER CurTime, TimeSinceStart;
		QueryPerformanceCounter(&CurTime);

		//���������κ����� ī���� ���� ���ϰ�
		TimeSinceStart.QuadPart = CurTime.QuadPart - m_StartCounter.QuadPart;
		
		//�ֱ⸦ ���ؼ� ���� �ð��� ����
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
		//���� �����Ӱ� ���� ������ ������ �ð��� ��Ȯ�� ����ϱ� ���� �Լ�
		QueryPerformanceFrequency(&m_CountFreq); //���� ī������ �󵵸� ������. �ý��� ���ý� ����, ��� ���μ������� �ϰ���
		m_CountDuration = 1.0 / m_CountFreq.QuadPart;

		QueryPerformanceCounter(&m_CurrentCounter); //���ػ�(1us) Ÿ�� �������� ���� ī������ ���� ���� ������.
		QueryPerformanceCounter(&m_PrevCounter);

		QueryPerformanceCounter(&m_StartCounter); //���� ī���Ͱ� ����
	}

	LARGE_INTEGER m_CountFreq;
	LARGE_INTEGER m_CurrentCounter;
	LARGE_INTEGER m_PrevCounter;
	LARGE_INTEGER m_StartCounter;

	double m_CountDuration;

	float mFrameStartTimef;

	//�� Ÿ�Ժ� ��ŸŸ��
	double m_dDeltaTime;
	float m_fDeltaTime;

};