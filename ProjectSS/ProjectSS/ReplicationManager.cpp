#include "stdafx.h"

//��� ��Ʈ���� ��ü �÷����� ���ø����̼� ����
void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects)
{
	//���ø����̼ǿ��̶�� �̸� ǥ��
	InStream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));
	
	//�� ��ü�� �ϳ��� ���
	for (int i = 0; i < InAllObjects.size(); i++)
	{ 
		GameObject* Go = InAllObjects[i];
		ReplicateIntoStream(InStream, Go);

		//+ ���ӿ�����Ʈ �ϳ��� ��ٸ�, �� ��Ʈ�� �� ���� (��Ŷ�� ������������ ��Ÿ��)
		bool bEndOfPacket = false;
		
		if (i == InAllObjects.size() - 1)
		{
			bEndOfPacket = true;
		}

		InStream.Write(bEndOfPacket);
	}
	
	printf("Real Send Cnt %d\n", InStream.GetBitLength());
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& InStream)
{
	std::unordered_set<GameObject*> ReceivedObjects;

	printf("Rcv Bit Count is %d\n", InStream.GetRemainingBitCount());
	//��Ʈ���� ��� �����͸� �� ���� ������
	while (InStream.GetRemainingBitCount() > 0)
	{
		GameObject* ReceivedGameObject = ReceiveReplicatedObject(InStream);
		ReceivedObjects.insert(ReceivedGameObject);

		//+ ���ӿ�����Ʈ �ϳ��� �о�Դٸ� �� ��Ʈ�� �� �о�� (��Ŷ�� ������������ ��Ÿ��)
		bool bEndOfPacket;
		InStream.Read(bEndOfPacket);

		if (bEndOfPacket)
		{
			break;
		}
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

void ReplicationManager::ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	//���ø����̼� ��� ���� ������ �������� �������
	ReplicationHeader Rh(ReplicationAction::RA_Create, LinkingContext::Get().GetNetworkId(InGameObject, true), InGameObject->GetClassId());
	Rh.Write(InStream);

	InGameObject->Write(InStream);
	//�κ� ���ø����̼�
	//InGameObject->WriteChanged(InStream);
}

void ReplicationManager::ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	ReplicationHeader Rh(ReplicationAction::RA_Update, LinkingContext::Get().GetNetworkId(InGameObject, false), InGameObject->GetClassId());
	Rh.Write(InStream);

	InGameObject->Write(InStream);
	//�κ� ���ø����̼�
	//InGameObject->WriteChanged(InStream);
}

void ReplicationManager::ReplicateDestroy(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	ReplicationHeader Rh(ReplicationAction::RA_Destroy, LinkingContext::Get().GetNetworkId(InGameObject, false));
	Rh.Write(InStream);
}

void ReplicationManager::RPC(OutputMemoryBitStream& InStream, RPCParams* InRPCParams)
{
	ReplicationHeader Rh(ReplicationAction::RA_RPC);
	Rh.Write(InStream);
	InStream.Write(InRPCParams->GetRPCId());

	RPCManager::Get().ProcessRPC(InStream, InRPCParams);
}

void ReplicationManager::RMI(OutputMemoryBitStream& InStream, GameObject* InGameObject, RPCParams* InRPCParams)
{
	ReplicationHeader Rh(ReplicationAction::RA_RMI, LinkingContext::Get().GetNetworkId(InGameObject, false));
	Rh.Write(InStream);
	InStream.Write(InRPCParams->GetRPCId());

	RPCManager::Get().ProcessRMI(InStream, InRPCParams, InGameObject);
}

void ReplicationManager::ProcessReplicationAction(InputMemoryBitStream& InStream)
{
	ReplicationHeader Rh;
	Rh.Read(InStream);

	switch (Rh.mReplicationAction)
	{
		case ReplicationAction::RA_Create:
		{
			GameObject* Go = ObjectCreationRegistry::Get().CreateGameObject(Rh.mClassId);
			LinkingContext::Get().AddGameObject(Go, Rh.mNetworkId);
			Go->SetNetworkId(Rh.mNetworkId);
			
			Go->Read(InStream);
			//�κ� ���ø����̼�
			//Go->ReadChanged(InStream);
		}
		break;
		case ReplicationAction::RA_Update:
		{
			GameObject* Go = LinkingContext::Get().GetGameObject(Rh.mNetworkId);
			if (Go)
			{
				Go->Read(InStream);
				//�κ� ���ø����̼�
				//Go->ReadChanged(InStream);
			}
			else
			{
				//���� ������ ���� ���� ���� �� ����.
				//�׷��Ƿ� ���� ��ü�� ����� ���� ���� �����
				uint32_t ClassId = Rh.mClassId;
				Go = ObjectCreationRegistry::Get().CreateGameObject(ClassId);
				Go->Read(InStream);
				//�κ� ���ø����̼�
				//Go->ReadChanged(InStream);
				delete Go;
			}
		}
		break;
		case ReplicationAction::RA_Destroy:
		{
			GameObject* Go = LinkingContext::Get().GetGameObject(Rh.mNetworkId);
			if (Go)
			{ 
				LinkingContext::Get().RemoveGameObject(Go);
				delete Go;
			}
		}
		break;
		case ReplicationAction::RA_RPC:
		{
			RPCManager::Get().ProcessRPC(InStream);
		}
		break;
		case ReplicationAction::RA_RMI:
		{	
			RPCManager::Get().ProcessRMI(InStream, Rh.mNetworkId);
		}
		default:
		//�� ������ ���⼭ ó�� �� ��.
		break;
	}
}

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	//1. ���� ��ü ��Ʈ��ũ id ���
	InStream.Write(LinkingContext::Get().GetNetworkId(InGameObject, true));

	//2. ���� ��ü Ŭ���� �ĺ��� ���
	uint32_t ClassId = InGameObject->GetClassId();
	InStream.Write(ClassId);

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