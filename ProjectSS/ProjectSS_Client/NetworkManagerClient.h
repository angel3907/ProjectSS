#pragma once
#include <string>
#include "NetworkManager.h"
#include "SocketAddress.h"

class InputMemoryBitStream;

class NetworkManagerClient : public NetworkManager
{
	//Ŭ���̾�Ʈ ���� ����
	enum NetworkClientState
	{
		NCS_Uninitialized, //�ʱ�ȭ���� ����
		NCS_SayingHello, //��� ��Ŷ�� ������ ��
		NCS_Welcomed //���� ��Ŷ�� ���� ����
	};

public:
	NetworkManagerClient(){}
	virtual ~NetworkManagerClient(){}
	
	//��Ŷ�� ������ �޴� �ڵ�
	void SendOutgoingPackets();
	void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);

	void UpdateSayingHello();
	void UpdateSendingInputPackets();

	void SendHelloPacket();
	void SendInputPacket();

	void HandleWelcomePacket(InputMemoryBitStream& InInputStream);
	void HandleStatePacket(InputMemoryBitStream& InInputStream);

private:
	NetworkClientState mState = NCS_Uninitialized;
	float mTimeOfLastHello = 0;
	float mTimeOfLastInputPacket = 0;

	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;

	SocketAddress mServerAddress;
	string mName;
	int32_t mPlayerId;
};
