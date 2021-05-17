#pragma once
#include "GameObject.h"
#include "InputMemoryBitStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "OutputMemoryStream.h"
#include <memory>
#include "InputState.h"

enum PlayerProperties : uint32_t
{
	//GameObject ���� ������ ��������
	PLR_StarCount = 1 << 1,
	PLR_TestValue = 1 << 2,
	PLR_Name = 1 << 3,
	PLR_MAX
};

class Player : public GameObject
{
public:
	Player(int32_t InitialStarCount = 0, bool TestValue = 0, std::string Name = "") : StarCount(InitialStarCount), TestValue(TestValue), Name(Name){}
	~Player(){}

	//CLASS_IDENTIFICATION('PLYR', Player);

	virtual void Update() override {}

	void ProcessInput(float InDeltaTime, const InputState& InCurrentState);
	void SimulateMovement(float InDeltaTime);

	bool ApplyPosLimit(Vector2& InOutPos);

	void Write(OutputMemoryStream& InStream) const;
	void Read(InputMemoryStream& InStream);

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const override;
	virtual void ReadChanged(InputMemoryBitStream& InStream) override;

	int32_t GetStarCount() const {return StarCount;}
	bool GetTestValue() const {return TestValue;}
	std::string GetName() const {return Name;}

	uint32_t GetPlayerId() const {return mPlayerId;}
	void SetPlayerId(uint32_t InPlayerId) { mPlayerId = InPlayerId;}

	float GetPlayerRadius() const {return PlayerRadius;}

	bool IsAttacked(){return bAttacked;}

private:
	int32_t StarCount;
	bool TestValue = false;
	std::string Name = "";
	uint32_t mPlayerId = 0;

protected:
	bool mIsAttacking = false;
	bool bAttacked = false; //���ݴ��ϴ� ������
	bool bAttackEffectOn = false; //���� ����Ʈ�� ��������

public:
	const float Speed = 50.0f;
	const float PlayerRadius = 50.0f;
	const float AttackRadius = 100.0f;

	//���� ����Ʈ �����ð�
	const float AttackEffectDuration = 2.5f;
};

using PlayerPtr = std::shared_ptr<Player>;