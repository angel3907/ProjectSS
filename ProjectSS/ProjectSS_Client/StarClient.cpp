#include "stdafx.h"
#include "StarClient.h"
#include "SDLRenderer.h"

void StarClient::Render()
{
	if (GetStarStatus().bHidden == false)
	{ 
		if (GetStarStatus().Value == 1)
		{
			SDLRenderer::Get().DrawTexture('STAS', GetPos());
		}
		else if (GetStarStatus().Value == 3)
		{
			SDLRenderer::Get().DrawTexture('STAL', GetPos());
		}
	}
}
