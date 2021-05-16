#pragma once
#include "Move.h"
#include <deque>

class MoveList
{
public:
	MoveList():
	mLastMoveTimeStamp(-1.f)
	{}
	~MoveList(){}

	//�̵����� �߰�����
	const Move& AddMove(const InputState& InInputState, float InTimeStamp);
	bool AddMove(const Move& InMove);

	//ó���� �̵����� �ִ��� ��
	bool HasMoves() const {return !mMoves.empty(); }
	size_t GetMoveCount() const {return mMoves.size(); }

	void Clear() {mMoves.clear();}
	const Move& GetLatestMove() const {return mMoves.back();}

	//��ȣ�� �̵����� ������ �� �ְ� ��
	const Move& operator[](size_t index) { return mMoves[index]; }
	
	std::deque<Move>::const_iterator begin() const {return mMoves.begin();}
	std::deque<Move>::const_iterator end () const {return mMoves.end(); }


private:
	float mLastMoveTimeStamp;
	std::deque<Move> mMoves;
};
