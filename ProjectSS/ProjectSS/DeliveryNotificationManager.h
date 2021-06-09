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

	//Ack�� ������ ��ȣ�� Ack Data�� �а� ���� �Լ�
	InFlightPacket* WriteState(OutputMemoryBitStream& InOutputStream);
	bool ReadAndProcessState(InputMemoryBitStream& InInputStream);

	void ProcessTimedOutPackets();

	uint32_t GetDroppedPacketCount() const {return mDroppedPacketCount;}
	uint32_t GetDeliveredPacketCount() const { return mDeliveredPacketCount; }
	uint32_t GetDispatchedPacketCount() const { return mDispatchedPacketCount; }

	const std::deque<InFlightPacket>& GetInFlightPackets() const {return mInFlightPackets;}

private:
	//���� : ���ϵǴ� �����ʹ� �����ؼ� ���� �� ��
	InFlightPacket* WriteSequenceNumber(OutputMemoryBitStream& InPacket);
	void WritePendingAcks(OutputMemoryBitStream& InPacket);

	bool ProcessSequenceNumber(InputMemoryBitStream& InPacket);
	void ProcessAcks(InputMemoryBitStream& InPacket);

	void AddPendingAck(PacketSequenceNumber InSequenceNumber);
	void HandlePacketDeliveryFailure(const InFlightPacket& InInFlightPacket);
	void HandlePacketDeliverySuccess(const InFlightPacket& InInFlightPacket);

	//���� ���� ������ ��ȣ
	PacketSequenceNumber mNextOutgoingSequenceNumber;

	//������ �� ������ ����ϴ� ������ ��ȣ
	PacketSequenceNumber mNextExpectedSequenceNumber;

	//Ȯ������ ���� ��Ŷ ���
	std::deque<InFlightPacket> mInFlightPackets;

	//������� Ȯ�� �����
	std::deque<AckRange> mPendingAcks;

	//���� ��Ŷ ��
	uint32_t mDispatchedPacketCount = 0;
	
	//����� ���޵� ��Ŷ ��
	uint32_t mDeliveredPacketCount = 0;
	
	//������ ��Ŷ ��
	uint32_t mDroppedPacketCount = 0;

	//��Ŷ Ÿ�Ӿƿ� �ð�
	float kAckTimeOut = 5.0f;

	//Ack�� ������ �ϴ��� / ó���ؾ� �ϴ��� ����
	bool mShouldSendAcks;
	bool mShouldProcessAcks;
};