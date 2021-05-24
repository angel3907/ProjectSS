#include "SquareButton.h"
#include "SDLRenderer.h"

bool SquareButton::IsPressed(Vector2 InPos)
{
	return (MinX <= InPos.PosX && InPos.PosX <= MaxX &&
			MinY <= InPos.PosY && InPos.PosY <= MaxY);
}

void SquareButton::Render()
{
	//TODO ��ư ��ġ ���� ���� �ӽ� �ڵ�
	SDLRenderer::Get().DrawSqure(GetPos(), Width, Height);
}
