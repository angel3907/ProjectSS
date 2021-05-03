#pragma once
#include "stdafx.h"
#include "Player.h"

class InputMemoryStream
{
public:
	InputMemoryStream(char* InBuffer, uint32_t InByteCount) : mBuffer(InBuffer), mCapacity(InByteCount), mHead(0)
	{}

	~InputMemoryStream() { delete mBuffer; }

	//남은 용량
	uint32_t GetRemainingDataSize() const {return mCapacity - mHead;}

	void Read(void* OutData, uint32_t InByteCount);
	template<class T> 
	void Read(T& OutData)
	{
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only Supports primitive data types"
			);

		Read(&OutData, sizeof(OutData));
	}
	
private:
	char* mBuffer; //수신 버퍼
	uint32_t mHead; //버퍼 데이터가 어디까지 찼는지
	uint32_t mCapacity; //버퍼 용량
};
