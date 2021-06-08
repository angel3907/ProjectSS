#include "stdafx.h"
#include "AckRange.h"

bool AckRange::ExtendIfShould(PacketSequenceNumber InSequenceNumber)
{
	//다음 번호로 붙을 수 있는 경우
	if (InSequenceNumber == mStart + mCount)
	{
		++mCount;
		return true;
	}
	else
	{
		return false;
	}
}

void AckRange::Write(OutputMemoryBitStream& InPacket) const
{
	//시작값 / 1개 이상인지 / 실제 개수
	InPacket.Write(mStart);
	bool HasCount = mCount > 1;
	InPacket.Write(HasCount);

	if (HasCount)
	{
		//확인 범위를 최대 8비트로 표현가능한 갓으로 한다고 침.
		uint32_t CountMinusOne = mCount - 1;
		uint8_t CountToAck = CountMinusOne > 255?
			255 : static_cast<uint8_t>(CountMinusOne);
		InPacket.Write(CountToAck);
	}
}

void AckRange::Read(InputMemoryBitStream& InPacket)
{
	InPacket.Read(mStart);
	bool HasCount;
	InPacket.Read(HasCount);
	if (HasCount)
	{
		uint8_t CountMinusOne;
		InPacket.Read(CountMinusOne);

		mCount = CountMinusOne + 1;
	}
	else
	{
		//디폴트
		mCount = 1;
	}
}