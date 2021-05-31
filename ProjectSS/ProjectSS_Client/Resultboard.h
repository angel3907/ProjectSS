#pragma once
#include "SquareButton.h"
#include "SDLRenderer.h"

class Resultboard
{
public:
	Resultboard();
	~Resultboard();

	void Render();
	bool IsQuitButtonPressed(Vector2 InPos){return QuitButton->IsPressed(InPos);}
	
private:
	uint32_t ResultboardTextureKey = 'RESU';
	SquareButton* QuitButton;

	SDL_Color BlackColor{0, 0, 0, 255};

	const int DisplayPlayerScoreNum = 3;
};