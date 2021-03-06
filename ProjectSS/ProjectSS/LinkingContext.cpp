#include "stdafx.h"

uint32_t LinkingContext::GetNetworkId(GameObject* InGameObject, bool InShouldCreateIfNotFound)
{
	auto It = mGameObjectToNetworkMap.find(InGameObject);
	if (It != mGameObjectToNetworkMap.end()) //end를 리턴하면 찾지 못한 것.
	{
		return It->second;
	}
	else if (InShouldCreateIfNotFound)
	{
		uint32_t NewNetworkId = mNextNetworkId++;
		AddGameObject(InGameObject, NewNetworkId);
		return NewNetworkId;
	}
	else
	{
		return 0;
	}
}

void LinkingContext::AddGameObject(GameObject* InGameObject, uint32_t InNetworkId)
{
	mNetworkIdToGameObjectMap[InNetworkId] = InGameObject;
	mGameObjectToNetworkMap[InGameObject] = InNetworkId;
	GameObjects.insert(InGameObject);

	InGameObject->SetNetworkId(InNetworkId); //네트워크 아이디 설정

	//플레이어였다면
	if (Player* PlayerValue = dynamic_cast<Player*>(InGameObject))
	{
		Players.insert(PlayerValue);
	}
}

void LinkingContext::RemoveGameObject(GameObject* InGameObject)
{
	uint32_t NetworkId = mGameObjectToNetworkMap[InGameObject];
	mGameObjectToNetworkMap.erase(InGameObject);
	mNetworkIdToGameObjectMap.erase(NetworkId);
	GameObjects.erase(InGameObject);

	//플레이어였다면
	if (Player* PlayerValue = dynamic_cast<Player*>(InGameObject))
	{
		Players.erase(PlayerValue);
	}
}

GameObject* LinkingContext::GetGameObject(uint32_t InNetworkId)
{
	auto It = mNetworkIdToGameObjectMap.find(InNetworkId);
	if (It != mNetworkIdToGameObjectMap.end()) //end를 리턴하면 찾지 못한 것.
	{
		return It->second;
	}
	else
	{
		return nullptr;
	}
}