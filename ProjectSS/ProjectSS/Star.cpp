#include "stdafx.h"

void StarStatus::Write(OutputMemoryBitStream& InStream) const
{
	InStream.Write(bHidden);
	InStream.Write(Value);
}

void StarStatus::Read(InputMemoryBitStream& InStream)
{
	InStream.Read(bHidden);
	InStream.Read(Value);
}

DataType* StarStatus::sDataType = nullptr;

void Star::Write(OutputMemoryBitStream& InStream) const
{
	GameObject::Write(InStream);
	mStarStatus.Write(InStream);
}

void Star::Read(InputMemoryBitStream& InStream)
{
	GameObject::Read(InStream);
	mStarStatus.Read(InStream);
}