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

private:
	ReplicationManager() {}

	void ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& InStream);

	std::unordered_set<GameObject*> mObjectsReplicatedToMe; //������ ��ü ���� ���� ������ ���� set

	//�������� �����ߴµ� �̱������� �ϸ� �׷� �ʿ� ������?
	//std::shared_ptr<LinkingContext> mLinkingContext;
};