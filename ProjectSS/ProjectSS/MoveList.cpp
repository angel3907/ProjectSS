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

void MoveList::Write(OutputMemoryBitStream& InStream) const
{
	//TODO �ۼ�
}

void MoveList::Read(InputMemoryBitStream& InStream)
{
	//TODO �ۼ�
}
