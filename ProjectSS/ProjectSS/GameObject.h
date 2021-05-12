#pragma once
#include "MathUtils.h"
#include "Serializable.h"

class RPCManager;
class RPCParams;
class LinkingContext;
class OutputMemoryBitStream;
class InputMemoryBitStream;

enum GameObjectProperties : uint32_t
{
	GOP_Pos = 1 << 0,
	GOP_MAX
};

//'\'를 맨 마지막에 붙여 매크로를 여러 행에 걸쳐 쓸 수 있음. 아 주석도 옆에 달면 안되네..
#define CLASS_IDENTIFICATION(InCode, InClass)\
enum {kClassId = InCode};\
virtual uint32_t GetClassId() const {return kClassId;}\
static GameObject* CreateInstance() {return new InClass();}

class GameObject : public Serializable
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos){}
	virtual ~GameObject(){}

	CLASS_IDENTIFICATION('GOBJ', GameObject);

	virtual void Update() {}

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const override;
	virtual void ReadChanged(InputMemoryBitStream& InStream) override;

	//아래는 RMI 테스트용
	void PrintSomething(const string& InName, const Vector2 InLocation, float InVolume);
	void UnwrapPrintSomething(InputMemoryBitStream& InStream, bool bFuncCall);
	void PrintSomething(OutputMemoryBitStream& InStream, RPCParams* InRPCParams);
	//끝

protected:
	Vector2 Pos;

public:
	Vector2 GetPos() {return Pos;}
	void SetPos(Vector2 NewPos){Pos = NewPos;}
};

using GameObjectPtr = shared_ptr<GameObject>;