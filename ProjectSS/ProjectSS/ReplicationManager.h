#pragma once
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include <vector>
#include <unordered_set>

class ReplicationManager
{
public:
	static ReplicationManager& Get()
	{
		static ReplicationManager sInstance;
		return sInstance;
	}
	
	void ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects);
	void ReceiveWorld(InputMemoryBitStream& InStream);

	//객체 상태 델타 리플렠이션을 위한 3가지 함수
	void ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& InStream, GameObject* InGameObject);

	void ProcessReplicationAction(InputMemoryBitStream& InStream);

private:
	ReplicationManager() {}

	void ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& InStream);

	std::unordered_set<GameObject*> mObjectsReplicatedToMe; //월드의 객체 삭제 여부 추적을 위한 set

	//본문에선 구현했는데 싱글턴으로 하면 그럴 필요 없을듯?
	//std::shared_ptr<LinkingContext> mLinkingContext;
};