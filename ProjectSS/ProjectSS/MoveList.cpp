#include "stdafx.h"

const Move& MoveList::AddMove(const InputState& InInputState, float InTimeStamp)
{
	//첫번째 것은 시간 변위를 0으로 함.
	float DeltaTime = mLastMoveTimeStamp >= 0.0f ? InTimeStamp - mLastMoveTimeStamp : 0.0f;
	mMoves.emplace_back(InInputState, InTimeStamp, DeltaTime);
	
	//마지막 조작 타임스탬프 기록
	mLastMoveTimeStamp = InTimeStamp;
	return mMoves.back();
}

bool MoveList::AddMove(const Move& InMove)
{
	//해당 Move값에서 타임스탬프를 가져와 Delta를 계산
	float TimeStamp = InMove.GetTimeStamp();
	float DeltaTime = mLastMoveTimeStamp >= 0.f? TimeStamp - mLastMoveTimeStamp : 0.f;
	mLastMoveTimeStamp = TimeStamp;
	mMoves.emplace_back(InMove.GetInputState(), TimeStamp, DeltaTime);

	return true;
}