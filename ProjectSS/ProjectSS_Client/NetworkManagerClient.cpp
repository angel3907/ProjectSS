#include "NetworkManagerClient.h"
#include "stdafx.h"

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
	float time = TimeUtil::GetCurrentTime();

	if (time > mTimeOfLastHello + kTimeBetweenHellos)
	{
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::UpdateSendingInputPackets()
{
	//입력 패킷을 일정 간격으로 전송
	float time = TimeUtil::GetCurrentTime();

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
	//TODO : 상태 패킷(리플리케이션 데이터) 처리
	if (mState == NCS_Welcomed)
	{

	}
}
