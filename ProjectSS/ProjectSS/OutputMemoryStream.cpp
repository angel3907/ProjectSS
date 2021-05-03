#include "stdafx.h"

void OutputMemoryStream::Write(const void* InData, size_t InByteCnt)
{
	//일단 공간을 충분히 확보
	uint32_t ResultHead = mHead + static_cast<uint32_t>(InByteCnt);
	if (ResultHead > mCapacity)
	{
		ReallocBuffer(max(mCapacity*2, ResultHead));
	}

	//버퍼의 제일 앞에 복사
	std::memcpy(mBuffer + mHead, InData, InByteCnt);
	
	//mHead를 전진시켜 다음 기록에 대비
	mHead = ResultHead;
}

void OutputMemoryStream::ReallocBuffer(uint32_t InNewLength)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, InNewLength));
	//realloc - 동적할당된 메모리 내용을 유지하며, 할당된 메모리 크기를 변경함

	if (mBuffer == nullptr)
	{
		printf("OutputMemoryStream::ReallocBuffer failed");
		return;
	}
	
	mCapacity = InNewLength;
}