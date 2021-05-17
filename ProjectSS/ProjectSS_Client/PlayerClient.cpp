#include "stdafx.h"
#include "PlayerClient.h"
#include "SDLRenderer.h"

void PlayerClient::Render()
{
	if (bAttackEffectOn)
	{
		SDLRenderer::Get().DrawTexture('PLAE', Pos);
	}

	SDLRenderer::Get().DrawTexture('PLYR', Pos);
}