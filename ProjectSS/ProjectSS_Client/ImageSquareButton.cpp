#include "ImageSquareButton.h"

void ImageSquareButton::Render()
{
	SDLRenderer::Get().DrawStretchedTexture(ImageKey, GetPos(), GetWidthHeight());
}