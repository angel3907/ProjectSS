#pragma once
#include "MathUtils.h"
#include "SDLRenderer.h"
#include <string>

class TextBox
{
public:
	TextBox(Vector2 InPos, int InWidth, int InHeight, ETextSize InTextSize)
	:Pos(InPos), Width(InWidth), Height(InHeight), TextSize(InTextSize)
	{
		Text = "";
		TextPos = Vector2(InPos.PosX - InWidth * 0.5f, InPos.PosY - InHeight * 0.5f);
	}

	void Render();
	void SetText(std::string InText) { Text = InText; }

private:
	Vector2 Pos;
	int Width;
	int Height;
	ETextSize TextSize;

	std::string Text;
	Vector2 TextPos;

	SDL_Color BlackColor = { 0,0,0,255 };
};