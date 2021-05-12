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

	//이동값을 추가해줌
	const Move& AddMove(const InputState& InInputState, float InTimeStamp);
	bool AddMove(const Move& InMove);

	//처리할 이동값이 있는지 봄
	bool HasMoves() const {return !mMoves.empty(); }
	int32_t GetMoveCount() const {return mMoves.size(); }

	void Clear() {mMoves.clear();}
	const Move& GetLatestMove() const {return mMoves.back();}

	//괄호로 이동값을 참조할 수 있게 함
	const Move& operator[](size_t index) { return mMoves[index]; }
	
	deque<Move>::const_iterator begin() const {return mMoves.begin();}
	deque<Move>::const_iterator end () const {return mMoves.end(); }


private:
	float mLastMoveTimeStamp;
	std::deque<Move> mMoves;
};
