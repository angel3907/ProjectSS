#include "TextBox.h"
#include "SDLRenderer.h"

void TextBox::Render()
{
	if (Text.size() > 0)
		SDLRenderer::Get().DrawFont(TextSize, BlackColor, TextPos, Text.c_str());
}