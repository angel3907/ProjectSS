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

	//������ ������
	float GetDesiredHorizontalDelta() const
	{
		return mDesiredRightAmount - mDesiredLeftAmount;
	}

	//������ ������
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

	//��������� �� �а� ����.
	void Write(OutputMemoryBitStream& InStream) const override;
	void Read(InputMemoryBitStream& InStream) override;

private:
	friend class InputManager;
	
	//�����¿� ������, ���� ����
	float mDesiredRightAmount, mDesiredLeftAmount;
	float mDesiredForwardAmount, mDesiredBackAmount;
	bool mIsAttacking;
};