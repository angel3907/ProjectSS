#pragma once
#include "InputMemoryBitStream.h"
#include "OutputMemoryBitStream.h"

class Serializable
{
	virtual void Write(OutputMemoryBitStream& InStream) const = 0;
	virtual void Read(InputMemoryBitStream& InStream) = 0;

	//객체 상태 부분 리플리케이션을 위함
	virtual void WriteChanged(OutputMemoryBitStream& InStream) const{};
	virtual void ReadChanged(InputMemoryBitStream& InStream){};

	//부분 리플리케이션 관련 속성값
public:
	uint32_t mProperties = 0;

	void SetProperties(uint32_t InProperties){mProperties = InProperties;}
	void ResetProperties() { mProperties = 0; }

	void AddProperties(uint32_t InProperties){mProperties |= InProperties;}
	void RemoveProperties(uint32_t InProperties){mProperties &= InProperties;}
};