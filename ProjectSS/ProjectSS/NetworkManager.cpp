#include "stdafx.h"
#include "DeliveryNotificationManager.h"

const uint32_t kMaxPakcetSize = 1470;

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

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, const SocketAddress& InToAddress)
{
	int32_t SentByteCount = mSocket->SendTo(InOutputStream.GetBufferPtr(), InOutputStream.GetByteLength(), InToAddress);
	//TODO : ���� ����Ʈ ���� ���� ó���� ���߿�
}

void NetworkManager::ReceiveReplicated(ReplicationManager& InReplicationManager)
{
	//�ӽù��۷� �����͸� �ް�
	char* TempBuffer = static_cast<char*>(new char[kMaxPakcetSize]);
	SocketAddress FromAddress;
	size_t RecvByteCount = mSocket->ReceiveFrom(TempBuffer, kMaxPakcetSize, FromAddress);

	if (RecvByteCount > 0)
	{
		//���� �������� �Է� �޸� ��Ʈ���� �ѱ�
		//���� �����Ϳ��Ҹ� �ϳ��� ������ ������� ���� �� ����.
		InputMemoryBitStream Stream(TempBuffer, static_cast<uint32_t>(RecvByteCount) << 3);

		//���� ��Ŷ �������� ����.
		PacketType PacketTypeValue = PacketType::PT_MAX;
		Stream.ReadBits(&PacketTypeValue, GetRequiredBits(static_cast<uint8_t>(PacketType::PT_MAX)));

		//��Ŷ ������ ���� ó�� �Լ��� �θ�
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
		//printf("Process Incoming Packet\n");
		//���� �������� �Է� �޸� ��Ʈ���� �ѱ�
		//���� �����Ϳ��Ҹ� �ϳ��� ������ ������� ���� �� ����.
		InputMemoryBitStream Stream(TempBuffer, static_cast<uint32_t>(RecvByteCount) << 3);
		ProcessPacket(Stream, FromAddress);
	}
	else
	{
		if (RecvByteCount == -WSAECONNRESET)
		{
			HandleConnectionReset(FromAddress);
		}

		delete[] TempBuffer;
	}
}