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
			ResetDisplayInputPosLine();
		}
		//입력
		else if (InEvent->type == SDL_TEXTINPUT)
		{
			if (Text.size() < TextLimit)
			{
				Text += InEvent->text.text;
				ResetDisplayInputPosLine();
			}
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

	bDisplayInputPosLine = true;
	InputPosLineDisplayTime = TimeUtil::Get().GetTimef();
}

void TextInputBox::EndInput()
{
	SDLRenderer::Get().EndTextInput();
	bStartInput = false;

	bDisplayInputPosLine = false;
	InputPosLineDisplayTime = 0.f;
}

void TextInputBox::Render()
{
	SquareButton::Render();
	RenderText();
	RenderInputPosLine();
}

void TextInputBox::RenderText()
{
	if (Text.size() > 0)
	{
		SDLRenderer::Get().DrawFont(TextSize, BlackColor, TextPos, Text.c_str());
	}
}

void TextInputBox::RenderInputPosLine()
{
	if (!bStartInput)
		return;

	float CurrentTime = TimeUtil::Get().GetTimef();

	if (CurrentTime > InputPosLineDisplayTime + InputPosLineDisplayDelay)
	{
		InputPosLineDisplayTime = CurrentTime;
		bDisplayInputPosLine ^= 1;
	}

	if (bDisplayInputPosLine)
	{ 
		int TextSizeInt = SDLRenderer::Get().GetTextSize(TextSize);
		Vector2 XOffsetVector = Vector2(TextSizeInt * Text.size() * 0.5f, 0);
		SDLRenderer::Get().DrawLine(TextPos + XOffsetVector, TextPos + XOffsetVector + Vector2(0, TextSizeInt));
	}
}

void TextInputBox::ResetDisplayInputPosLine()
{
	bDisplayInputPosLine = true;
	InputPosLineDisplayTime = TimeUtil::Get().GetTimef();
}
