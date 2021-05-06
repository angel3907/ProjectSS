#pragma once
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

class Serializable
{
	virtual void Write(OutputMemoryBitStream& InStream) const = 0;
	virtual void Read(InputMemoryBitStream& InStream) = 0;

	//객체 상태 부분 리플리케이션을 위함
	virtual void WriteChanged(OutputMemoryBitStream& InStream, uint32_t InProperties) const{};
	virtual void ReadChanged(InputMemoryBitStream& InStream){};
};