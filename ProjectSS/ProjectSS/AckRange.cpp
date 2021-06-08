#include "stdafx.h"
#include "AckRange.h"

bool AckRange::ExtendIfShould(PacketSequenceNumber InSequenceNumber)
{
	//���� ��ȣ�� ���� �� �ִ� ���
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
	//���۰� / 1�� �̻����� / ���� ����
	InPacket.Write(mStart);
	bool HasCount = mCount > 1;
	InPacket.Write(HasCount);

	if (HasCount)
	{
		//Ȯ�� ������ �ִ� 8��Ʈ�� ǥ�������� ������ �Ѵٰ� ħ.
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
		//����Ʈ
		mCount = 1;
	}
}