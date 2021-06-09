#pragma once
#include <string>
#include "NetworkManager.h"
#include "SocketAddress.h"
#include "ReplicationManager.h"
#include "Player.h"
#include "DeliveryNotificationManager.h"

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
	static NetworkManagerClient* sInstance;
	virtual ~NetworkManagerClient() {}

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ	
	static void StaticInit (const SocketAddress& InServerAddress, const std::string& InName);

	//��Ŷ ����
	void SendOutgoingPackets();

	//��Ŷ ó��
	virtual void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress) override;

	//�÷��̾� ���̵� ������
	int32_t GetPlayerId() const { return mPlayerId; }

	//���� ���� �ּ� ����, �÷��̾� �̸� ����, ���� ����
	void SetServerSocketAddress(const SocketAddress& InServerAddress) {mServerAddress = InServerAddress;}
	void SetPlayerName(std::string InName){ mName = InName; }
	void SetPlayerColor(PlayerColor InPlayerColor) { mPlayerColor  = InPlayerColor;}

	void SendReadyPacket(ReadyPacketType InReadyPacketType);

private:
	NetworkManagerClient();
	void Init (const SocketAddress& InServerAddress, const std::string& InName); //���� �ʱ�ȭ �Լ�

	//��� ��Ŷ / �Է� ��Ŷ ����
	void UpdateSayingHello();
	void UpdateSendingInputPackets();

	void SendHelloPacket();
	void SendInputPacket();

	//���� ��Ŷ / ���� (���ø����̼�) ��Ŷ ó��
	void HandleWelcomePacket(InputMemoryBitStream& InInputStream);
	void HandleNoAdmittancePacket(InputMemoryBitStream& InInputStream);
	void HandleStatePacket(InputMemoryBitStream& InInputStream);
	void HandleReadyPacket(InputMemoryBitStream& InInputStream);

private:
	//��� ���� ������
	DeliveryNotificationManager	mDeliveryNotificationManager;
	//���� ���ø����̼� �Ŵ���
	ReplicationManager mReplicationManagerClient;
	
	//��� / �Է� ��Ŷ ������ �ֱ� ����
	float mTimeOfLastHello = 0;
	float mTimeOfLastInputPacket = 0;
	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;

	//Ŭ���̾�Ʈ ����
	NetworkClientState mState = NCS_Uninitialized;

	//���� �ּ�
	SocketAddress mServerAddress;

	//�÷��̾� �̸��� ���̵� ����
	std::string mName;
	int32_t mPlayerId;
	PlayerColor mPlayerColor;
};
