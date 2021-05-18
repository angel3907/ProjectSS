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
	std::map<std::string, int32_t> NameToStarCountMap;

	ScoreboardManager(){}
	~ScoreboardManager(){}
};