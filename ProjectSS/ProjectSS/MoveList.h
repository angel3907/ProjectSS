#pragma once
#include "Move.h"
#include <deque>

class MoveList : public Serializable
{
public:
	MoveList(){}
	~MoveList(){}

	const Move& AddMove(const InputState& InInputState, float InTimeStamp);
	bool HasMoves() const {return !mMoves.empty(); }
	int32_t GetMoveCount() const {return mMoves.size(); }

	void Write(OutputMemoryBitStream& InStream) const override;
	void Read(InputMemoryBitStream& InStream) override;
	
	void Clear() {mMoves.clear();}

	const Move& operator[](size_t index) { return mMoves[index]; }
	

private:
	std::deque<Move> mMoves;
	float mLastMoveTimeStamp = 0.f;
};
