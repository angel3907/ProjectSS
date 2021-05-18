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
	//GameObject 다음 값으로 설정해줌
	PLR_StarCount = 1 << 1,
	PLR_Name = 1 << 2,
	PLR_MAX
};

class Player : public GameObject
{
public:
	Player(int32_t InitialStarCount = 0, std::string Name = "") : StarCount(InitialStarCount), Name(Name){}
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
	std::string GetName() const {return Name;}

	uint32_t GetPlayerId() const {return mPlayerId;}
	void SetPlayerId(uint32_t InPlayerId) { mPlayerId = InPlayerId;}

	float GetPlayerRadius() const {return PlayerRadius;}

	bool IsAttacked(){return bAttacked;}

	void AddStarCount(int InStarValue) { StarCount += InStarValue; }

	void SetName(const std::string& InName){Name = InName;}

private:
	int32_t StarCount;
	std::string Name = "";
	uint32_t mPlayerId = 0;

protected:
	bool mIsAttacking = false;
	bool bAttacked = false; //공격당하는 중인지
	bool bAttackEffectOn = false; //공격 이펙트가 켜졌는지

public:
	const float Speed = 150.0f;
	const float PlayerRadius = 50.0f;
	const float AttackRadius = 100.0f;

	//공격 이펙트 유지시간
	const float AttackEffectDuration = 2.5f;
};

using PlayerPtr = std::shared_ptr<Player>;