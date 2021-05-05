#include "stdafx.h"
//������ ����ȭ�� ������ ��Ʈ ���� ������ Ÿ�ӿ� ������
constexpr int32_t GetRequiredBits(int32_t Value, int Bits = 0)
{
	return Value ? GetRequiredBits(Value >> 1, Bits + 1) : Bits;
}

//��� ��Ʈ���� ��ü �÷����� ���ø����̼� ����
void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects)
{
	//���ø����̼ǿ��̶�� �̸� ǥ��
	InStream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));

	//�� ��ü�� �ϳ��� ���
	for(GameObject* Go : InAllObjects)
	{
		ReplicateIntoStream(InStream, Go);
	}
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& InStream)
{
	std::unordered_set<GameObject*> ReceivedObjects;

	//��Ʈ���� ��� �����͸� �� ���� ������
	while (InStream.GetRemainingBitCount() > 0)
	{
		GameObject* ReceivedGameObject = ReceiveReplicatedObject(InStream);
		ReceivedObjects.insert(ReceivedGameObject);
	}

	//���� mObjectsReplicatedToMe ������ ��ȸ�ϸ�
	//���� ���ø����̼� ���� ���տ��� ������ ��ü�� ������ ����
	for (GameObject* Go : mObjectsReplicatedToMe)
	{
		//�����Ǿ��ٸ� ����
		if (ReceivedObjects.find(Go) == ReceivedObjects.end())
		{
			LinkingContext::Get().RemoveGameObject(Go);
			delete Go; //TODO? ���߿� Destroy �Լ� ���� �ɷ� ��ü�ؾ� �ұ�?
		}
	}

	mObjectsReplicatedToMe = ReceivedObjects;
}

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	//1. ���� ��ü ��Ʈ��ũ id ���
	InStream.Write(LinkingContext::Get().GetNetworkId(InGameObject, true));

	//2. ���� ��ü Ŭ���� �ĺ��� ���
	InStream.Write(InGameObject->GetClassId());

	//3. ��ü ������ ���
	InGameObject->Write(InStream);
}

GameObject* ReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& InStream)
{
	uint32_t NetworkId = 0;
	uint32_t ClassId = 0;
	InStream.Read(NetworkId);
	InStream.Read(ClassId);

	//���ӿ�����Ʈ �������� ������ ����
	GameObject* Go = LinkingContext::Get().GetGameObject(NetworkId);
	if (Go == nullptr)
	{
		Go = ObjectCreationRegistry::Get().CreateGameObject(ClassId);
		LinkingContext::Get().AddGameObject(Go, NetworkId); //�ʿ��� �߰�
	}

	//��ü�� ��Ʈ������ �о����
	Go->Read(InStream);

	//���� ���� ������ ���� ������ ��ü�� ����
	return Go;
}