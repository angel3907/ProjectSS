#pragma once
#include "InputState.h"
#include "Serializable.h"

class Move : public Serializable
{
public:
	Move(){}
	Move(const InputState& InInputState, float InTimeStamp, float InDeltaTime):
	mInputState(InInputState),
	mTimeStamp(InTimeStamp),
	mDeltaTime(InDeltaTime)
	{}

	const InputState& GetInputState() const { return mInputState; }
	float GetTimeStamp() const {return mTimeStamp;}
	float GetDeltaTime() const { return mDeltaTime; }

	//��������� ��Ʈ���� �а� ��.
	void Write(OutputMemoryBitStream& InStream) const override;
	void Read(InputMemoryBitStream& InStream) override;

private:
	InputState mInputState;
	float mTimeStamp;
	float mDeltaTime;
};