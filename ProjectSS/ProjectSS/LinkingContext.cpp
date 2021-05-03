#include "stdafx.h"

uint32_t LinkingContext::GetNetworkId(GameObject* InGameObject)
{
	auto It = mGameObjectToNetworkMap.find(InGameObject);
	if (It != mGameObjectToNetworkMap.end()) //end를 리턴하면 찾지 못한 것.
	{
		return It->second;
	}
	else
	{
		return 0;
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

