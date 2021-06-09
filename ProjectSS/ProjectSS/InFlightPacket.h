#pragma once
#include <unordered_map>
#include "TransmissionData.h"
class DeliveryNotificationManager;

//��Ʈ�� �� ���� �Ǵ� ���� ����ؾ� �Ѵٸ� Ÿ���� �ٲ�� �� ��.
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

	void HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const;
	void HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const;

private:
	PacketSequenceNumber mSequenceNumber; //��Ŷ�� ��������ȣ
	float mTimeDispatched; //��Ŷ�� �۽ŵ� �ð�
	std::unordered_map<int, TransmissionDataPtr> mTransmissionDataMap; 
};