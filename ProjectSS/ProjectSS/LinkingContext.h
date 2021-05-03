#pragma once
#include <stdint.h>
#include "GameObject.h"
#include <unordered_map>

class LinkingContext
{
public:
	uint32_t GetNetworkId(GameObject* InGameObject);
	GameObject* GetGameObject(uint32_t InNetworkId);

private:
	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap; //네트워크 아이디에 해당 객체 포인터를 매핑해둔 맵.
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkMap; //객체 포인터에 네트워크 아이디를 매핑해둔 맵.
};