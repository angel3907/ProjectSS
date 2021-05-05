#pragma once
#include <stdint.h>
#include "GameObject.h"
#include <unordered_map>

class LinkingContext
{
public:
	LinkingContext() : mNextNetworkId(1) {}
	uint32_t GetNetworkId(GameObject* InGameObject, bool InShouldCreateIfNotFound);
	
	void AddGameObject(GameObject* InGameObject, uint32_t InNetworkId);
	void RemoveGameObject(GameObject* InGameObject);

	GameObject* GetGameObject(uint32_t InNetworkId);

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap; //��Ʈ��ũ ���̵� �ش� ��ü �����͸� �����ص� ��.
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkMap; //��ü �����Ϳ� ��Ʈ��ũ ���̵� �����ص� ��.

	uint32_t mNextNetworkId; //������ ��ü�� �Ҵ��� ��Ʈ��ũ ���̵�
};