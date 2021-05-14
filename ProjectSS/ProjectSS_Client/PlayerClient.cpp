#include "stdafx.h"
#include "PlayerClient.h"
#include "SDLRenderer.h"

void PlayerClient::Render()
{
	SDLRenderer::Get().DrawTexture('PLYR', Pos);
}