#pragma once
#include <stdint.h>
#include "GameObject.h"
#include <unordered_map>
#include <unordered_set>
#include "ReplicationHeader.h"

struct ReplicationCommand
{
	int32_t NetworkId;
	ReplicationAction RA;
};


class LinkingContext
{
public:
	static LinkingContext& Get()
	{
		static LinkingContext sInstance;
		return sInstance;
	}

	uint32_t GetNetworkId(GameObject* InGameObject, bool InShouldCreateIfNotFound);
	
	void AddGameObject(GameObject* InGameObject, uint32_t InNetworkId);
	void RemoveGameObject(GameObject* InGameObject);

	GameObject* GetGameObject(uint32_t InNetworkId);

	std::unordered_set<GameObject*>& GetGameObjectSet() { return GameObjects; }

private:
	LinkingContext() : mNextNetworkId(1) {}

	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap; //네트워크 아이디에 해당 객체 포인터를 매핑해둔 맵.
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkMap; //객체 포인터에 네트워크 아이디를 매핑해둔 맵.
	std::unordered_set<GameObject*> GameObjects;

	uint32_t mNextNetworkId; //다음에 객체에 할당할 네트워크 아이디
};