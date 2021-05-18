#include "stdafx.h"
#include "PlayerClient.h"
#include "SDLRenderer.h"
#include "NetworkManagerClient.h"

void PlayerClient::Render()
{
	if (bAttackEffectOn)
	{
		SDLRenderer::Get().DrawTexture('PLAE', Pos);
	}

	SDLRenderer::Get().DrawTexture('PLYR', Pos);

	if (IsLocallyControlled())
	{ 
		SDL_Color Color;
		Color.r = Color.g = Color.b = 0;

		std::string S = "Star : " + std::to_string(GetStarCount());
		SDLRenderer::Get().DrawFont('CARL', Color, Vector2(WORLD_WIDTH * 0.825,0), S.c_str());
	}
}

bool PlayerClient::IsLocallyControlled()
{
	return NetworkManagerClient::sInstance->GetPlayerId() == GetPlayerId();
}