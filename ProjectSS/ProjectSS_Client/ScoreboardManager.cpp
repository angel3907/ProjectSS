#include "ScoreboardManager.h"
#include "SDLRenderer.h"
#include "stdafx.h"

void ScoreboardManager::UpdateScoreboard()
{
	StarCountToName.clear();

	for (auto Player_ : LinkingContext::Get().GetPlayerPtrSet())
	{
		StarCountToName.push_back({Player_->GetStarCount(), Player_->GetName()});
	}

	sort(StarCountToName.begin(), StarCountToName.end(), std::greater<>());
}

void ScoreboardManager::RenderScoreborad()
{
	SDLRenderer::Get().DrawTexture('SCOR', Vector2(WINDOW_WIDTH - (WINDOW_WIDTH - WORLD_WIDTH) * 0.5f, WINDOW_HEIGHT * 0.5f));

	float BasePosX = (WINDOW_WIDTH - (WINDOW_WIDTH - WORLD_WIDTH)) * 1.075f;
	float BasePosY = (WINDOW_HEIGHT * 0.195f);

	SDL_Color BlackColor = {0, 0, 0};
	int Index = 0;
	int XOffset = 200;
	int YOffset = 75;

	for (auto StarCountToNameValue : StarCountToName)
	{
		SDLRenderer::Get().DrawFont('SMAL', BlackColor, Vector2(BasePosX, BasePosY + Index * YOffset), StarCountToNameValue.second.c_str());
		SDLRenderer::Get().DrawFont('SMAL', BlackColor, Vector2(BasePosX + XOffset, BasePosY + Index * YOffset), std::to_string(StarCountToNameValue.first).c_str());
		Index++;
	}
}
