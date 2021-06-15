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
		//패킷은 정렬되어 있으니 시간 초과된 패킷은 항상 앞에 위치함
		const auto& NextInFlightPacket = mInFlightPackets.front();

		if (NextInFlightPacket.GetTimeDispatched() < TimeOutTime)
		{
			HandlePacketDeliveryFailure(NextInFlightPacket);
			mInFlightPackets.pop_front();
		}
		else
		{
			//이후로는 시간초과 아님
			break;
		}
	}
}

InFlightPacket* DeliveryNotificationManager::WriteSequenceNumber(OutputMemoryBitStream& InPacket)
{
	//다음번 시퀀스 번호를 기록, 증가시킴
	PacketSequenceNumber SequenceNumber = mNextOutgoingSequenceNumber++;
	InPacket.Write(SequenceNumber);

	LOG("I Write Sequence Number : %d", SequenceNumber);

	//보낸 패킷수 +1
	++mDispatchedPacketCount;
	
	//확인응답 받을 패킷 목록에 추가
	if (mShouldProcessAcks)
	{
		mInFlightPackets.emplace_back(SequenceNumber); //번호를 받아 생성
		return &mInFlightPackets.back();
	}
	else
	{
		return nullptr;
	}
}

void DeliveryNotificationManager::WritePendingAcks(OutputMemoryBitStream& InPacket)
{
	//모아둔 확인응답이 없을 수도 있으므로 비트 하나를 둔다.
	bool HasAcks = (mPendingAcks.size() > 0);
	InPacket.Write(HasAcks);
	if (HasAcks)
	{
		//있으면 AckRange 하나만 기록한다.
		//제일 오래된 걸 먼저 보낸다
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
		//1. 기다리던 번호, 확인응답 대기 목록에 추가하고 패킷을 처리함.
		//2. 다음에 기대하던 번호보다 큰 번호가 옴 => 패킷 손실
		//	 중간건 누락됐으니, 이 패킷 다음걸 기다림.
		//	 확인응답이 빠진 중도 패킷은 송신측에서 인지하고 재전송할 것.
		mNextExpectedSequenceNumber = SequenceNumber + 1;

		//Ack를 기대하고 있었다면 목록에 추가한다.
		if (mShouldSendAcks)
		{
			AddPendingAck(SequenceNumber);
		}
		return true;
	}
	else
	{
		//3. 다음에 기대하던 번호보다 작은 번호가 옴
		//	 패킷이 누락됨. 폐기처리
		//	 단 예외상황으로 mNextExpectedSequenceNumber가 PacketSequenceNumber의 최대값에 근접하고, 
		//	 방금 수신한 번호가 최솟값에 가까우면 시퀀스 번호가 오버플로되어 그럴 수 있음. 
		//	 이는 2번 상황으로 처리해야 함
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

		//InFlightPacket 중 시퀀스 번호가 시작값보다 작은 건 실패로 처리함.

		//Ack로 온 시퀀스 시작 번호
		PacketSequenceNumber NextAckdSequenceNumber = AckRange_.GetStart();
		
		//Ack로 온 마지막 시퀀스 번호 + 1
		uint32_t OnePastAckdSequenceNumber = NextAckdSequenceNumber + AckRange_.GetCount();
		
		//AckRange 내의 모든 시퀀스 번호를 돌면서 
		//& 확인응답 받아야 할 피킷이 빌 때까지
		while (NextAckdSequenceNumber < OnePastAckdSequenceNumber && !mInFlightPackets.empty())
		{
			//다음 확인응답 받아야 되는 패킷
			const auto& NextInFlightPacket = mInFlightPackets.front();
			PacketSequenceNumber NextInFlightPacketSequenceNumber = NextInFlightPacket.GetSequenceNumber();

			//다음 확인응답 받아야 되는 패킷보다 Ack로 온 시퀀스 번호가 크다면
			//이 패킷은 누락된 것임. 
			if (NextInFlightPacketSequenceNumber < NextAckdSequenceNumber)
			{
				//사본을 만들고 목록에서 제거
				auto CopyOfInFlightPacket = NextInFlightPacket;
				mInFlightPackets.pop_front();

				HandlePacketDeliveryFailure(CopyOfInFlightPacket);
			}
			//상대방이 수신한 경우
			else if (NextInFlightPacketSequenceNumber == NextAckdSequenceNumber)
			{
				HandlePacketDeliverySuccess(NextInFlightPacket);
				mInFlightPackets.pop_front();
				++NextAckdSequenceNumber;
			}
			//이전 시퀀스 번호 패킷이 온 상황. 확인응답이 오지 않아(or 지연되어) 누락 처리된 것
			else if (NextInFlightPacketSequenceNumber > NextAckdSequenceNumber)
			{
				//나머지를 계속해서 검사
				NextAckdSequenceNumber = NextInFlightPacketSequenceNumber;
			}
		}
	}
}

void DeliveryNotificationManager::AddPendingAck(PacketSequenceNumber InSequenceNumber)
{
	//연속된 범위의 시퀀스 번호가 아니라면 하나만 추가.
	//연속된 범위라면 기존 것에 붙여줌
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