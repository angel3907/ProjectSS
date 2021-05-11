#include "stdafx.h"

void InputState::Write(OutputMemoryBitStream& InStream) const
{
	InStream.Write(mDesiredRightAmount);
	InStream.Write(mDesiredLeftAmount);
	InStream.Write(mDesiredForwardAmount);
	InStream.Write(mDesiredBackAmount);
	InStream.Write(mIsAttacking);
}

void InputState::Read(InputMemoryBitStream& InStream)
{
	InStream.Read(mDesiredRightAmount);
	InStream.Read(mDesiredLeftAmount);
	InStream.Read(mDesiredForwardAmount);
	InStream.Read(mDesiredBackAmount);
	InStream.Read(mIsAttacking);
}
