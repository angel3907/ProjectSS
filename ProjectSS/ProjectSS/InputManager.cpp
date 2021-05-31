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
		//눌렀으면 1, 아니면 0
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
	//입력한 Keycode에 따라서 입력상태를 처리
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
	//샘플링할 시간 주기마다 샘플링
	if (IsTimeToSampleInput())
	{
		mPendingMove = &SampleInputAsMove();
	}
}

bool InputManager::IsTimeToSampleInput()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//샘플링할 시간이 지났다면
	if (time > mNextTimeToSampleInput)
	{
		//다음에 샘플링할 시간 갱신
		mNextTimeToSampleInput = mNextTimeToSampleInput + mTimeBetweenInputSamples;
		return true;
	}

	return false;
}	

const Move& InputManager::SampleInputAsMove()
{
	return mMoveList.AddMove(GetState(), TimeUtil::Get().GetFrameStartTime());
}
