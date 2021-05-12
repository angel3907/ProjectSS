#include "stdafx.h"

bool NetworkManager::Init(uint16_t InPort)
{
	//받아온 포트번호와 이 컴퓨터 IP로 소켓에 바인딩
	mSocket = SocketUtil::CreateUDPSocket(INET);
	SocketAddress OwnAddress(INADDR_ANY, InPort);
	mSocket->Bind(OwnAddress);

	LOG("Initializing NetworkManager at port %d", InPort);

	//바인드가 제대로 됐는지 확인
	if (mSocket == nullptr)
	{
		return false;
	}

	//논 블로킹 모드 설정
	if (mSocket->SetNonBlockingMode(true) != NO_ERROR)
	{
		return false;
	}

	//제대로 설정됐다면 true 리턴
	return true;
}

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress)
{
	int32_t SentByteCount = mSocket->SendTo(InOutputStream.GetBufferPtr(), InOutputStream.GetByteLength(), InToAddress);
	//TODO : 보낸 바이트 수에 대한 처리는 나중에
}

int32_t NetworkManager::SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager, 
									ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams)
{
	OutputMemoryBitStream Stream;
	
	//상태 패킷이라고 표시
	Stream.Write(kStateCC);

	//리플리케이션용이라고 미리 표시
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