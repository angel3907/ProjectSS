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

void MoveList::Write(OutputMemoryBitStream& InStream) const
{
	//TODO 작성
}

void MoveList::Read(InputMemoryBitStream& InStream)
{
	//TODO 작성
}
