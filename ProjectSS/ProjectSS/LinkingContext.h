#pragma once
#include <stdint.h>
#include "GameObject.h"
#include <unordered_map>
#include <unordered_set>
#include "ReplicationHeader.h"

struct ReplicationCommand
{
	int32_t NerworkId;
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

	std::unordered_set<GameObject*>& GetGameObjectSet(){return GameObjects;}

	vector<ReplicationCommand>& GetUnprocessedRAs() {return UnprocessedRAs;}
	void AddUnprocessedRA(ReplicationCommand& RC) {UnprocessedRAs.push_back(RC);}
	void ClearUnprocessedRAs() {UnprocessedRAs.clear(); }

private:
	LinkingContext() : mNextNetworkId(1) {}

	std::unordered_map<uint32_t, GameObject*> mNetworkIdToGameObjectMap; //네트워크 아이디에 해당 객체 포인터를 매핑해둔 맵.
	std::unordered_map<GameObject*, uint32_t> mGameObjectToNetworkMap; //객체 포인터에 네트워크 아이디를 매핑해둔 맵.

	std::unordered_set<GameObject*> GameObjects;

	vector<ReplicationCommand> UnprocessedRAs; //+추가 : RA해야할 일이 있으면 여기 추가해줌

	uint32_t mNextNetworkId; //다음에 객체에 할당할 네트워크 아이디
};