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

	//��ü ���� ��Ÿ ���ø����̼��� ���� 3���� �Լ�
	void ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	void ReplicateDestroy(OutputMemoryBitStream& InStream, int InNetworkId);

	void RPC(OutputMemoryBitStream& InStream, RPCParams* InRPCParams);
	void RMI(OutputMemoryBitStream& InStream, GameObject* InGameObject, RPCParams* InRPCParams);

	void ProcessReplicationAction(InputMemoryBitStream& InStream);

	std::vector<ReplicationCommand>& GetUnprocessedRAs() { return UnprocessedRAs; }
	void AddUnprocessedRA(ReplicationCommand& RC) { UnprocessedRAs.push_back(RC); }
	void ClearUnprocessedRAs() { UnprocessedRAs.clear(); }


private:
	void ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject);
	GameObject* ReceiveReplicatedObject(InputMemoryBitStream& InStream);

	std::unordered_set<GameObject*> mObjectsReplicatedToMe; //������ ��ü ���� ���� ������ ���� set

	std::vector<ReplicationCommand> UnprocessedRAs; //+�߰� : RA�ؾ��� ���� ������ ���� �߰�����

	//�������� �����ߴµ� �̱������� �ϸ� �׷� �ʿ� ������?
	//std::shared_ptr<LinkingContext> mLinkingContext;
};