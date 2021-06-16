#include "stdafx.h"
#include "DeliveryNotificationManager.h"

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

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, const SocketAddress& InToAddress)
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

void NetworkManager::ReadIncomingPacketsIntoQueue()
{
	char PacketMem[1500];
	int PacketSize = sizeof(PacketMem);

	//버퍼 소유권을 입력 메모리 스트림에 넘김
	InputMemoryBitStream InputStream(PacketMem, PacketSize * 8);
	SocketAddress FromAddress;
	
	//읽을 게 없을때까지 읽어옴 (or 프레임당 읽어울 수 있는 최대 횟수에 도달)
	int ReceivedPacketCount = 0;
	int TotalReadByteCount = 0;
	
	while (ReceivedPacketCount < kMaxPacketPerFrameCount)
	{
		int RecvByteCount = mSocket->ReceiveFrom(PacketMem, kMaxPakcetSize, FromAddress);
		if (RecvByteCount == 0)
		{
			//읽을 게 없음
			break;
		}
		if (RecvByteCount > 0)
		{
			InputStream.ResetToCapacity(RecvByteCount);
			++ReceivedPacketCount;
			TotalReadByteCount += RecvByteCount;

			//패킷을 드롭시킬지 결정한다.
			//드롭시키지 않는 경우
			if (GetRandomFloat() >= mDropPacketChance)
			{
				//시뮬레이션하는 레이턴시에 더해서 전해줌
				float SimulatedReceivedTime = TimeUtil::Get().GetTimef() + mSimulatedLatency;
				mPacketQueue.emplace(SimulatedReceivedTime, InputStream, FromAddress);
			}
			//드롭시키는 경우
			else
			{
				LOG("Dropped Packet!", 0);
			}
		}
		else
		{
			if (RecvByteCount == -WSAECONNRESET)
			{
				HandleConnectionReset(FromAddress);
			}
		}
	}
}

void NetworkManager::ProcessQueuedPackets()
{
	//큐가 빌 때까지
	while (!mPacketQueue.empty())
	{
		//받은 패킷이 처리할 시간이 되었다면
		ReceivedPacket& NextPacket = mPacketQueue.front();
		if (TimeUtil::Get().GetTimef() > NextPacket.GetReceivedTime())
		{
			//처리하고 큐에서 제거
			ProcessPacket(NextPacket.GetPacketBuffer(), NextPacket.GetFromAddress());
			mPacketQueue.pop();
		}
		else
		{
			break;
		}
	}
}

void NetworkManager::ProcessInComingPacket()
{
	ReadIncomingPacketsIntoQueue();

	ProcessQueuedPackets();
}
