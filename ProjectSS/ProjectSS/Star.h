#pragma once
#include "GameObject.h"
#include <stdint.h>
#include "MemberVariable.h"
#include <vector>

//클래스에 &(참조)단항 연산자를 정의 OR 가상클래스를 상속 OR 멤버변수를 레퍼런스로 가진다면, 이 함수는 사용할 수 없다.
#define OffsetOf(c, mv) ((size_t) &(static_cast<c*>(nullptr)->mv))

//pod 관련 제약사항으로 복잡한 클래스 처리가 힘들 때, POD할 수 있는 부분만 떼면 처리하기 편함.
class StarStatus
{
public:
	StarStatus(uint8_t Value = 1, bool bHidden = false) : Value(Value), bHidden(bHidden){}

	uint8_t Value; //1 또는 3 값을 가짐. bool로 해도될것 같지만.. 일단 2비트 보내는 것으로
	bool bHidden; //유저가 먹어서 안보이게 됐는지 여부

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
	Star(StarStatus InStarStatus) : mStarStatus(InStarStatus){}
	const StarStatus& GetStarStatus() const { return mStarStatus; }

private:
	StarStatus mStarStatus;
};