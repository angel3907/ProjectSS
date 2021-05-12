#include "stdafx.h"

void Move::Write(OutputMemoryBitStream& InStream) const
{
	mInputState.Write(InStream);
	InStream.Write(mTimeStamp);
	//InStream.Write(mDeltaTime);
}

void Move::Read(InputMemoryBitStream& InStream)
{
	mInputState.Read(InStream);
	InStream.Read(mTimeStamp);
	//InStream.Read(mDeltaTime);
}

