#pragma once
#include "stdafx.h"
#include "Player.h"

class InputMemoryStream
{
public:
	InputMemoryStream(char* InBuffer, uint32_t InByteCount) : mBuffer(InBuffer), mCapacity(InByteCount), mHead(0)
	{}

	~InputMemoryStream() { delete mBuffer; }

	//���� �뷮
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
	char* mBuffer; //���� ����
	uint32_t mHead; //���� �����Ͱ� ������ á����
	uint32_t mCapacity; //���� �뷮
};
