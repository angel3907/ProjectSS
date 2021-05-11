#pragma once
#include <stdint.h>
#include <vector>
#include "MathUtils.h"
#include "MemoryBitStream.h"
#include <string>
class GameObject;

class InputMemoryBitStream : public MemoryBitStream
{
public:
	InputMemoryBitStream(char* InBuffer, uint32_t InBitCount) 
		: mBuffer(InBuffer), mBitCapacity(InBitCount), mBitHead(0) {}

	~InputMemoryBitStream() { delete mBuffer; }

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetRemainingBitCount() const { return mBitCapacity - mBitHead; }

	virtual void Serialize(void* IoData, uint32_t InBitCount )
	{
		ReadBits(IoData, InBitCount);
	}

	virtual bool IsInput() const { return true; } //�б�

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

	void Read(std::string& OutString);
	void ReadPosF(Vector2& OutVector);

	void Read(GameObject* OutGameObject);

	void Read(Quaternion& OutQuat);

private:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};