#pragma once
#include "GameObject.h"
#include "InputMemoryBitStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "OutputMemoryStream.h"

enum PlayerProperties : uint32_t
{
	//GameObject 다음 값으로 설정해줌
	PLR_StarCount = 1 << 1,
	PLR_TestValue = 1 << 2,
	PLR_Name = 1 << 3,
	PLR_MAX
};

class Player : public GameObject
{
public:
	Player(int32_t InitialStarCount = 0, bool TestValue = 0, string Name = "") : StarCount(InitialStarCount), TestValue(TestValue), Name(Name){}
	~Player(){}

	CLASS_IDENTIFICATION('PLYR', Player);

	void NaivelySendPlayer(int InSocket, const Player* InPlayer);
	void NaivelyReceivePlayer(int InSocket, Player* OutPlayer);

	void Write(OutputMemoryStream& InStream) const;
	void Read(InputMemoryStream& InStream);

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const override;
	virtual void ReadChanged(InputMemoryBitStream& InStream) override;

	int32_t GetStarCount() const {return StarCount;}
	bool GetTestValue() const {return TestValue;}
	string GetName() const {return Name;}

private:
	int32_t StarCount;
	bool TestValue = false;
	string Name = "";
};