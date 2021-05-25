#pragma once
#include "SquareButton.h"
#include <string>
#include "SDLRenderer.h"

class TextInputBox : public SquareButton
{
public:
	TextInputBox(Vector2 InPos, int InWidth, int InHeight, ETextSize InTextSize, int InTextLimit = 0)
	: SquareButton(InPos, InWidth, InHeight)
	{
		Text = "";
		bStartInput = false;
		TextPos = Vector2(InPos.PosX - InWidth * 0.5f, InPos.PosY - InHeight * 0.5f);
		TextSize = InTextSize;
		TextLimit = InTextLimit;

		bDisplayInputPosLine = true;
		InputPosLineDisplayTime = 0.f;
	}

	virtual ~TextInputBox() {}

	bool IsStartInput() const { return bStartInput; }
	void ProcessInput(SDL_Event* InEvent);

	void CheckInput(Vector2 InPos);
	void CheckStartInput(Vector2 InPos);
	void CheckEndInput(Vector2 InPos);

	void StartInput();
	void EndInput();

	virtual void Render() override;

	void RenderText();
	void RenderInputPosLine();
	void SetDisplayInputPosLine(bool bInDisplayInputPosLine) { bDisplayInputPosLine = bInDisplayInputPosLine; }
	
	void ResetDisplayInputPosLine();

	std::string GetText() {return Text;}

private:
	std::string Text;
	bool bStartInput;

	Vector2 TextPos;
	int TextLimit; //0이면 한계 없음

	ETextSize TextSize;
	SDL_Color BlackColor = { 0,0,0,255 };

	float InputPosLineDisplayDelay = 0.5f;
	bool bDisplayInputPosLine;

	float InputPosLineDisplayTime;
};