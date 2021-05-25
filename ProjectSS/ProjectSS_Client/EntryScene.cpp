#include "stdafx.h"
#include "EntryScene.h"
#include "SDLRenderer.h"
#include "SquareButton.h"
#include "CircleButton.h"
#include "TextInputBox.h"

EntryScene::EntryScene()
{
	SelectedPlayerColor = RED;
	CheckMarkPos = Vector2(WINDOW_WIDTH * 0.455f, WINDOW_HEIGHT * 0.34f);
	CheckMarkOffsetX = Vector2(98, 0);
	PlayerColorPos = Vector2(WINDOW_WIDTH * 0.175f, WINDOW_HEIGHT * 0.31f);

	ColorButtonPos = Vector2(WINDOW_WIDTH * 0.443f, WINDOW_HEIGHT * 0.366f);

	EntryButton = new SquareButton(Vector2(WINDOW_WIDTH * 0.834f, WINDOW_HEIGHT * 0.832f), 303, 122);
	
	for (int i = 0; i < 6; i++)
	{
		ColorButton[i] = new CircleButton(ColorButtonPos + CheckMarkOffsetX * i, 40);
	}

	NameInputBox = new TextInputBox(Vector2(WINDOW_WIDTH * 0.605f, WINDOW_HEIGHT * 0.248f), 530, 60, NameLimit);
}

EntryScene::~EntryScene()
{
	delete EntryButton;

	for (int i = 0; i < 6; i++)
	{
		delete ColorButton[i];
	}

	delete NameInputBox;
}

void EntryScene::Update()
{

}

void EntryScene::Render()
{
 	SDLRenderer::Get().DrawTexture('ENTR', Vector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f));
 	SDLRenderer::Get().DrawTexture('CHEK', CheckMarkPos + CheckMarkOffsetX * SelectedPlayerColor);
 	SDLRenderer::Get().DrawTexture(SelectedPlayerColor, PlayerColorPos);

	NameInputBox->Render();

//버튼 테스트 렌더링
// 	EntryButton->Render();
// 
// 	for (int i = 0; i < 6; i++)
// 	{
// 		ColorButton[i]->Render();
// 	}
}

void EntryScene::HandleInput(SDL_Event* InEvent)
{
	switch (InEvent->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (InEvent->button.button == SDL_BUTTON_LEFT)
		{ 
			CheckButtonsPressed(Vector2(InEvent->button.x, InEvent->button.y));
		}
		break;
	case SDL_TEXTINPUT:
	case SDL_KEYDOWN:
		ProcessInput(InEvent);
		break;
	}
}

void EntryScene::CheckButtonsPressed(Vector2 InPos)
{
	if (EntryButton->IsPressed(InPos))
	{
		printf("Pressed\n");
	}

	for (int i = 0; i < 6; i++)
	{
		if (ColorButton[i]->IsPressed(InPos))
		{
			SelectedPlayerColor = (PlayerColor)i;
		}
	}

	NameInputBox->CheckInput(InPos);
}

void EntryScene::ProcessInput(SDL_Event* InEvent)
{
	if (NameInputBox->IsStartInput())
	{
		NameInputBox->ProcessInput(InEvent);
	}
}
