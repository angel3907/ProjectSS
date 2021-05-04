#pragma once
#include <stdint.h>
#include "GameInfo.h"

class MemoryBitStream
{
	template<class T>
	void Serialize(T& IoData)
	{
		//���� �ڷ������� ���θ� ������ Ÿ�ӿ� �˻�
		static_assert(
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		uint32_t BitCount = sizeof(T) * 8;

		//bool�� ��� 1��Ʈ�̹Ƿ� 1�� ��������.
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
			if (IsInput()) //�б�
			{
				//�а� ���� ������
				T Data;
				Serialize(&Data, BitCount);
				IoData = ByteSwap(Data);
			}
			else //����
			{
				//���� �� �����ؼ� ��
				T SwappedData = ByteSwap(IoData);
				Serialize(&SwappedData, BitCount);
			}
		}
	}

	virtual void Serialize(void* IoData, uint32_t InBitCount) = 0; //�а� ���� �Լ�
	virtual bool IsInput() const = 0; //�д°��� ���°���
};