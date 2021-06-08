#pragma once
#include <stdint.h>
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "InFlightPacket.h"

class AckRange
{
public:
	AckRange() : mStart(0), mCount(0) {}
	AckRange(PacketSequenceNumber InStart) : mStart(InStart), mCount(1){}

	~AckRange(){}

	inline bool ExtendIfShould(PacketSequenceNumber InSequenceNumber);
	
	PacketSequenceNumber GetStart() const {return mStart;}
	uint32_t GetCount() const {return mCount;}
	
	void Write(OutputMemoryBitStream& InPacket) const;
	void Read(InputMemoryBitStream& InPacket);

private:
	PacketSequenceNumber mStart;
	uint32_t mCount;
};