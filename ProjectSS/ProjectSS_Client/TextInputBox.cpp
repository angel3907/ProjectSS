#include "TextInputBox.h"
#include "stdafx.h"

void TextInputBox::ProcessInput(SDL_Event* InEvent)
{
	if (InEvent == nullptr)
	{
		printf("InEvent is null at TextInputBox::ProcessInput\n");
		return;
	}

	if (InEvent->type == SDL_TEXTINPUT || InEvent->type == SDL_KEYDOWN)
	{
		//지우기
		if (InEvent->type == SDL_KEYDOWN && InEvent->key.keysym.sym == SDLK_BACKSPACE && Text.size() > 0)
		{
			Text = Text.substr(0, Text.size() - 1);
		}
		//입력
		else if (InEvent->type == SDL_TEXTINPUT)
		{
			if (Text.size() < TextLimit)
				Text += InEvent->text.text;
		}
	}
}

void TextInputBox::CheckInput(Vector2 InPos)
{
	CheckStartInput(InPos);
	CheckEndInput(InPos);
}

void TextInputBox::CheckStartInput(Vector2 InPos)
{
	if (IsPressed(InPos))
	{
		StartInput();
	}
}

void TextInputBox::CheckEndInput(Vector2 InPos)
{
	if (!IsPressed(InPos))
	{
		EndInput();
	}
}

void TextInputBox::StartInput()
{
	SDLRenderer::Get().StartTextInput();
	bStartInput = true;
}

void TextInputBox::EndInput()
{
	SDLRenderer::Get().EndTextInput();
	bStartInput = false;
}

void TextInputBox::Render()
{
	SquareButton::Render();

	//텍스트를 그림
	if (Text.size() > 0)
	{ 
		SDLRenderer::Get().DrawFont('EXTR', BlackColor, TextPos, Text.c_str());
	}
}