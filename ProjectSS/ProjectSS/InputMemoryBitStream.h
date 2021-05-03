#pragma once
#include "stdafx.h"

class InputMemoryBitStream
{
public:
	InputMemoryBitStream(char* InBuffer, uint32_t InBitCount) 
		: mBuffer(InBuffer), mBitCapacity(InBitCount), mBitHead(0) {}

	~InputMemoryBitStream() { delete mBuffer; }

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetRemainingBitCount() const { return mBitCapacity - mBitHead; }

	void ReadBits(uint8_t& OutData, uint32_t InBitCount);
	void ReadBits(void* OutData, uint32_t InBitCount);
	void ReadBytes(void* OutData, uint32_t InByteCount)
	{
		ReadBits(OutData, InByteCount << 3); //*3 해서 비트수로 바꿔서 보내줌.
	}

	template<class T>
	void Read(T& OutData, uint32_t InBitCount = sizeof(T) * 8)
	{
		//원시 자료형인지 여부를 컴파일 타임에 검사
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		//bool의 경우 1비트이므로 1로 설정해줌.
		if (typeid(T) == typeid(bool))
		{
			InBitCount = 1;
		}

		ReadBits(&OutData, InBitCount);
	}

private:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};