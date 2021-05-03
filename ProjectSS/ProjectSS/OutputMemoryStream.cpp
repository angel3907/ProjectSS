#include "stdafx.h"

void OutputMemoryStream::Write(const void* InData, size_t InByteCnt)
{
	//�ϴ� ������ ����� Ȯ��
	uint32_t ResultHead = mHead + static_cast<uint32_t>(InByteCnt);
	if (ResultHead > mCapacity)
	{
		ReallocBuffer(max(mCapacity*2, ResultHead));
	}

	//������ ���� �տ� ����
	std::memcpy(mBuffer + mHead, InData, InByteCnt);
	
	//mHead�� �������� ���� ��Ͽ� ���
	mHead = ResultHead;
}

void OutputMemoryStream::ReallocBuffer(uint32_t InNewLength)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, InNewLength));
	//realloc - �����Ҵ�� �޸� ������ �����ϸ�, �Ҵ�� �޸� ũ�⸦ ������

	if (mBuffer == nullptr)
	{
		printf("OutputMemoryStream::ReallocBuffer failed");
		return;
	}
	
	mCapacity = InNewLength;
}