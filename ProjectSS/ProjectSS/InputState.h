#pragma once
#include "Serializable.h"
class InputManager;

class InputState : public Serializable
{
public:
	InputState():
		mDesiredRightAmount(0),
		mDesiredLeftAmount(0),
		mDesiredForwardAmount(0),
		mDesiredBackAmount(0),
		mIsAttacking(false)
		{}

	//수평축 오프셋
	float GetDesiredHorizontalDelta() const
	{
		return mDesiredRightAmount - mDesiredLeftAmount;
	}

	//수직축 오프셋
	float GetDesiredVerticallDelta() const
	{
		return mDesiredForwardAmount - mDesiredBackAmount;
	}

	bool IsAttacking() const
	{
		return mIsAttacking;
	}

	void PrintState() const
	{
		LOG("mDesiredRightAmount : %f, mDesiredLeftAmount : %f, mDesiredForwardAmount : %f, mDesiredBackAmount : %f, mIsAttacking : %d \n", 
			mDesiredRightAmount, mDesiredLeftAmount, mDesiredForwardAmount, mDesiredBackAmount, mIsAttacking);
	}

	//멤버변수에 값 읽고 쓰기.
	void Write(OutputMemoryBitStream& InStream) const override;
	void Read(InputMemoryBitStream& InStream) override;

private:
	friend class InputManager;
	
	//상하좌우 오프셋, 공격 여부
	float mDesiredRightAmount, mDesiredLeftAmount;
	float mDesiredForwardAmount, mDesiredBackAmount;
	bool mIsAttacking;
};