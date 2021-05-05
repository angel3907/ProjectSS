#pragma once

#include "GameObjectSubClasses.h"
#include <unordered_map>

using GameObjectCreationFunc = GameObject* (*)(); //Gameobject*을 리턴하는 함수 포인터 형식

//객체 생성 레지스트리 - 식별자로 객체 생성 가능
class ObjectCreationRegistry
{
public:
	static ObjectCreationRegistry& Get() //객체 생성 레지스트리 객체 리턴
	{
		static ObjectCreationRegistry sInstance;
		return sInstance;
	}

	template <class T>
	void RegisterCreationFunction() //식별자 - 생성 함수 묶기 - 식별자와 생성함수가 서로 맞지 않는 것을 방지하기 위해 제공 (자동으로 해당함수 찾아서 불러줌)
	{
		//중복된 클래스 아이디가 없어야 함.
		assert(mNameToGameObjectCreationFunctionMap.find(T::kClassId) 
			== mNameToGameObjectCreationFunctionMap.end());

		mNameToGameObjectCreationFunctionMap[T::kClassId] = T::CreateInstance;
	}

	GameObject* CreateGameObject(uint32_t InClassId) //식별자를 주면 객체를 생성해주는 함수
	{
		//필요시 에러체크 추가
		//현재는 없을 때 크래시가 날 것임.

		GameObjectCreationFunc CreationFunc = mNameToGameObjectCreationFunctionMap[InClassId];
		GameObject* GameObject = CreationFunc();
		return GameObject;
	}

private:
	ObjectCreationRegistry(){}
	std::unordered_map<uint32_t, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap; //식별자 - 객체 생성 함수 매핑해둔 맵
};

//객체 생성 레지스트리를 사용하려면, 게임 시작 코드 적당한 위치에서 호출해야 함.
void RegisterObjectCreation() //리플리케이션 하고 싶은 각 클래스를 객체 생성 레지스트리에 등록 (클래스 식별자 - 객체 생성 함수)
{
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Player>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}