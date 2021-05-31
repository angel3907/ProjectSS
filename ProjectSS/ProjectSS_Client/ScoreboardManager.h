#pragma once
#include "Player.h"
typedef std::vector<std::pair<uint32_t, std::string>> Uint32ToString;

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
	Uint32ToString StarCountToName;

	ScoreboardManager(){}
	~ScoreboardManager(){}

	const uint32_t ScoreboardTextureKey = 'SCOR';
};