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

	//��ü ���� ��Ÿ ���Î��̼��� ���� 3���� �Լ�
	void ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& InStream, GameObject* InGameObject);

	void ProcessReplicationAction(InputMemoryBitStream& InStream);

private:
	ReplicationManager() {}

	void ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& InStream);

	std::unordered_set<GameObject*> mObjectsReplicatedToMe; //������ ��ü ���� ���� ������ ���� set

	//�������� �����ߴµ� �̱������� �ϸ� �׷� �ʿ� ������?
	//std::shared_ptr<LinkingContext> mLinkingContext;
};