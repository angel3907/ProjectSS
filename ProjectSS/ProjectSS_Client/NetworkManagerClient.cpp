#include "NetworkManagerClient.h"
#include "stdafx.h"

void NetworkManagerClient::SendOutgoingPackets()
{
	//��� ��Ŷ ���� / �Է�(����) ��Ŷ ����
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

	//���� ��Ŷ ó��, ���� ��Ŷ ó��
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
	//Hello �޽����� ���� �������� ����
	float time = TimeUtil::GetCurrentTime();

	if (time > mTimeOfLastHello + kTimeBetweenHellos)
	{
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::UpdateSendingInputPackets()
{
	//�Է� ��Ŷ�� ���� �������� ����
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
	//TODO : �ڿ��� ����
}

void NetworkManagerClient::HandleWelcomePacket(InputMemoryBitStream& InInputStream)
{
	//Welcome�� �ޱ� ��ٸ��� ���̶��
	if (mState == NCS_SayingHello)
	{
		//player id�� �޾Ҵٸ� ������ ���� ��
		int32_t PlayerId;
		InInputStream.Read(PlayerId);
		mPlayerId = PlayerId;
		mState = NCS_Welcomed;
		LOG("'%s' was welcomed on client as player %d", mName.c_str(), mPlayerId);
	}
}

void NetworkManagerClient::HandleStatePacket(InputMemoryBitStream& InInputStream)
{
	//TODO : ���� ��Ŷ(���ø����̼� ������) ó��
	if (mState == NCS_Welcomed)
	{

	}
}
