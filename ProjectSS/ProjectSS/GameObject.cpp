#include "stdafx.h"

void GameObject::Write(OutputMemoryBitStream& InStream) const
{
	InStream.WritePosF(Pos);
}

void GameObject::Read(InputMemoryBitStream& InStream)
{
	InStream.ReadPosF(Pos);
}