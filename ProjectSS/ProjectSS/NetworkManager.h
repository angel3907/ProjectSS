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

	//네트워크 매니저 초기화
	bool Init(uint16_t InPort);

	//패킷 처리
	virtual void ProcessPacket (InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress ) = 0;
	
	//패킷 전송
	void SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress);

	//패킷 받기
	void ReceiveReplicated(ReplicationManager& InReplicationManager);

	//들어오는 패킷 처리
	void ProcessInComingPacket();

	//리플리케이션 패킷 전송
	size_t SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager,
		ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams);

	//연결 리셋
	virtual void HandleConnectionReset(const SocketAddress& InFromAddress){}

protected:
	UDPSocketPtr mSocket;
};