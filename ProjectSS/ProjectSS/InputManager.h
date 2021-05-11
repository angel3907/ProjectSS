#pragma once
#include "MoveList.h"

class InputManager
{
public:
	static InputManager& Get()
	{
		static InputManager sInstance;
		return sInstance;
	}

	MoveList& GetMoveList() { return mMoveList; }

private:
	InputManager(){}
	virtual ~InputManager(){}

	MoveList mMoveList;
};