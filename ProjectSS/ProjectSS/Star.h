#pragma once
#include "GameObject.h"
#include <stdint.h>
#include "MemberVariable.h"
#include <vector>

enum StarStatusProperties : uint8_t
{
	SSP_Value = 1 << 0,
	SSP_bHidden = 1 << 1,
	SSP_MAX
};

//Ŭ������ &(����)���� �����ڸ� ���� OR ����Ŭ������ ��� OR ��������� ���۷����� �����ٸ�, �� �Լ��� ����� �� ����.
#define OffsetOf(c, mv) ((size_t) &(static_cast<c*>(nullptr)->mv))

//pod ���� ����������� ������ Ŭ���� ó���� ���� ��, POD�� �� �ִ� �κи� ���� ó���ϱ� ����.
class StarStatus : public Serializable
{
public:
	StarStatus(uint8_t Value = 1, bool bHidden = false) : Value(Value), bHidden(bHidden){}

	uint8_t Value; //1 �Ǵ� 3 ���� ����. bool�� �ص��ɰ� ������.. �ϴ� 2��Ʈ ������ ������
	bool bHidden; //������ �Ծ �Ⱥ��̰� �ƴ��� ����

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream, uint32_t InProperties) const;
	virtual void ReadChanged(InputMemoryBitStream& InStream);

	static DataType* sDataType;

	//�ʱ�ȭ�� ������ ������ �� �Լ��� ȣ������
	static void InitDataType()
	{
		sDataType = new DataType
		({
			MemberVariable("Value", EPrimitiveType::EPT_Uint8, OffsetOf(StarStatus, Value)),
			MemberVariable("bHidden", EPrimitiveType::EPT_BOOL, OffsetOf(StarStatus, bHidden))
		});
	}
};

class Star : public GameObject
{
public:
	Star(StarStatus InStarStatus = StarStatus()) : mStarStatus(InStarStatus){}
	~Star(){}
	const StarStatus& GetStarStatus() const { return mStarStatus; }
	CLASS_IDENTIFICATION('STAR', Star);

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

private:
	StarStatus mStarStatus;
};