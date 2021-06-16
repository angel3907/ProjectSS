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

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	bool Init(uint16_t InPort);

	//��Ŷ ó��
	virtual void ProcessPacket (InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress ) = 0;
	
	//��Ŷ ����
	void SendPacket(OutputMemoryBitStream& InOutputStream, const SocketAddress& InToAddress);

	//��Ŷ �ޱ�
	void ReceiveReplicated(ReplicationManager& InReplicationManager);

	//������ ��Ŷ�� ť�� �о����
	void ReadIncomingPacketsIntoQueue();

	//ť�� �ִ� ��Ŷ ó��
	void ProcessQueuedPackets();

	//������ ��Ŷ ó��
	void ProcessInComingPacket();

	//���� ����
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