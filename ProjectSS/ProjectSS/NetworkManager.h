#pragma once
#include <stdint.h>
#include "UDPSocket.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include "OutputMemoryBitStream.h"
#include "ReplicationManager.h"

class DeliveryNotificationManager;

enum class NoAdmittanceReason : uint8_t
{
	NONE,
	FULL_PLAYER,
	GAME_STARTED
};

enum ReadyPacketType : uint8_t
{
	READY_ACTIVE,
	READY_NONACTIVE,
	READY_SEND,
	READY_ACK,
	START,
	END
};

class NetworkManager
{
public:
	static const uint32_t kHelloCC = 'HELO';
	static const uint32_t kWelcomeCC = 'WLCM';
	static const uint32_t kStateCC = 'STAT';
	static const uint32_t kInputCC = 'INPT';
	static const uint32_t kNoAdmittanceCC = 'NOAD';
	static const uint32_t kReadyCC = 'REDY';

	NetworkManager(){}
	virtual ~NetworkManager(){}

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	bool Init(uint16_t InPort);

	//��Ŷ ó��
	virtual void ProcessPacket (InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress ) = 0;
	
	//��Ŷ ����
	void SendPacket(OutputMemoryBitStream& InOutputStream, const SocketAddress& InToAddress);

	//��Ŷ �ޱ�
	void ReceiveReplicated(ReplicationManager& InReplicationManager);

	//������ ��Ŷ ó��
	void ProcessInComingPacket();

	//���ø����̼� ��Ŷ ����
	size_t SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager,
		DeliveryNotificationManager* InDeliveryNotificationManager,
		ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams);

	//���� ����
	virtual void HandleConnectionReset(const SocketAddress& InFromAddress){}

protected:
	UDPSocketPtr mSocket;
};