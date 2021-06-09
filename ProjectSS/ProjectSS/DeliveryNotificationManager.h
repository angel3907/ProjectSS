#pragma once
#include <stdint.h>
#include "InFlightPacket.h"
#include "AckRange.h"
#include <deque>
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"

class DeliveryNotificationManager
{
public:
	DeliveryNotificationManager(bool InShouldSendAcks, bool InShouldProceeAcks);
	~DeliveryNotificationManager();

	//Ack의 시퀀스 번호와 Ack Data를 읽고 쓰는 함수
	InFlightPacket* WriteState(OutputMemoryBitStream& InOutputStream);
	bool ReadAndProcessState(InputMemoryBitStream& InInputStream);

	void ProcessTimedOutPackets();

	uint32_t GetDroppedPacketCount() const {return mDroppedPacketCount;}
	uint32_t GetDeliveredPacketCount() const { return mDeliveredPacketCount; }
	uint32_t GetDispatchedPacketCount() const { return mDispatchedPacketCount; }

	const std::deque<InFlightPacket>& GetInFlightPackets() const {return mInFlightPackets;}

private:
	//주의 : 리턴되는 포인터는 보관해서 쓰지 말 것
	InFlightPacket* WriteSequenceNumber(OutputMemoryBitStream& InPacket);
	void WritePendingAcks(OutputMemoryBitStream& InPacket);

	bool ProcessSequenceNumber(InputMemoryBitStream& InPacket);
	void ProcessAcks(InputMemoryBitStream& InPacket);

	void AddPendingAck(PacketSequenceNumber InSequenceNumber);
	void HandlePacketDeliveryFailure(const InFlightPacket& InInFlightPacket);
	void HandlePacketDeliverySuccess(const InFlightPacket& InInFlightPacket);

	//다음 나갈 시퀀스 번호
	PacketSequenceNumber mNextOutgoingSequenceNumber;

	//다음에 올 것으로 기대하는 시퀀스 번호
	PacketSequenceNumber mNextExpectedSequenceNumber;

	//확인응답 받을 패킷 목록
	std::deque<InFlightPacket> mInFlightPackets;

	//대기중인 확인 응답들
	std::deque<AckRange> mPendingAcks;

	//보낸 패킷 수
	uint32_t mDispatchedPacketCount = 0;
	
	//제대로 전달된 패킷 수
	uint32_t mDeliveredPacketCount = 0;
	
	//누락된 패킷 수
	uint32_t mDroppedPacketCount = 0;

	//패킷 타임아웃 시간
	float kAckTimeOut = 5.0f;

	//Ack를 보내야 하는지 / 처리해야 하는지 여부
	bool mShouldSendAcks;
	bool mShouldProcessAcks;
};