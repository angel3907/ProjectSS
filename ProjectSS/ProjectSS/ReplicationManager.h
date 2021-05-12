#pragma once
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include <vector>
#include <unordered_set>
#include "RPCManager.h"

class ReplicationManager
{
public:
	ReplicationManager() {}
	
	void ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects);
	void ReceiveWorld(InputMemoryBitStream& InStream);

	//객체 상태 델타 리플리케이션을 위한 3가지 함수
	void ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& InStream, GameObject* InGameObject);

	void RPC(OutputMemoryBitStream& InStream, RPCParams* InRPCParams);
	void RMI(OutputMemoryBitStream& InStream, GameObject* InGameObject, RPCParams* InRPCParams);

	void ProcessReplicationAction(InputMemoryBitStream& InStream);

private:
	void ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& InStream);

	std::unordered_set<GameObject*> mObjectsReplicatedToMe; //월드의 객체 삭제 여부 추적을 위한 set

	//본문에선 구현했는데 싱글턴으로 하면 그럴 필요 없을듯?
	//std::shared_ptr<LinkingContext> mLinkingContext;
};