#include "NetworkManagerClient.h"
#include "stdafx.h"

NetworkManagerClient* NetworkManagerClient::sInstance;

NetworkManagerClient::NetworkManagerClient()
	: mState(NCS_Uninitialized)
{

}

void NetworkManagerClient::StaticInit(const SocketAddress& InServerAddress, const std::string& InName)
{
	sInstance = new NetworkManagerClient();
	return sInstance->Init(InServerAddress, InName);
}

void NetworkManagerClient::Init(const SocketAddress& InServerAddress, const std::string& InName)
{
	//클라이언트는 빈 아무 포트로나 초기화
	NetworkManager::Init(0);

	mServerAddress = InServerAddress;
	mState = NCS_SayingHello;
	mTimeOfLastHello = 0.f;
	mName = InName;
}

void NetworkManagerClient::SendOutgoingPackets()
{
	//헬로 패킷 전송 / 입력(상태) 패킷 전송
	switch (mState)
	{
	case NCS_SayingHello:
		UpdateSayingHello();
		break;
	case NCS_Welcomed:
		UpdateSendingInputPackets();
		break;
	}
}

void NetworkManagerClient::ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	//웰컴 패킷 처리, 상태 패킷 처리
	switch (PacketType)
	{
	case kWelcomeCC:
		HandleWelcomePacket(InInputStream);
		break;
	case kStateCC:
		HandleStatePacket(InInputStream);
		break;
	}
}

void NetworkManagerClient::UpdateSayingHello()
{
	//Hello 메시지를 일정 간격으로 전송
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastHello + kTimeBetweenHellos)
	{
		printf("I Send Hello Packet\n"); //TODO 삭제
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::UpdateSendingInputPackets()
{
	//입력 패킷을 일정 간격으로 전송
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastInputPacket + kTimeBetweenInputPackets)
	{
		SendInputPacket();
		mTimeOfLastInputPacket = time;
	}
}

void NetworkManagerClient::SendHelloPacket()
{
	OutputMemoryBitStream HelloPacket;
	HelloPacket.Write(kHelloCC);
	HelloPacket.Write(mName);
	
	SendPacket(HelloPacket, mServerAddress);
}

void NetworkManagerClient::SendInputPacket()
{
	//입력이 있을 때만 구현
	MoveList& MoveListValue = InputManager::Get().GetMoveList();
	if (!MoveListValue.HasMoves())
		return;

	OutputMemoryBitStream InputPacket;
	InputPacket.Write(kInputCC);

	//최근 3개의 입력조작만 전송
	int MoveCount = MoveListValue.GetMoveCount();
	int StartIndex = MoveCount > 3 ? MoveCount - 3 : 0; //뒤에서 3개까지만 보냄 //-3-1이 아닌 것 같은데..
	
	InputPacket.Write(MoveCount - StartIndex, 2); //2비트만 보냄

	for (int i = StartIndex; i < MoveCount; i++)
	{
		MoveListValue[i].Write(InputPacket);
	}

	SendPacket(InputPacket, mServerAddress);
	MoveListValue.Clear();
}

void NetworkManagerClient::HandleWelcomePacket(InputMemoryBitStream& InInputStream)
{
	//Welcome을 받길 기다리는 중이라면
	if (mState == NCS_SayingHello)
	{
		//player id를 받았다면 마중을 받은 것
		int32_t PlayerId;
		InInputStream.Read(PlayerId);
		mPlayerId = PlayerId;
		mState = NCS_Welcomed;
		LOG("'%s' was welcomed on client as player %d", mName.c_str(), mPlayerId);
	}
}

void NetworkManagerClient::HandleStatePacket(InputMemoryBitStream& InInputStream)
{
	if (mState == NCS_Welcomed)
	{
		//먼저 패킷 종류부터 받음.
		PacketType PacketTypeValue = PacketType::PT_MAX;
		InInputStream.ReadBits(&PacketTypeValue, GetRequiredBits(static_cast<uint8_t>(PacketType::PT_MAX)));

		//패킷 종류에 따라 처리 함수를 부름
		switch (PacketTypeValue)
		{
		case PacketType::PT_Hello:
			break;
		case PacketType::PT_ReplicationData:
			mReplicationManagerClient.ProcessReplicationAction(InInputStream);
			break;
		case PacketType::PT_Disconnect:
			break;
		case PacketType::PT_MAX:
			break;
		default:
			break;
		}
	}
}
