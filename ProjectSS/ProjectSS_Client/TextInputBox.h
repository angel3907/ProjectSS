#pragma once
#include "SquareButton.h"
#include <string>
#include "SDLRenderer.h"

class TextInputBox : public SquareButton
{
public:
	TextInputBox(Vector2 InPos, int InWidth, int InHeight, int InTextLimit = 0)
	: SquareButton(InPos, InWidth, InHeight)
	{
		Text = "";
		bStartInput = false;
		TextPos = Vector2(InPos.PosX - InWidth * 0.5f, InPos.PosY - InHeight * 0.5f);
		TextLimit = InTextLimit;
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

private:
	std::string Text;
	bool bStartInput;

	Vector2 TextPos;
	int TextLimit; //0이면 한계 없음

	SDL_Color BlackColor = { 0,0,0,255 };
};