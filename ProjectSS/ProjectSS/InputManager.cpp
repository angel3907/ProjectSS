#include "stdafx.h"
#include "InputManager.h"

namespace
{
	inline void UpdateDesiredVariableFromKey(EInputAction InInputAction, bool& IoVariable)
	{
		if (InInputAction == EIA_Pressed)
		{
			IoVariable = true;
		}
		else if (InInputAction == EIA_Released)
		{
			IoVariable = false;
		}
	}

	inline void UpdateDesiredFloatFromKey(EInputAction InInputAction, float& IoVariable)
	{
		//�������� 1, �ƴϸ� 0
		if (InInputAction == EIA_Pressed)
		{
			IoVariable = 1.0f;
			printf("You Pressed Key");
		}
		else if (InInputAction == EIA_Released)
		{
			IoVariable = 0.f;
			printf("You Released Key");
		}
	}
}

InputManager::InputManager()
	:mNextTimeToSampleInput(0.f),
	 mPendingMove(nullptr)
{

}


void InputManager::HandleInput(EInputAction InInputAction, int InKeyCode)
{
	//�Է��� Keycode�� ���� �Է»��¸� ó��
	switch(InKeyCode)
	{
	case 'a':
		UpdateDesiredFloatFromKey(InInputAction, mCurrentState.mDesiredLeftAmount);
		break;
	case 'd':
		UpdateDesiredFloatFromKey(InInputAction, mCurrentState.mDesiredRightAmount);
		break;
	case 'w':
		UpdateDesiredFloatFromKey(InInputAction, mCurrentState.mDesiredForwardAmount);
		break;
	case 's':
		UpdateDesiredFloatFromKey(InInputAction, mCurrentState.mDesiredBackAmount);
		break;
	case 'k':
		UpdateDesiredVariableFromKey(InInputAction, mCurrentState.mIsAttacking);
		break;
	}
}

void InputManager::Update()
{
	//���ø��� �ð� �ֱ⸶�� ���ø�
	if (IsTimeToSampleInput())
	{
		mPendingMove = &SampleInputAsMove();
	}
}

bool InputManager::IsTimeToSampleInput()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//���ø��� �ð��� �����ٸ�
	if (time > mNextTimeToSampleInput)
	{
		//������ ���ø��� �ð� ����
		mNextTimeToSampleInput = mNextTimeToSampleInput + mTimeBetweenInputSamples;
		return true;
	}

	return false;
}	

const Move& InputManager::SampleInputAsMove()
{
	return mMoveList.AddMove(GetState(), TimeUtil::Get().GetFrameStartTime());
}
