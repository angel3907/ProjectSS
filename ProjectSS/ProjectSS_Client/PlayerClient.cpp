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

	SDLRenderer::Get().DrawStretchedTexture(GetPlayerColor(), Pos, Vector2(PlayerRadius * 2, PlayerRadius * 2));
	
	SDL_Color BlackColor;
	BlackColor.r = BlackColor.g = BlackColor.b = 0;

	int NameLength = static_cast<int>(GetName().size());
	Vector2 NamePlateOffsetVector(-NameLength * 7.7f, -80);
	SDLRenderer::Get().DrawFont('SMAL', BlackColor, GetPos() + NamePlateOffsetVector, GetName().c_str());

	if (IsLocallyControlled())
	{ 
		std::string S = "Star:" + std::to_string(GetStarCount());
		SDLRenderer::Get().DrawFont('LARG', BlackColor, Vector2(WORLD_WIDTH * 0.82, 20), S.c_str());
	}
}

bool PlayerClient::IsLocallyControlled()
{
	return NetworkManagerClient::sInstance->GetPlayerId() == GetPlayerId();
}