#pragma once
#include "GameObject.h"
#include "InputMemoryBitStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "OutputMemoryStream.h"

class Player : public GameObject
{
public:
	Player() : StarCount(0){}
	
	Player(int32_t InitialStarCount) : StarCount(InitialStarCount){}

	Player(int32_t InitialStarCount, bool TestValue) : StarCount(InitialStarCount), TestValue(TestValue){}

	void NaivelySendPlayer(int InSocket, const Player* InPlayer);
	void NaivelyReceivePlayer(int InSocket, Player* OutPlayer);

	void Write(OutputMemoryStream& InStream) const;
	void Read(InputMemoryStream& InStream);

	void Write(OutputMemoryBitStream& InStream) const;
	void Read(InputMemoryBitStream& InStream);

	int32_t GetStarCount() {return StarCount;}
	bool GetTestValue() {return TestValue;}

private:
	int32_t StarCount;
	bool TestValue = false;
};