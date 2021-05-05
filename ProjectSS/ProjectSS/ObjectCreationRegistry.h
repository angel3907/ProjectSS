#pragma once

#include "GameObjectSubClasses.h"
#include <unordered_map>

using GameObjectCreationFunc = GameObject* (*)(); //Gameobject*�� �����ϴ� �Լ� ������ ����

//��ü ���� ������Ʈ�� - �ĺ��ڷ� ��ü ���� ����
class ObjectCreationRegistry
{
public:
	static ObjectCreationRegistry& Get() //��ü ���� ������Ʈ�� ��ü ����
	{
		static ObjectCreationRegistry sInstance;
		return sInstance;
	}

	template <class T>
	void RegisterCreationFunction() //�ĺ��� - ���� �Լ� ���� - �ĺ��ڿ� �����Լ��� ���� ���� �ʴ� ���� �����ϱ� ���� ���� (�ڵ����� �ش��Լ� ã�Ƽ� �ҷ���)
	{
		//�ߺ��� Ŭ���� ���̵� ����� ��.
		assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId) 
			== mNameToGameObjectCreationFunctionMap.end());

		mNameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t InClassId) //�ĺ��ڸ� �ָ� ��ü�� �������ִ� �Լ�
	{
		//�ʿ�� ����üũ �߰�
		//����� ���� �� ũ���ð� �� ����.

		GameObjectCreationFunc CreationFunc = mNameToGameObjectCreationFunctionMap[InClassId];
		GameObject* GameObject = CreationFunc();
		return GameObject;
	}

private:
	ObjectCreationRegistry(){}
	std::unordered_map<uint32_t, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap; //�ĺ��� - ��ü ���� �Լ� �����ص� ��
};

//��ü ���� ������Ʈ���� ����Ϸ���, ���� ���� �ڵ� ������ ��ġ���� ȣ���ؾ� ��.
void RegisterObjectCreation() //���ø����̼� �ϰ� ���� �� Ŭ������ ��ü ���� ������Ʈ���� ��� (Ŭ���� �ĺ��� - ��ü ���� �Լ�)
{
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Player>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}