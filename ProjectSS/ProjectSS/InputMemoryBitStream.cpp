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

	//printf("I Read %d Bits\n", InBitCount);
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

void InputMemoryBitStream::Read(GameObject* OutGameObject)
{
	uint32_t NetworkId;
	Read(NetworkId);
	//TODO. �ڿ� �̾
	//OutGameObject = mLinkingContext->GetGameObject(NetworkId);
}

void InputMemoryBitStream::Read(std::string& OutString)
{
	size_t ElementCount = 0;
	Read(ElementCount);
	OutString.resize(ElementCount);

	for (auto& Element : OutString)
	{
		Read(Element);
	}
}

void InputMemoryBitStream::Read(Quaternion& OutQuat)
{
	float Precision = (2.f / 65535.f);

	uint32_t f = 0;

	Read(f, 16);
	OutQuat.X = ConvertFromFixed(f, -1.f, Precision);
	Read(f, 16);
	OutQuat.Y = ConvertFromFixed(f, -1.f, Precision);
	Read(f, 16);
	OutQuat.Z = ConvertFromFixed(f, -1.f, Precision);

	OutQuat.W = sqrtf(1.f - 
					 (OutQuat.X * OutQuat.X + 
					  OutQuat.Y * OutQuat.Y + 
					  OutQuat.Z * OutQuat.Z )); //float�� ��ȯ�ϴ� ������ ���ϱ�

	bool IsNegative = false;
	Read(IsNegative);

	if (IsNegative)
		OutQuat.W *= -1;
}

void InputMemoryBitStream::ReadPosF(Vector2& OutVector)
{
	uint32_t PosX = 0, PosY = 0;
	Read(PosX, REQUIRED_BITNUM_POS);
	Read(PosY, REQUIRED_BITNUM_POS);

	OutVector.PosX = ConvertFromFixed(PosX, -WORLD_HALF_WIDTH, PRECISION_POS);
	OutVector.PosY = ConvertFromFixed(PosY, -WORLD_HALF_HEIGHT, PRECISION_POS);
}