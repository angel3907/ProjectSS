#pragma once
#include <unordered_map>
#include "TransmissionData.h"
class DeliveryNoficationManager;

//비트를 더 적게 또는 많이 사용해야 한다면 타입을 바꿔야 할 것.
typedef uint16_t PacketSequenceNumber;

class InFlightPacket
{
public:
	InFlightPacket(PacketSequenceNumber InSequenceNumber);
	~InFlightPacket(){}

	PacketSequenceNumber GetSequenceNumber() const {return mSequenceNumber;}
	float GetTimeDispatched() const {return mTimeDispatched;}

	void SetTransmissionData(int InKey, TransmissionDataPtr InTransmissionData)
	{
		mTransmissionDataMap[InKey] = InTransmissionData;
	}
	
	const TransmissionDataPtr GetTransmissionData(int InKey) const
	{
		auto It = mTransmissionDataMap.find(InKey);
		return (It != mTransmissionDataMap.end()) ? It->second : nullptr;
	}

	void HandleDeliveryFailure(DeliveryNoficationManager* InDeliveryNotificationManager) const;
	void HandleDeliverySuccess(DeliveryNoficationManager* InDeliveryNotificationManager) const;

private:
	PacketSequenceNumber mSequenceNumber; //패킷의 시퀀스번호
	float mTimeDispatched; //패킷이 송신된 시각
	std::unordered_map<int, TransmissionDataPtr> mTransmissionDataMap; 
};