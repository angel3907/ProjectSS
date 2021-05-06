#pragma once
#include "MathUtils.h"
#include "Serializable.h"

enum GameObjectProperties : uint32_t
{
	GOP_Pos = 1 << 0,
	GOP_MAX
};


class LinkingContext;
class OutputMemoryBitStream;
class InputMemoryBitStream;

//'\'를 맨 마지막에 붙여 매크로를 여러 행에 걸쳐 쓸 수 있음. 아 주석도 옆에 달면 안되네..
#define CLASS_IDENTIFICATION(InCode, InClass)\
enum {kClassId = InCode};\
virtual uint32_t GetClassId() const {return kClassId;}\
static GameObject* CreateInstance() {return new InClass();}

class GameObject : public Serializable
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos)
	{
		//일단 보류
		//LinkingContext::Get().GetNetworkId(this, true);
	}

	virtual ~GameObject()
	{
		//LinkingContext::Get().RemoveGameObject(this);
	}

	CLASS_IDENTIFICATION('GOBJ', GameObject);

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const override;
	virtual void ReadChanged(InputMemoryBitStream& InStream) override;

protected:
	Vector2 Pos;

public:
	Vector2 GetPos() {return Pos;}
	void SetPos(Vector2 NewPos){Pos = NewPos;}
};