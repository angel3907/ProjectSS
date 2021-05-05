#pragma once
#include "MathUtils.h"

//'\'�� �� �������� �ٿ� ��ũ�θ� ���� �࿡ ���� �� �� ����. �� �ּ��� ���� �޸� �ȵǳ�..
#define CLASS_IDENTIFICATION(InCode, InClass)\
enum {kClassId = InCode};\
virtual uint32_t GetClassId() const {return kClassId;}\
static GameObject* CreateInstance() {return new InClass();}

class GameObject
{
public:
	GameObject(Vector2 Pos = {0,0}) : Pos(Pos){}
	virtual ~GameObject(){}
	CLASS_IDENTIFICATION('GOBJ', GameObject);

protected:
	Vector2 Pos;

public:
	Vector2 GetPos() {return Pos;}
	void SetPos(Vector2 NewPos){Pos = NewPos;}
};