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
		ReadBits(OutData, InByteCount << 3); //*3 �ؼ� ��Ʈ���� �ٲ㼭 ������.
	}

	template<class T>
	void Read(T& OutData, uint32_t InBitCount = sizeof(T) * 8)
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

		ReadBits(&OutData, InBitCount);
	}

	template<class T>
	void Read(std::vector<T>& OutVector)
	{
		//���� ���̺��� �о�ͼ� ������ ���� ���Ҵ�
		size_t ElementCount = 0;
		Read(ElementCount);
		OutVector.resize(ElementCount);

		//������ �� ���Ҹ� �ϳ��� ����
		for (const T& Element : OutVector)
		{
			Read(Element);
		}
	}

	void Read(string& OutString);
	void ReadPosF(Vector2& OutVector);

	void Read(GameObject* OutGameObject);

private:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};