#include "stdafx.h"

void OutputMemoryBitStream::WriteBits(uint8_t InData, size_t InBitCount)
{
	//비트를 스트림에 기록하는 내부 작업 수행
	//바이트 하나를 매개변수로 받아 지정된 비트수만큼 비트를 추출하여 스트림에 기록함.
	
	//버퍼 데이터 받아온 후의 Head 위치 세팅 & 용량할당
	uint32_t NextBitHead = mBitHead + static_cast<uint32_t>(InBitCount);
	if (NextBitHead > mBitCapacity)
	{
		ReallocBuffer(max(mBitCapacity * 2, NextBitHead));
	}

	//바이트 오프셋 : 비트 헤드를 8로 나눔
	//비트 오프셋 : 8에 대한 나머지
	//현재 버퍼에 데이터가 x(ByteOffset)바이트 + y(BitOffset)비트 존재
	uint32_t ByteOffset = mBitHead >> 3;
	uint32_t BitOffset = mBitHead & 0x07; //8로 나눈 나머지를 구함. 또는 시프트하다 버려지는 3비트를 구하면 됨
	
	//현재 처리 중 바이트에서 몇 비트를 남길지 계산
	uint8_t CurrentMask = ~(0xff << BitOffset);
	mBuffer[ByteOffset] = (mBuffer[ByteOffset] & CurrentMask) | (InData << BitOffset);
	
	//기록할 바이트에 몇 비트가 아직 남아있나 계산
	uint32_t BitsFreeThisByte = 8 - BitOffset; //기록할 수 있었던 비트 수.
	
	//전체 비트를 기록하지 못했다면 (공간이 모자라면), 다음 바이트에 나머지를 기록(넘김).
	if (BitsFreeThisByte < InBitCount)
	{
		//다음 바이트에 나머지 비트를 기록
		mBuffer[ByteOffset + 1] = InData >> BitsFreeThisByte; //기록한 비트를 제외한 나머지 비트.
	}

	mBitHead = NextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* InData, size_t InBitCount)
{
	const char* SrcByte = static_cast<const char*>(InData);
	
	//바이트를 하나씩 모두 기록 (1바이트씩 앞으로 가면서)
	while (InBitCount > 8)
	{
		WriteBits(*SrcByte, 8);
		++SrcByte;
		InBitCount -= 8;
	}
	
	//아직 남은 비트를 기록
	if (InBitCount > 0)
	{
		WriteBits(*SrcByte, InBitCount);
	}
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t InNewBitCapacity)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, InNewBitCapacity));
	//realloc - 동적할당된 메모리 내용을 유지하며, 할당된 메모리 크기를 변경함

	if (mBuffer == nullptr)
	{
		printf("OutputMemoryBitStream::ReallocBuffer failed");
		return;
	}

	mBitCapacity = InNewBitCapacity;
}
