#pragma once
#define STREAM_ENDIANNESS 0
#define PLATFORM_ENDIANNESS 0
#include "ByteSwapper.h"

class OutputMemoryStream
{
public:
	OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0)
	{ ReallocBuffer(32); }

	~OutputMemoryStream() { delete mBuffer; }

	//스트림의 데이터 시작 위치 포인터를 구함.
	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	void Write(const void* InData, size_t InByteCnt);
	template<class T> void Write(const T& InData)
	{
		//자료형 특성 정보를 이용하여 정적 단언문을 쓰기.

		//원시 자료형인지 여부를 컴파일 타임에 검사
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Write(&InData, sizeof(InData));
		}
		else //엔디언에 따라 바이트 스왑여부 결정
		{
			T SwappedData = ByteSwap(InData);
			Write(&SwappedData, sizeof(SwappedData));
		}
	}

private:
	void ReallocBuffer(uint32_t InNewLength);

	char* mBuffer; //버퍼 포인터
	uint32_t mHead; //버퍼 데이터가 어디까지 찼는지. (여기부터 쓸 것)
	uint32_t mCapacity; //버퍼 용량
};