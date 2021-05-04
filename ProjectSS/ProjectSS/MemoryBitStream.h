#pragma once
#include <stdint.h>
#include "GameInfo.h"

class MemoryBitStream
{
	template<class T>
	void Serialize(T& IoData)
	{
		//원시 자료형인지 여부를 컴파일 타임에 검사
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		uint32_t BitCount = sizeof(T) * 8;

		//bool의 경우 1비트이므로 1로 설정해줌.
		if (typeid(T) == typeid(bool))
		{
			BitCount = 1;
		}

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Serialize(&IoData, BitCount);
		}
		else
		{
			if (IsInput()) //읽기
			{
				//읽고 나서 스왑함
				T Data;
				Serialize(&Data, BitCount);
				IoData = ByteSwap(Data);
			}
			else //쓰기
			{
				//쓰기 전 스왑해서 씀
				T SwappedData = ByteSwap(IoData);
				Serialize(&SwappedData, BitCount);
			}
		}
	}

	virtual void Serialize(void* IoData, uint32_t InBitCount) = 0; //읽고 쓰는 함수
	virtual bool IsInput() const = 0; //읽는건지 쓰는건지
};