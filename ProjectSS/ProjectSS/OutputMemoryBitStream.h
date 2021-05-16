#pragma once
#include <WinSock2.h>
#include <stdint.h>
#include <vector>
#include "MathUtils.h"
#include "MemoryBitStream.h"
class GameObject;

inline uint32_t ConvertToFixed(float InNumber, float InMin, float InPrecision)
{
	return static_cast<uint32_t>((InNumber - InMin) / InPrecision); //�ش� �ε��Ҽ����� ���ε� �����Ҽ��� ���� ����.
}

inline float ConvertFromFixed(uint32_t InNumber, float InMin, float InPrecision)
{
	return static_cast<float>(InNumber) * InPrecision + InMin; //�ش� �����Ҽ����� ���ε� �ε��Ҽ��� ���� ����.
}

class OutputMemoryBitStream : public MemoryBitStream
{
public:
	OutputMemoryBitStream() { ReallocBuffer(256); } //32����Ʈ
	~OutputMemoryBitStream() { delete mBuffer; }
	
	virtual void Serialize(void* IoData, uint32_t InBitCount)
	{
		WriteBits(IoData, InBitCount);
	}

	virtual bool IsInput() const { return false; } //����

	void WriteBits(uint8_t InData, size_t InBitCount);
	void WriteBits(const void* InData, size_t InBitCount);

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetBitLength() const { return mBitHead; }
	uint32_t GetByteLength() const { return (mBitHead + 7) >> 3;} //>>3�� ������ 8, Head�� 1��Ʈ�� ������ 1����Ʈ �ִ� ���.

	void WriteBytes(const void* InData, size_t InByteCount)
	{
		WriteBits(InData, InByteCount << 3); //*3 �ؼ� ��Ʈ���� �ٲ㼭 ������.
	}

	template<class T>
	void Write(T InData, size_t InBitCount = sizeof(T) * 8)
	{
		//���� �ڷ������� ���θ� ������ Ÿ�ӿ� �˻�
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");
		
		//bool�� ��� 1��Ʈ�̹Ƿ� 1�� ��������.
		if (typeid(T) == typeid(bool))
		{
			InBitCount = 1;
		}

		WriteBits(&InData, InBitCount);
	}

	template<class T>
	void Write(const std::vector<T>& InVector)
	{
		//���� : ������ ���̺��� �����. �̸� ���� �޴� �ʿ��� �׸�ŭ ũ���� ���͸� �Ҵ��ϰ� ���� �� ����.
		size_t ElementCount = InVector.size();
		Write(ElementCount);

		//������ �� ���Ҹ� �ϳ��� ����ȭ
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