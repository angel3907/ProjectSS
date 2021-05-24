#include "stdafx.h"
#include "InGameScene.h"
#include "SDLRenderer.h"
#include "ScoreboardManager.h"

void InGameScene::Update()
{
	ScoreboardManager::Get().UpdateScoreboard();
}

void InGameScene::Render()
{
	SDLRenderer::Get().DrawBackground();
	ScoreboardManager::Get().RenderScoreborad();
}

InGameScene::InGameScene()
{

}

InGameScene::~InGameScene()
{

}
