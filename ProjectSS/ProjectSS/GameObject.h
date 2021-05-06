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

//'\'�� �� �������� �ٿ� ��ũ�θ� ���� �࿡ ���� �� �� ����. �� �ּ��� ���� �޸� �ȵǳ�..
#define CLASS_IDENTIFICATION(InCode, InClass)\
enum {kClassId = InCode};\
virtual uint32_t GetClassId() const {return kClassId;}\
static GameObject* CreateInstance() {return new InClass();}

class GameObject : public Serializable
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos)
	{
		//�ϴ� ����
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