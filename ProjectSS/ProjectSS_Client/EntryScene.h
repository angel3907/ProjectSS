#pragma once
#include "Scene.h"

enum PlayerColor
{
	RED = 0,
	YELLOW = 1,
	GREEN = 2,
	SKYBLUE = 3,
	DARKBLUE = 4,
	PURPLE = 5
};


class EntryScene : public Scene
{
public:
	static EntryScene& Get()
	{
		static EntryScene sInstance;
		return sInstance;
	}

	virtual void Update() override;
	virtual void Render() override;

private:
	EntryScene();
	virtual ~EntryScene();

	PlayerColor SelectedPlayerColor = RED;

	Vector2 CheckMarkPos;
	Vector2 CheckMarkOffsetX;
	Vector2 PlayerColorPos;
};