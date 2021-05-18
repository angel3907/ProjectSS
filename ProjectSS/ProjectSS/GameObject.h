#pragma once
#include "MathUtils.h"
#include "Serializable.h"
#include <memory>

class LinkingContext;
class RPCManager;
class RPCParams;
class OutputMemoryBitStream;
class InputMemoryBitStream;

enum GameObjectProperties : uint32_t
{
	GOP_Pos = 1 << 0,
	GOP_MAX
};

//'\'�� �� �������� �ٿ� ��ũ�θ� ���� �࿡ ���� �� �� ����. �� �ּ��� ���� �޸� �ȵǳ�..
#define CLASS_IDENTIFICATION(InCode, InClass)\
enum {kClassId = InCode};\
virtual uint32_t GetClassId() const {return kClassId;}\
static GameObject* CreateInstance() {return static_cast< GameObject* >(new InClass());}

class GameObject : public Serializable
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos){}
	virtual ~GameObject();

	CLASS_IDENTIFICATION('GOBJ', GameObject);

	virtual void Update() {}
	virtual void Render() {}

	virtual void Write(OutputMemoryBitStream& InStream) const override;
	virtual void Read(InputMemoryBitStream& InStream) override;

	virtual void WriteChanged(OutputMemoryBitStream& InStream) const override;
	virtual void ReadChanged(InputMemoryBitStream& InStream) override;

	//�Ʒ��� RMI �׽�Ʈ��
	void PrintSomething(const std::string& InName, const Vector2 InLocation, float InVolume);
	void UnwrapPrintSomething(InputMemoryBitStream& InStream, bool bFuncCall);
	void PrintSomething(OutputMemoryBitStream& InStream, RPCParams* InRPCParams);
	//��

	void SetNetworkId(int InNetworkId) { mNetworkId = InNetworkId; }
	int GetNetworkId() const {return mNetworkId;}

protected:
	Vector2 Pos;
	int mNetworkId;

public:
	Vector2 GetPos() {return Pos;}
	void SetPos(Vector2 NewPos){Pos = NewPos;}
};

using GameObjectPtr = std::shared_ptr<GameObject>;