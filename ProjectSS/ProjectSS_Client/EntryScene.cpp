#include "stdafx.h"
#include "EntryScene.h"
#include "SDLRenderer.h"
#include "SquareButton.h"
#include "CircleButton.h"
#include "TextInputBox.h"
#include "TextBox.h"

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

	NameInputBox = new TextInputBox(Vector2(WINDOW_WIDTH * 0.605f, WINDOW_HEIGHT * 0.248f), 530, 60, ETextSize::EXTREME, NameLimit);

	IPInputBox = new TextInputBox(Vector2(WINDOW_WIDTH * 0.328f, WINDOW_HEIGHT * 0.675f), 550, 60, ETextSize::EXTREME, IPLimit);
	PortInputBox = new TextInputBox(Vector2(WINDOW_WIDTH * 0.843f, WINDOW_HEIGHT * 0.675f), 250, 60, ETextSize::EXTREME, PortLimit);
	EntryStatusTextBox = new TextBox(Vector2(WINDOW_WIDTH * 0.383f, WINDOW_HEIGHT * 0.795f), 730, 60, ETextSize::LARGE);

	EntryStatusTextBox->SetText("Click the enter button");

	InitServerStateToStringMap();
}

EntryScene::~EntryScene()
{
	delete EntryButton;

	for (int i = 0; i < 6; i++)
	{
		delete ColorButton[i];
	}

	delete NameInputBox;

	delete IPInputBox;
	delete PortInputBox;
	delete EntryStatusTextBox;
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

	IPInputBox->Render();
	PortInputBox->Render();
	EntryStatusTextBox->Render();
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
	IPInputBox->CheckInput(InPos);
	PortInputBox->CheckInput(InPos);
}

void EntryScene::ProcessInput(SDL_Event* InEvent)
{
	if (NameInputBox->IsStartInput())
	{
		NameInputBox->ProcessInput(InEvent);
	}

	if (IPInputBox->IsStartInput())
	{
		IPInputBox->ProcessInput(InEvent);
	}

	if (PortInputBox->IsStartInput())
	{
		PortInputBox->ProcessInput(InEvent);
	}
}

void EntryScene::InitServerStateToStringMap()
{
	ServerStateToStringMap[ServerState::NO_SERVER] = "There's no server with this IP and port number.";
	ServerStateToStringMap[ServerState::FULL_PLAYER] = "The Room is full.";
	ServerStateToStringMap[ServerState::GAME_STARTED] = "Game Is Already Started.";
}
