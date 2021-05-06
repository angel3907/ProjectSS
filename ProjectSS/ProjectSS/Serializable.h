#pragma once
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

class Serializable
{
	virtual void Write(OutputMemoryBitStream& InStream) const = 0;
	virtual void Read(InputMemoryBitStream& InStream) = 0;
};