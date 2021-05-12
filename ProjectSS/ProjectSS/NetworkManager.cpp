#include "stdafx.h"

bool NetworkManager::Init(uint16_t InPort)
{
	//�޾ƿ� ��Ʈ��ȣ�� �� ��ǻ�� IP�� ���Ͽ� ���ε�
	mSocket = SocketUtil::CreateUDPSocket(INET);
	SocketAddress OwnAddress(INADDR_ANY, InPort);
	mSocket->Bind(OwnAddress);

	LOG("Initializing NetworkManager at port %d", InPort);

	//���ε尡 ����� �ƴ��� Ȯ��
	if (mSocket == nullptr)
	{
		return false;
	}

	//�� ���ŷ ��� ����
	if (mSocket->SetNonBlockingMode(true) != NO_ERROR)
	{
		return false;
	}

	//����� �����ƴٸ� true ����
	return true;
}

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress)
{
	int32_t SentByteCount = mSocket->SendTo(InOutputStream.GetBufferPtr(), InOutputStream.GetByteLength(), InToAddress);
	//TODO : ���� ����Ʈ ���� ���� ó���� ���߿�
}

int32_t NetworkManager::SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager, 
									ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams)
{
	OutputMemoryBitStream Stream;
	
	//���� ��Ŷ�̶�� ǥ��
	Stream.Write(kStateCC);

	//���ø����̼ǿ��̶�� �̸� ǥ��
	Stream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));

	switch (InReplicationAction)
	{
	case ReplicationAction::RA_Create:
		InReplicationManager.ReplicateCreate(Stream, InGameObject);
		break;
	case ReplicationAction::RA_Update:
		InReplicationManager.ReplicateUpdate(Stream, InGameObject);
		break;
	case ReplicationAction::RA_Destroy:
		InReplicationManager.ReplicateDestroy(Stream, InGameObject);
		break;
	case ReplicationAction::RA_RPC:
		InReplicationManager.RPC(Stream, InRPCParams);
		break;
	case ReplicationAction::RA_RMI:
		InReplicationManager.RMI(Stream, InGameObject, InRPCParams);
		break;
	default:
		break;
	}

	size_t SentByteCount = mSocket->SendTo(Stream.GetBufferPtr(), Stream.GetByteLength(), InToAddress);
	return SentByteCount;
}