#pragma once
#include <stdint.h>
#include "GameObject.h"
#include "Player.h"
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
	std::unordered_set<Player*>& GetPlayerPtrSet() {return Players;}

private:
	LinkingContext() : mNextNetworkId(1) {}

	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap; //��Ʈ��ũ ���̵� �ش� ��ü �����͸� �����ص� ��.
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkMap; //��ü �����Ϳ� ��Ʈ��ũ ���̵� �����ص� ��.
	std::unordered_set<GameObject*> GameObjects;

	std::unordered_set<Player*> Players; //�÷��̾� �����͸� ��Ƶ� ��

	uint32_t mNextNetworkId; //������ ��ü�� �Ҵ��� ��Ʈ��ũ ���̵�
};