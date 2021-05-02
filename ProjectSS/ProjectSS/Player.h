#pragma once
#include "GameObject.h"
#include <stdint.h>

class Player : public GameObject
{
public:
	Player() : StarCount(0){}
	
	Player(int32_t InitialStarCount) : StarCount(InitialStarCount){}

	void NaivelySendPlayer(int InSocket, const Player* InPlayer);
	void NaivelyReceivePlayer(int InSocket, Player* OutPlayer);

	int32_t GetStarCount() {return StarCount;}

private:
	int32_t StarCount;
};