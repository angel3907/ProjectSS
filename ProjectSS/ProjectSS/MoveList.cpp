#include "stdafx.h"

const Move& MoveList::AddMove(const InputState& InInputState, float InTimeStamp)
{
	//ù��° ���� �ð� ������ 0���� ��.
	float DeltaTime = mLastMoveTimeStamp >= 0.0f ? InTimeStamp - mLastMoveTimeStamp : 0.0f;
	mMoves.emplace_back(InInputState, InTimeStamp, DeltaTime);
	
	//������ ���� Ÿ�ӽ����� ���
	mLastMoveTimeStamp = InTimeStamp;
	return mMoves.back();
}

bool MoveList::AddMove(const Move& InMove)
{
	//�ش� Move������ Ÿ�ӽ������� ������ Delta�� ���
	float TimeStamp = InMove.GetTimeStamp();
	float DeltaTime = mLastMoveTimeStamp >= 0.f? TimeStamp - mLastMoveTimeStamp : 0.f;
	mLastMoveTimeStamp = TimeStamp;
	mMoves.emplace_back(InMove.GetInputState(), TimeStamp, DeltaTime);

	return true;
}