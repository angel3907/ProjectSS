#pragma once
#include <stdint.h>
#include <string>
#include <vector>
enum class EPrimitiveType
{
	EPT_Uint8,
	EPT_String,
	EPT_BOOL
};

//클래스의 한 멤버 변수를 나타내는 클래스
class MemberVariable
{
public:
	MemberVariable(const char* InName, EPrimitiveType InPrimitiveType, uint32_t InOffset) : 
					mName(InName), mPrimitiveType(InPrimitiveType), mOffset(InOffset){}
	
	uint32_t GetOffset() const { return mOffset; }
	EPrimitiveType GetPrimitiveType() const {return mPrimitiveType;}
	std::string GetName() const {return mName;}

private:
	std::string mName; //변수 이름
	EPrimitiveType mPrimitiveType; //원시 자료형
	uint32_t mOffset; //클래스에서 오프셋 (중요! 객체 위치에서 이 값을 더해 해당 멤버변수 위치를 알아내서 읽고 쓸 수 있음)
};

//클래스마다의 모든 멤버변수 정보를 가지는 클래스
//데이터 주도 직렬화가 필요한 클래스마다 이에 대응되는 DataType 인스턴스를 만들어야 함.
class DataType
{
public:
	DataType(std::vector<MemberVariable>&& InMVs) : mMemberVariables(InMVs){}

	const std::vector<MemberVariable>& GetMemberVariables() const
	{
		return mMemberVariables;
	}

private:
	std::vector<MemberVariable> mMemberVariables;
};