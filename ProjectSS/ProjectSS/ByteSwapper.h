#pragma once
#include <stdint.h>

//바이트 스와핑 함수
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
class TypeAliaser //자료형 별칭 클래스
{
public:
	TypeAliaser(tFrom InFromValue) : 
		mAsFromType(InFromValue){}
	tTo& Get() { return mAsToType; } //변환하고자 하는 타입으로 가져옴.

	union
	{
		tFrom mAsFromType; //원래 타입
		tTo mAsToType; //변환하고자 하는 타입
	};
};

template <class T, size_t tSize> class ByteSwapper;

//2바이트용 특수화
template <class T>
class ByteSwapper<T, 2>
{
public:
	T Swap(T InData) const
	{
		uint16_t Result = ByteSwap2(TypeAliaser<T, uint16_t>(InData).Get()); //받아온 2바이트 자료형을 uint16_t로 변경하고, 그 상태에서 바이트 스왑.
		return TypeAliaser<uint16_t, T>(Result).Get(); //다시 해당 자료형으로 바꿔줌
	}
};

//4바이트용 특수화
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

//8바이트용 특수화
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

//바이트 스왑을 위해 최종적으로 불리는 함수.
template <class T>
T ByteSwap(T InData)
{
	return ByteSwapper<T, sizeof(T)>().Swap(InData);
}