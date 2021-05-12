#include "stdafx.h"

void InputState::Write(OutputMemoryBitStream& InStream) const
{
	InStream.Write(GetDesiredHorizontalDelta());
	InStream.Write(GetDesiredVerticallDelta());
	InStream.Write(mIsAttacking);
}

void InputState::Read(InputMemoryBitStream& InStream)
{
	InStream.Read(mDesiredRightAmount);
	InStream.Read(mDesiredForwardAmount);
	InStream.Read(mIsAttacking);
}