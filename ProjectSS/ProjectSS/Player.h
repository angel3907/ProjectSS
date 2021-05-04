#pragma once
#include "GameObject.h"
#include "InputMemoryBitStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "OutputMemoryStream.h"

class Player : public GameObject
{
public:
	Player(int32_t InitialStarCount = 0, bool TestValue = 0, string Name = "") : StarCount(InitialStarCount), TestValue(TestValue), Name(Name){}

	void NaivelySendPlayer(int InSocket, const Player* InPlayer);
	void NaivelyReceivePlayer(int InSocket, Player* OutPlayer);

	void Write(OutputMemoryStream& InStream) const;
	void Read(InputMemoryStream& InStream);

	void Write(OutputMemoryBitStream& InStream) const;
	void Read(InputMemoryBitStream& InStream);

	int32_t GetStarCount() const {return StarCount;}
	bool GetTestValue() const {return TestValue;}
	string GetName() const {return Name;}

private:
	int32_t StarCount;
	bool TestValue = false;
	string Name = "";
};