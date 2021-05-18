#include "ScoreboardManager.h"
#include "SDLRenderer.h"

void ScoreboardManager::UpdateScoreboard()
{
}

void ScoreboardManager::RenderScoreborad()
{
	SDLRenderer::Get().DrawTexture('SCOR', Vector2(WINDOW_WIDTH - (WINDOW_WIDTH - WORLD_WIDTH) * 0.5f, WINDOW_HEIGHT * 0.5f));
}
