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


//클래스에 &(참조)단항 연산자를 정의 OR 가상클래스를 상속 OR 멤버변수를 레퍼런스로 가진다면, 이 함수는 사용할 수 없다.
#define OffsetOf(c, mv) ((size_t) &(static_cast<c*>(nullptr)->mv))

//pod 관련 제약사항으로 복잡한 클래스 처리가 힘들 때, POD할 수 있는 부분만 떼면 처리하기 편함.
class StarStatus : public Serializable
{
public:
	StarStatus(uint8_t Value = 1, bool bHidden = true, int InStarRadius = 32) : Value(Value), bHidden(bHidden), StarRadius(InStarRadius){}

	uint8_t Value; //1 또는 3 값을 가짐. bool로 해도될것 같지만.. 일단 2비트 보내는 것으로
	bool bHidden; //유저가 먹어서 안보이게 됐는지 여부
	uint8_t StarRadius; //별의 반지름 (32 or 64)

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

	//초기화시 적절한 시점에 이 함수를 호출해줌
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