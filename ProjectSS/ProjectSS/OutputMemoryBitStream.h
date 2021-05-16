#pragma once
#include <WinSock2.h>
#include <stdint.h>
#include <vector>
#include "MathUtils.h"
#include "MemoryBitStream.h"
class GameObject;

inline uint32_t ConvertToFixed(float InNumber, float InMin, float InPrecision)
{
	return static_cast<uint32_t>((InNumber - InMin) / InPrecision); //해당 부동소수점에 매핑된 고정소수점 값을 얻어옴.
}

inline float ConvertFromFixed(uint32_t InNumber, float InMin, float InPrecision)
{
	return static_cast<float>(InNumber) * InPrecision + InMin; //해당 고정소수점에 매핑된 부동소수점 값을 얻어옴.
}

class OutputMemoryBitStream : public MemoryBitStream
{
public:
	OutputMemoryBitStream() { ReallocBuffer(256); } //32바이트
	~OutputMemoryBitStream() { delete mBuffer; }
	
	virtual void Serialize(void* IoData, uint32_t InBitCount)
	{
		WriteBits(IoData, InBitCount);
	}

	virtual bool IsInput() const { return false; } //쓰기

	void WriteBits(uint8_t InData, size_t InBitCount);
	void WriteBits(const void* InData, size_t InBitCount);

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetByteLength() const { return (mBitHead + 7) >> 3;} //>>3은 나누기 8, Head가 1비트라도 있으면 1바이트 있는 취급.

	void WriteBytes(const void* InData, size_t InByteCount)
	{
		WriteBits(InData, InByteCount << 3); //*3 해서 비트수로 바꿔서 보내줌.
	}

	template<class T>
	void Write(T InData, size_t InBitCount = sizeof(T) * 8)
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

		WriteBits(&InData, InBitCount);
	}

	template<class T>
	void Write(const std::vector<T>& InVector)
	{
		//유의 : 벡터의 길이부터 기록함. 이를 통해 받는 쪽에서 그만큼 크기의 벡터를 할당하고 받을 수 있음.
		size_t ElementCount = InVector.size();
		Write(ElementCount);

		//벡터의 각 원소를 하나씩 직렬화
		for (const T& Element : InVector)
		{
			Write(Element);
		}
	}

	void Write(const std::string& InString);
	void Write(const GameObject* InGameObject);

	void Write(const Quaternion& InQuat);

	void WritePosF(const Vector2& InVector);

private:
	void ReallocBuffer(uint32_t InNewBitCapacity);

	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};