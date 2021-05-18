#pragma once
#include "GameObject.h"
#include <stdint.h>
#include "MemberVariable.h"
#include <vector>

enum StarStatusProperties : uint32_t
{
	SSP_Value = 1 << 0,
	SSP_bHidden = 1 << 1,
	SSP_MAX
};

#define STAR_S_RADIUS 32;
#define STAR_L_RADIUS 64;


//Ŭ������ &(����)���� �����ڸ� ���� OR ����Ŭ������ ��� OR ��������� ���۷����� �����ٸ�, �� �Լ��� ����� �� ����.
#define OffsetOf(c, mv) ((size_t) &(static_cast<c*>(nullptr)->mv))

//pod ���� ����������� ������ Ŭ���� ó���� ���� ��, POD�� �� �ִ� �κи� ���� ó���ϱ� ����.
class StarStatus : public Serializable
{
public:
	StarStatus(uint8_t Value = 1, bool bHidden = true, int InStarRadius = 32) : Value(Value), bHidden(bHidden), StarRadius(InStarRadius){}

	uint8_t Value; //1 �Ǵ� 3 ���� ����. bool�� �ص��ɰ� ������.. �ϴ� 2��Ʈ ������ ������
	bool bHidden; //������ �Ծ �Ⱥ��̰� �ƴ��� ����
	uint8_t StarRadius; //���� ������ (32 or 64)

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const;
	virtual void ReadChanged(InputMemoryBitStream& InStream);

	void SetValue(uint8_t InValue)
	{
		Value = InValue;

		if (Value == 1)
		{
			StarRadius = 32;
		}
		else //3		
		{
			StarRadius = 64;
		}
	}

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
	virtual ~Star(){}
	StarStatus& GetStarStatus() { return mStarStatus; }
	//CLASS_IDENTIFICATION('STAR', Star);

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const;
	virtual void ReadChanged(InputMemoryBitStream& InStream);

private:
	StarStatus mStarStatus;
};