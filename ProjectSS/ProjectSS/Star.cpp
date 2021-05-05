#include "stdafx.h"

DataType* StarStatus::sDataType = nullptr;

void Star::Write(OutputMemoryBitStream& InStream) const
{
	GameObject::Write(InStream);
	InStream.Write(mStarStatus.bHidden);
	InStream.Write(mStarStatus.Value);
}

void Star::Read(InputMemoryBitStream& InStream)
{
	GameObject::Read(InStream);
	InStream.Read(mStarStatus.bHidden);
	InStream.Read(mStarStatus.Value);
}
