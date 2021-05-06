#pragma once
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

class Serializable
{
	virtual void Write(OutputMemoryBitStream& InStream) const = 0;
	virtual void Read(InputMemoryBitStream& InStream) = 0;

	//��ü ���� �κ� ���ø����̼��� ����
	virtual void WriteChanged(OutputMemoryBitStream& InStream, uint32_t InProperties) const{};
	virtual void ReadChanged(InputMemoryBitStream& InStream){};
};