#include "stdafx.h"

const uint32_t kMaxPakcetSize = 1470;

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

void NetworkManager::ReceiveReplicated(ReplicationManager& InReplicationManager)
{
	//임시버퍼로 데이터를 받고
	char* TempBuffer = static_cast<char*>(new char[kMaxPakcetSize]);
	SocketAddress FromAddress;
	size_t RecvByteCount = mSocket->ReceiveFrom(TempBuffer, kMaxPakcetSize, FromAddress);

	if (RecvByteCount > 0)
	{
		//버퍼 소유권을 입력 메모리 스트림에 넘김
		//이제 데이터원소를 하나씩 쓰여진 순서대로 읽을 수 있음.
		InputMemoryBitStream Stream(TempBuffer, static_cast<uint32_t>(RecvByteCount) << 3);

		//먼저 패킷 종류부터 받음.
		PacketType PacketTypeValue = PacketType::PT_MAX;
		Stream.ReadBits(&PacketTypeValue, GetRequiredBits(static_cast<uint8_t>(PacketType::PT_MAX)));

		//패킷 종류에 따라 처리 함수를 부름
		switch (PacketTypeValue)
		{
		case PacketType::PT_Hello:
			break;
		case PacketType::PT_ReplicationData:
			InReplicationManager.ProcessReplicationAction(Stream);
			break;
		case PacketType::PT_Disconnect:
			break;
		case PacketType::PT_MAX:
			break;
		default:
			break;
		}
	}
	else
	{
		delete[] TempBuffer;
	}

}

void NetworkManager::ProcessInComingPacket()
{
	char* TempBuffer = static_cast<char*>(new char[kMaxPakcetSize]);
	SocketAddress FromAddress;
	size_t RecvByteCount = mSocket->ReceiveFrom(TempBuffer, kMaxPakcetSize, FromAddress);

	if (RecvByteCount > 0)
	{
		printf("Process Incoming Packet\n");
		//버퍼 소유권을 입력 메모리 스트림에 넘김
		//이제 데이터원소를 하나씩 쓰여진 순서대로 읽을 수 있음.
		InputMemoryBitStream Stream(TempBuffer, static_cast<uint32_t>(RecvByteCount) << 3);
		ProcessPacket(Stream, FromAddress);
	}
	else
	{
		delete[] TempBuffer;
	}
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