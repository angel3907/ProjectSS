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

//Ŭ������ �� ��� ������ ��Ÿ���� Ŭ����
class MemberVariable
{
public:
	MemberVariable(const char* InName, EPrimitiveType InPrimitiveType, uint32_t InOffset) : 
					mName(InName), mPrimitiveType(InPrimitiveType), mOffset(InOffset){}
	
	uint32_t GetOffset() const { return mOffset; }
	EPrimitiveType GetPrimitiveType() const {return mPrimitiveType;}
	std::string GetName() const {return mName;}

private:
	std::string mName; //���� �̸�
	EPrimitiveType mPrimitiveType; //���� �ڷ���
	uint32_t mOffset; //Ŭ�������� ������ (�߿�! ��ü ��ġ���� �� ���� ���� �ش� ������� ��ġ�� �˾Ƴ��� �а� �� �� ����)
};

//Ŭ���������� ��� ������� ������ ������ Ŭ����
//������ �ֵ� ����ȭ�� �ʿ��� Ŭ�������� �̿� �����Ǵ� DataType �ν��Ͻ��� ������ ��.
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