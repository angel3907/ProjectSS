#include "stdafx.h"
#include "EntryScene.h"
#include "SDLRenderer.h"

EntryScene::EntryScene()
{
	SelectedPlayerColor = RED;
	CheckMarkPos = Vector2(WINDOW_WIDTH * 0.455f, WINDOW_HEIGHT * 0.34f);
	CheckMarkOffsetX = Vector2(98, 0);
	PlayerColorPos = Vector2(WINDOW_WIDTH * 0.175f, WINDOW_HEIGHT * 0.31f);
}

EntryScene::~EntryScene()
{

}

void EntryScene::Update()
{

}

void EntryScene::Render()
{
	SDLRenderer::Get().DrawTexture('ENTR', Vector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f));
	SDLRenderer::Get().DrawTexture('CHEK', CheckMarkPos + CheckMarkOffsetX * SelectedPlayerColor);
	SDLRenderer::Get().DrawTexture(SelectedPlayerColor, PlayerColorPos);
}
