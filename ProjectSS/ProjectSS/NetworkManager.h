#pragma once
#include <stdint.h>
#include "UDPSocket.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include "OutputMemoryBitStream.h"
#include "ReplicationManager.h"
#include <queue>
#include <list>

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

	static const int kMaxPacketPerFrameCount = 10;

	NetworkManager(){}
	virtual ~NetworkManager(){}

	//네트워크 매니저 초기화
	bool Init(uint16_t InPort);

	//패킷 처리
	virtual void ProcessPacket (InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress ) = 0;
	
	//패킷 전송
	void SendPacket(OutputMemoryBitStream& InOutputStream, const SocketAddress& InToAddress);

	//패킷 받기
	void ReceiveReplicated(ReplicationManager& InReplicationManager);

	//들어오는 패킷을 큐로 읽어오기
	void ReadIncomingPacketsIntoQueue();

	//큐에 있는 패킷 처리
	void ProcessQueuedPackets();

	//들어오는 패킷 처리
	void ProcessInComingPacket();

	//연결 리셋
	virtual void HandleConnectionReset(const SocketAddress& InFromAddress){}

protected:
	UDPSocketPtr mSocket;

private:
	class ReceivedPacket
	{
	public:
		ReceivedPacket(float InReceivedTime, InputMemoryBitStream& InInputMemoryBitStream, const SocketAddress& InAddress)
		:mReceivedTime(InReceivedTime), mFromAddress(InAddress), mPacketBuffer(InInputMemoryBitStream){}

		const SocketAddress& GetFromAddress() const {return mFromAddress;}
		float GetReceivedTime() const {return mReceivedTime;}
		InputMemoryBitStream& GetPacketBuffer() {return mPacketBuffer;}

	private:
		float mReceivedTime;
		InputMemoryBitStream mPacketBuffer;
		SocketAddress mFromAddress;
	};

	std::queue <ReceivedPacket, std::list<ReceivedPacket> > mPacketQueue;

	float mDropPacketChance = 0.2f;
	float mSimulatedLatency = 0.1f;
};