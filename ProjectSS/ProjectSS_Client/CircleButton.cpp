#include "CircleButton.h"
#include "stdafx.h"
#include "SDLRenderer.h"

bool CircleButton::IsPressed(Vector2 InPos)
{
	float LengthFromButtonToClickPos = Vector2::GetLength(InPos - GetPos());	
	return LengthFromButtonToClickPos <= Radius;
}

void CircleButton::Render()
{
	SDLRenderer::Get().DrawSqure(GetPos(), Radius, Radius);
}
