#pragma once
#include "Player.h"

class PlayerServer : public Player
{
public:
	PlayerServer(int32_t InitialStarCount = 0, bool TestValue = 0, string Name = "")
	: Player(InitialStarCount, TestValue, Name)
	{}
	~PlayerServer(){}

	virtual void Update() override;

	void ProcessInput(float InDeltaTime, const InputState& InCurrentState);
	void SimulateMovement(float InDeltaTime);

private:

};