#pragma once
#include "Button.h"

class SquareButton : public Button
{
public:
	SquareButton(Vector2 InPos, int InWidth, int InHeight)
	: Button(InPos), Width(InWidth), Height(InHeight)
	{
		MinX = GetPos().PosX - Width * 0.5f;
		MinY = GetPos().PosY - Height * 0.5f;
		MaxX = GetPos().PosX + Width * 0.5f;
		MaxY = GetPos().PosY + Height * 0.5f;
	}

	~SquareButton(){}

	bool IsPressed(Vector2 InPos) override;
	void Render() override;

private:
	int Width;
	int Height;
	
	float MinX, MinY, MaxX, MaxY;
};