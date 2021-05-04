#include "stdafx.h"

void OutputMemoryBitStream::WriteBits(uint8_t InData, size_t InBitCount)
{
	//��Ʈ�� ��Ʈ���� ����ϴ� ���� �۾� ����
	//����Ʈ �ϳ��� �Ű������� �޾� ������ ��Ʈ����ŭ ��Ʈ�� �����Ͽ� ��Ʈ���� �����.
	
	//���� ������ �޾ƿ� ���� Head ��ġ ���� & �뷮�Ҵ�
	uint32_t NextBitHead = mBitHead + static_cast<uint32_t>(InBitCount);
	if (NextBitHead > mBitCapacity)
	{
		ReallocBuffer(max(mBitCapacity * 2, NextBitHead));
	}

	//����Ʈ ������ : ��Ʈ ��带 8�� ����
	//��Ʈ ������ : 8�� ���� ������
	//���� ���ۿ� �����Ͱ� x(ByteOffset)����Ʈ + y(BitOffset)��Ʈ ����
	uint32_t ByteOffset = mBitHead >> 3;
	uint32_t BitOffset = mBitHead & 0x07; //8�� ���� �������� ����. �Ǵ� ����Ʈ�ϴ� �������� 3��Ʈ�� ���ϸ� ��
	
	//���� ó�� �� ����Ʈ���� �� ��Ʈ�� ������ ���
	uint8_t CurrentMask = ~(0xff << BitOffset);
	mBuffer[ByteOffset] = (mBuffer[ByteOffset] & CurrentMask) | (InData << BitOffset);
	
	//����� ����Ʈ�� �� ��Ʈ�� ���� �����ֳ� ���
	uint32_t BitsFreeThisByte = 8 - BitOffset; //����� �� �־��� ��Ʈ ��.
	
	//��ü ��Ʈ�� ������� ���ߴٸ� (������ ���ڶ��), ���� ����Ʈ�� �������� ���(�ѱ�).
	if (BitsFreeThisByte < InBitCount)
	{
		//���� ����Ʈ�� ������ ��Ʈ�� ���
		mBuffer[ByteOffset + 1] = InData >> BitsFreeThisByte; //����� ��Ʈ�� ������ ������ ��Ʈ.
	}

	mBitHead = NextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* InData, size_t InBitCount)
{
	const char* SrcByte = static_cast<const char*>(InData);
	
	//����Ʈ�� �ϳ��� ��� ��� (1����Ʈ�� ������ ���鼭)
	while (InBitCount > 8)
	{
		WriteBits(*SrcByte, 8);
		++SrcByte;
		InBitCount -= 8;
	}
	
	//���� ���� ��Ʈ�� ���
	if (InBitCount > 0)
	{
		WriteBits(*SrcByte, InBitCount);
	}
}

void OutputMemoryBitStream::Write(const GameObject* InGameObject)
{
	//TODO. �ڿ� �̾
	//uint32_t NetworkId = mLinkingContext->GetNetworkId(InGameObject);
	//Write(NetworkId);
}

void OutputMemoryBitStream::Write(const string& InString)
{
	size_t ElementCount = InString.size();
	Write(ElementCount);

	for (const auto& Element : InString)
	{
		Write(Element);
	}
}

void OutputMemoryBitStream::Write(const Quaternion& InQuat)
{
	float Precision = (2.f / 65535.f); //-1~1 ��, ���е� 16��Ʈ
	Write(ConvertToFixed(InQuat.X, -1.f, Precision), 16);
	Write(ConvertToFixed(InQuat.Y, -1.f, Precision), 16);
	Write(ConvertToFixed(InQuat.Z, -1.f, Precision), 16);
	Write(InQuat.W < 0); //������ ���Ҵ� ��ȣ�� ���.
}

void OutputMemoryBitStream::WritePosF(const Vector2& InVector)
{
	Write(ConvertToFixed(InVector.PosX, -WORLD_HALF_WIDTH, PRECISION_POS), REQUIRED_BITNUM_POS);
	Write(ConvertToFixed(InVector.PosY, -WORLD_HALF_HEIGHT, PRECISION_POS), REQUIRED_BITNUM_POS);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t InNewBitCapacity)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, InNewBitCapacity));
	//realloc - �����Ҵ�� �޸� ������ �����ϸ�, �Ҵ�� �޸� ũ�⸦ ������

	if (mBuffer == nullptr)
	{
		printf("OutputMemoryBitStream::ReallocBuffer failed");
		return;
	}

	mBitCapacity = InNewBitCapacity;
}
