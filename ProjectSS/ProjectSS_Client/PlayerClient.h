#pragma once
#include "Player.h"

class PlayerClient : public Player
{
public:
	CLASS_IDENTIFICATION('PLYR', PlayerClient);

	PlayerClient(){};
	~PlayerClient() {}
};