#pragma once
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
	std::vector<std::pair<uint32_t, std::string>> StarCountToName;

	ScoreboardManager(){}
	~ScoreboardManager(){}
};