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
	//Ŭ���̾�Ʈ�� �� �ƹ� ��Ʈ�γ� �ʱ�ȭ
	NetworkManager::Init(0);

	mServerAddress = InServerAddress;
	mState = NCS_SayingHello;
	mTimeOfLastHello = 0.f;
	mName = InName;
}

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
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastHello + kTimeBetweenHellos)
	{
		printf("I Send Hello Packet\n"); //TODO ����
		SendHelloPacket();
		mTimeOfLastHello = time;
	}
}

void NetworkManagerClient::UpdateSendingInputPackets()
{
	//�Է� ��Ŷ�� ���� �������� ����
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
	//�Է��� ���� ���� ����
	MoveList& MoveListValue = InputManager::Get().GetMoveList();
	if (!MoveListValue.HasMoves())
		return;

	OutputMemoryBitStream InputPacket;
	InputPacket.Write(kInputCC);

	//�ֱ� 3���� �Է����۸� ����
	int MoveCount = MoveListValue.GetMoveCount();
	int StartIndex = MoveCount > 3 ? MoveCount - 3 : 0; //�ڿ��� 3�������� ���� //-3-1�� �ƴ� �� ������..
	
	InputPacket.Write(MoveCount - StartIndex, 2); //2��Ʈ�� ����

	for (int i = StartIndex; i < MoveCount; i++)
	{
		MoveListValue[i].Write(InputPacket);
	}

	SendPacket(InputPacket, mServerAddress);
	MoveListValue.Clear();
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
	if (mState == NCS_Welcomed)
	{
		//���� ��Ŷ �������� ����.
		PacketType PacketTypeValue = PacketType::PT_MAX;
		InInputStream.ReadBits(&PacketTypeValue, GetRequiredBits(static_cast<uint8_t>(PacketType::PT_MAX)));

		//��Ŷ ������ ���� ó�� �Լ��� �θ�
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
