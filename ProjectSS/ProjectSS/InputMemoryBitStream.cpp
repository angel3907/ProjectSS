#include "stdafx.h"

void InputMemoryBitStream::ReadBits(uint8_t& OutData, uint32_t InBitCount)
{
	//���� ó������ �о��
	uint32_t ByteOffset = mBitHead >> 3;
	uint32_t BitOffset = mBitHead & 0x7;

	OutData = static_cast<uint8_t>(mBuffer[ByteOffset]) >> BitOffset; //�̹� ���� �о�� �κ��� ��Ʈ ����ŭ �о�, �о������ ���� �ִ� �κи� ������

	uint32_t BitsFreeThisByte = 8 - BitOffset;
	if (BitsFreeThisByte < InBitCount)
	{
		//���� ����Ʈ���� �� �о�;� ��.
		OutData |= static_cast<uint8_t>(mBuffer[ByteOffset + 1]) << BitsFreeThisByte; //�Ʊ� �о�°ſ� �̾ ���ʿ� ���� ���
	}

	//���ϴ� ��Ʈ ������ŭ�� �о�� �� �ֵ��� ����ŷ
	OutData &= (~ (0x00ff << InBitCount)); //�ش� ��Ʈ������ŭ�� 1�� ����, �׻��¿��� &�ϸ� �� ��Ʈ�鸸 ����.

	//�о�� ��ġ ����.
	mBitHead += InBitCount;
}

void InputMemoryBitStream::ReadBits(void* OutData, uint32_t InBitCount)
{
	uint8_t* DestByte = reinterpret_cast<uint8_t*>(OutData);

	//��� ����Ʈ�� ����
	while (InBitCount > 8)
	{
		ReadBits(*DestByte, 8);
		++DestByte;
		InBitCount -= 8;
	}
	
	//���� ��Ʈ�� ����
	if (InBitCount > 0)
	{
		ReadBits(*DestByte, InBitCount);
	}
}