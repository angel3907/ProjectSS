#pragma once
#include "MathUtils.h"

class GameObject
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos){}
	virtual ~GameObject(){}

protected:
	Vector2 Pos;

public:
	Vector2 GetPos() {return Pos;}
	void SetPos(Vector2 NewPos){Pos = NewPos;}
};