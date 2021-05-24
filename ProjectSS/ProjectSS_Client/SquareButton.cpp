#include "SquareButton.h"
#include "SDLRenderer.h"

bool SquareButton::IsPressed(Vector2 InPos)
{
	return (MinX <= InPos.PosX && InPos.PosX <= MaxX &&
			MinY <= InPos.PosY && InPos.PosY <= MaxY);
}

void SquareButton::Render()
{
	//TODO 버튼 위치 보기 위한 임시 코드
	SDLRenderer::Get().DrawSqure(GetPos(), Width, Height);
}
