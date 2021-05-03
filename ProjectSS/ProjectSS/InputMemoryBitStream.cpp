#include "stdafx.h"

void InputMemoryBitStream::ReadBits(uint8_t& OutData, uint32_t InBitCount)
{
	//버퍼 처음부터 읽어옴
	uint32_t ByteOffset = mBitHead >> 3;
	uint32_t BitOffset = mBitHead & 0x7;

	OutData = static_cast<uint8_t>(mBuffer[ByteOffset]) >> BitOffset; //이미 값을 읽어온 부분의 비트 수만큼 밀어, 읽어오려는 값이 있는 부분만 가져옴

	uint32_t BitsFreeThisByte = 8 - BitOffset;
	if (BitsFreeThisByte < InBitCount)
	{
		//다음 바이트에서 더 읽어와야 함.
		OutData |= static_cast<uint8_t>(mBuffer[ByteOffset + 1]) << BitsFreeThisByte; //아까 읽어온거에 이어서 앞쪽에 값을 기록
	}

	//원하는 비트 개수만큼만 읽어올 수 있도록 마스킹
	OutData &= (~ (0x00ff << InBitCount)); //해당 비트개수만큼만 1로 차며, 그상태에서 &하면 그 비트들만 남음.

	//읽어온 위치 갱신.
	mBitHead += InBitCount;
}

void InputMemoryBitStream::ReadBits(void* OutData, uint32_t InBitCount)
{
	uint8_t* DestByte = reinterpret_cast<uint8_t*>(OutData);

	//모든 바이트를 쓴다
	while (InBitCount > 8)
	{
		ReadBits(*DestByte, 8);
		++DestByte;
		InBitCount -= 8;
	}
	
	//남은 비트를 쓴다
	if (InBitCount > 0)
	{
		ReadBits(*DestByte, InBitCount);
	}
}