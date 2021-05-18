#pragma once
#include <map>
#include "Player.h"

class ScoreboardManager
{
public:
	static ScoreboardManager& Get()
	{
		static ScoreboardManager sInstance;
		return sInstance;
	}

	void UpdateScoreboard();
	void RenderScoreborad();

private:
	ScoreboardManager(){}
	~ScoreboardManager(){}
};