#include "stdafx.h"

uint32_t LinkingContext::GetNetworkId(GameObject* InGameObject)
{
	auto It = mGameObjectToNetworkMap.find(InGameObject);
	if (It != mGameObjectToNetworkMap.end()) //end�� �����ϸ� ã�� ���� ��.
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
	if (It != mNetworkIdToGameObjectMap.end()) //end�� �����ϸ� ã�� ���� ��.
	{
		return It->second;
	}
	else
	{
		return nullptr;
	}
}

