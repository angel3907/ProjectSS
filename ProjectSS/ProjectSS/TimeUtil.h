#pragma once

class TimeUtil
{
public:
	static TimeUtil& Get()
	{
		static TimeUtil sInstance;
		return sInstance;
	}

	static float GetCurrentTime()
	{
		return 0.0f;
	}

	static float GetFrameStartTime() { return 0.f; }

	void Update(){};
};