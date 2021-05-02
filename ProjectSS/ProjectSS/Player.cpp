#include "stdafx.h"
#include "Player.h"

void Player::NaivelySendPlayer(int InSocket, const Player* InPlayer)
{
	send(InSocket, reinterpret_cast<const char*>(InPlayer), sizeof(*InPlayer) , 0);
}

void Player::NaivelyReceivePlayer(int InSocket, Player* OutPlayer)
{
	recv(InSocket, reinterpret_cast<char*>(OutPlayer), sizeof(*OutPlayer), 0);
}