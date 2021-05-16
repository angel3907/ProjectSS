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

	//printf("I Read %d Bits\n", InBitCount);
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

void InputMemoryBitStream::Read(GameObject* OutGameObject)
{
	uint32_t NetworkId;
	Read(NetworkId);
	//TODO. 뒤에 이어서
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
					  OutQuat.Z * OutQuat.Z )); //float을 반환하는 제곱근 구하기

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