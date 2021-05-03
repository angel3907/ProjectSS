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

	//��Ʈ���� ������ ���� ��ġ �����͸� ����.
	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	void Write(const void* InData, size_t InByteCnt);
	template<class T> void Write(const T& InData)
	{
		//�ڷ��� Ư�� ������ �̿��Ͽ� ���� �ܾ��� ����.

		//���� �ڷ������� ���θ� ������ Ÿ�ӿ� �˻�
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		if (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
		{
			Write(&InData, sizeof(InData));
		}
		else //����� ���� ����Ʈ ���ҿ��� ����
		{
			T SwappedData = ByteSwap(InData);
			Write(&SwappedData, sizeof(SwappedData));
		}
	}

private:
	void ReallocBuffer(uint32_t InNewLength);

	char* mBuffer; //���� ������
	uint32_t mHead; //���� �����Ͱ� ������ á����. (������� �� ��)
	uint32_t mCapacity; //���� �뷮
};