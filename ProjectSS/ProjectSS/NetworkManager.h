#pragma once
#include <stdint.h>
#include "UDPSocket.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include "OutputMemoryBitStream.h"
#include "ReplicationManager.h"

class NetworkManager
{
public:
	static const uint32_t kHelloCC = 'HELO';
	static const uint32_t kWelcomeCC = 'WLCM';
	static const uint32_t kStateCC = 'STAT';
	static const uint32_t kInputCC = 'INPT';

	NetworkManager(){}
	virtual ~NetworkManager(){}

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	bool Init(uint16_t InPort);

	//��Ŷ ó��
	virtual void ProcessPacket (InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress ) = 0;
	
	//��Ŷ ����
	void SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress);

	//��Ŷ �ޱ�
	void ReceiveReplicated(ReplicationManager& InReplicationManager);

	//������ ��Ŷ ó��
	void ProcessInComingPacket();

	//���ø����̼� ��Ŷ ����
	size_t SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager,
		ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams);

	//���� ����
	virtual void HandleConnectionReset(const SocketAddress& InFromAddress){}

protected:
	UDPSocketPtr mSocket;
};