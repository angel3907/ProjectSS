#include "stdafx.h"
#include "DeliveryNotificationManager.h"

DeliveryNotificationManager::DeliveryNotificationManager(bool InShouldSendAcks, bool InShouldProceeAcks) :
mNextExpectedSequenceNumber(0),
mNextOutgoingSequenceNumber(0),
mShouldProcessAcks(InShouldProceeAcks),
mShouldSendAcks(InShouldSendAcks),
mDeliveredPacketCount(0),
mDroppedPacketCount(0),
mDispatchedPacketCount(0)
{

}

DeliveryNotificationManager::~DeliveryNotificationManager()
{
	LOG("DNM destructor. Delivery rate %d%%, Drop rate %d%%",
		(100 * mDeliveredPacketCount) / mDispatchedPacketCount,
		(100 * mDroppedPacketCount) / mDispatchedPacketCount);
}

InFlightPacket* DeliveryNotificationManager::WriteState(OutputMemoryBitStream& InOutputStream)
{
	InFlightPacket* ToRet = WriteSequenceNumber(InOutputStream);
	if (mShouldSendAcks)
	{
		WritePendingAcks(InOutputStream);
	}

	return ToRet;
}

bool DeliveryNotificationManager::ReadAndProcessState(InputMemoryBitStream& InInputStream)
{
	bool ToRet = ProcessSequenceNumber(InInputStream);
	if (mShouldProcessAcks)
	{
		ProcessAcks(InInputStream);
	}

	return ToRet;
}

void DeliveryNotificationManager::ProcessTimedOutPackets()
{
	float TimeOutTime = TimeUtil::Get().GetTimef() - kAckTimeOut;

	while (!mInFlightPackets.empty())
	{
		//��Ŷ�� ���ĵǾ� ������ �ð� �ʰ��� ��Ŷ�� �׻� �տ� ��ġ��
		const auto& NextInFlightPacket = mInFlightPackets.front();

		if (NextInFlightPacket.GetTimeDispatched() < TimeOutTime)
		{
			HandlePacketDeliveryFailure(NextInFlightPacket);
			mInFlightPackets.pop_front();
		}
		else
		{
			//���ķδ� �ð��ʰ� �ƴ�
			break;
		}
	}
}

InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber(OutputMemoryBitStream& InPacket)
{
	//������ ������ ��ȣ�� ���, ������Ŵ
	PacketSequenceNumber SequenceNumber = mNextOutgoingSequenceNumber++;
	InPacket.Write(SequenceNumber);

	LOG("I Write Sequence Number : %d", SequenceNumber);

	//���� ��Ŷ�� +1
	++mDispatchedPacketCount;
	
	//Ȯ������ ���� ��Ŷ ��Ͽ� �߰�
	if (mShouldProcessAcks)
	{
		mInFlightPackets.emplace_back(SequenceNumber); //��ȣ�� �޾� ����
		return &mInFlightPackets.back();
	}
	else
	{
		return nullptr;
	}
}

void DeliveryNotificationManager::WritePendingAcks(OutputMemoryBitStream& InPacket)
{
	//��Ƶ� Ȯ�������� ���� ���� �����Ƿ� ��Ʈ �ϳ��� �д�.
	bool HasAcks = (mPendingAcks.size() > 0);
	InPacket.Write(HasAcks);
	if (HasAcks)
	{
		//������ AckRange �ϳ��� ����Ѵ�.
		//���� ������ �� ���� ������
		mPendingAcks.front().Write(InPacket);
		mPendingAcks.pop_front();
	}
}

bool DeliveryNotificationManager::ProcessSequenceNumber(InputMemoryBitStream& InPacket)
{
	PacketSequenceNumber SequenceNumber;
	InPacket.Read(SequenceNumber);

	LOG("I Process Sequence Number : %d", SequenceNumber);

	if (SequenceNumber >= mNextExpectedSequenceNumber)
	{
		//1. ��ٸ��� ��ȣ, Ȯ������ ��� ��Ͽ� �߰��ϰ� ��Ŷ�� ó����.
		//2. ������ ����ϴ� ��ȣ���� ū ��ȣ�� �� => ��Ŷ �ս�
		//	 �߰��� ����������, �� ��Ŷ ������ ��ٸ�.
		//	 Ȯ�������� ���� �ߵ� ��Ŷ�� �۽������� �����ϰ� �������� ��.
		mNextExpectedSequenceNumber = SequenceNumber + 1;

		//Ack�� ����ϰ� �־��ٸ� ��Ͽ� �߰��Ѵ�.
		if (mShouldSendAcks)
		{
			AddPendingAck(SequenceNumber);
		}
		return true;
	}
	else
	{
		//3. ������ ����ϴ� ��ȣ���� ���� ��ȣ�� ��
		//	 ��Ŷ�� ������. ���ó��
		//	 �� ���ܻ�Ȳ���� mNextExpectedSequenceNumber�� PacketSequenceNumber�� �ִ밪�� �����ϰ�, 
		//	 ��� ������ ��ȣ�� �ּڰ��� ������ ������ ��ȣ�� �����÷εǾ� �׷� �� ����. 
		//	 �̴� 2�� ��Ȳ���� ó���ؾ� ��
		return false;
	}
}

void DeliveryNotificationManager::ProcessAcks(InputMemoryBitStream& InPacket)
{
	bool HasAcks;
	InPacket.Read(HasAcks);

	if (HasAcks)
	{
		AckRange AckRange_;
		AckRange_.Read(InPacket);

		//InFlightPacket �� ������ ��ȣ�� ���۰����� ���� �� ���з� ó����.

		//Ack�� �� ������ ���� ��ȣ
		PacketSequenceNumber NextAckdSequenceNumber = AckRange_.GetStart();
		
		//Ack�� �� ������ ������ ��ȣ + 1
		uint32_t OnePastAckdSequenceNumber = NextAckdSequenceNumber + AckRange_.GetCount();
		
		//AckRange ���� ��� ������ ��ȣ�� ���鼭 
		//& Ȯ������ �޾ƾ� �� ��Ŷ�� �� ������
		while (NextAckdSequenceNumber < OnePastAckdSequenceNumber && !mInFlightPackets.empty())
		{
			//���� Ȯ������ �޾ƾ� �Ǵ� ��Ŷ
			const auto& NextInFlightPacket = mInFlightPackets.front();
			PacketSequenceNumber NextInFlightPacketSequenceNumber = NextInFlightPacket.GetSequenceNumber();

			//���� Ȯ������ �޾ƾ� �Ǵ� ��Ŷ���� Ack�� �� ������ ��ȣ�� ũ�ٸ�
			//�� ��Ŷ�� ������ ����. 
			if (NextInFlightPacketSequenceNumber < NextAckdSequenceNumber)
			{
				//�纻�� ����� ��Ͽ��� ����
				auto CopyOfInFlightPacket = NextInFlightPacket;
				mInFlightPackets.pop_front();

				HandlePacketDeliveryFailure(CopyOfInFlightPacket);
			}
			//������ ������ ���
			else if (NextInFlightPacketSequenceNumber == NextAckdSequenceNumber)
			{
				HandlePacketDeliverySuccess(NextInFlightPacket);
				mInFlightPackets.pop_front();
				++NextAckdSequenceNumber;
			}
			//���� ������ ��ȣ ��Ŷ�� �� ��Ȳ. Ȯ�������� ���� �ʾ�(or �����Ǿ�) ���� ó���� ��
			else if (NextInFlightPacketSequenceNumber > NextAckdSequenceNumber)
			{
				//�������� ����ؼ� �˻�
				NextAckdSequenceNumber = NextInFlightPacketSequenceNumber;
			}
		}
	}
}

void DeliveryNotificationManager::AddPendingAck(PacketSequenceNumber InSequenceNumber)
{
	//���ӵ� ������ ������ ��ȣ�� �ƴ϶�� �ϳ��� �߰�.
	//���ӵ� ������� ���� �Ϳ� �ٿ���
	if (mPendingAcks.size() == 0 ||
		!mPendingAcks.back().ExtendIfShould(InSequenceNumber))
	{
		mPendingAcks.emplace_back(InSequenceNumber);
	}
}

void DeliveryNotificationManager::HandlePacketDeliveryFailure(const InFlightPacket& InInFlightPacket)
{
	++mDroppedPacketCount;
	InInFlightPacket.HandleDeliveryFailure(this);
}

void DeliveryNotificationManager::HandlePacketDeliverySuccess(const InFlightPacket& InInFlightPacket)
{
	++mDeliveredPacketCount;
	InInFlightPacket.HandleDeliverySuccess(this);
}