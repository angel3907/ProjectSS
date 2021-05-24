#pragma once
#include "MathUtils.h"

class Button
{
public:
	Button(Vector2 InPos) : Pos(InPos) {};
	virtual ~Button(){}
	virtual bool IsPressed(Vector2 InPos) = 0;
	const Vector2& GetPos() { return Pos; }

	virtual void Render() = 0;

private:
	Vector2 Pos;
};