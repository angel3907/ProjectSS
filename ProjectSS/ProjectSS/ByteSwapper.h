#pragma once
#include <stdint.h>

//����Ʈ ������ �Լ�
inline uint16_t ByteSwap2(uint16_t InData)
{
	return (InData >> 8) || (InData << 8);
}

inline uint32_t ByteSwap4(uint32_t InData)
{	
	return (InData >> 24) & (0x000000FF) |
		   (InData <<  8) & (0x0000FF00) |
		   (InData >>  8) & (0x00FF0000) |
		   (InData << 24) & (0xFF000000);
}

inline uint64_t ByteSwap8(uint64_t InData)
{
	return (InData >> 56) & (0x00000000000000FF) |
		   (InData >> 40) & (0x000000000000FF00) |
		   (InData >> 24) & (0x0000000000FF0000) |
		   (InData >>  8) & (0x00000000FF000000) |
		   (InData <<  8) & (0x000000FF00000000) |
		   (InData << 24) & (0x0000FF0000000000) |
		   (InData << 40) & (0x00FF000000000000) |
		   (InData << 56) & (0xFF00000000000000);
}

template <class tFrom, class tTo>
class TypeAliaser //�ڷ��� ��Ī Ŭ����
{
public:
	TypeAliaser(tFrom InFromValue) : 
		mAsFromType(InFromValue){}
	tTo& Get() { return mAsToType; } //��ȯ�ϰ��� �ϴ� Ÿ������ ������.

	union
	{
		tFrom mAsFromType; //���� Ÿ��
		tTo mAsToType; //��ȯ�ϰ��� �ϴ� Ÿ��
	};
};

template <class T, size_t tSize> class ByteSwapper;

//2����Ʈ�� Ư��ȭ
template <class T>
class ByteSwapper<T, 2>
{
public:
	T Swap(T InData) const
	{
		uint16_t Result = ByteSwap2(TypeAliaser<T, uint16_t>(InData).Get()); //�޾ƿ� 2����Ʈ �ڷ����� uint16_t�� �����ϰ�, �� ���¿��� ����Ʈ ����.
		return TypeAliaser<uint16_t, T>(Result).Get(); //�ٽ� �ش� �ڷ������� �ٲ���
	}
};

//4����Ʈ�� Ư��ȭ
template <class T>
class ByteSwapper<T, 4>
{
public:
	T Swap(T InData) const
	{
		uint32_t Result = ByteSwap4(TypeAliaser<T, uint32_t>(InData).Get());
		return TypeAliaser<uint32_t, T>(Result).Get();
	}
};

//8����Ʈ�� Ư��ȭ
template <class T>
class ByteSwapper<T, 8>
{
public:
	T Swap(T InData) const
	{
		uint64_t Result = ByteSwap2(TypeAliaser<T, uint64_t>(InData).Get());
		return TypeAliaser<uint64_t, T>(Result).Get();
	}
};

//����Ʈ ������ ���� ���������� �Ҹ��� �Լ�.
template <class T>
T ByteSwap(T InData)
{
	return ByteSwapper<T, sizeof(T)>().Swap(InData);
}