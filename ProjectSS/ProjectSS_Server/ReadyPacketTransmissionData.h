#pragma once
#include "TransmissionData.h"
#include <vector>
#include "NetworkManager.h"
#include "ClientProxy.h"

class ReadyPacketTransmissionData : public TransmissionData
{
public:
	ReadyPacketTransmissionData(){}

	void AddTransmission(ClientProxyPtr InClientProxy, ReadyPacketType InPacketType);

	virtual void HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const override;
	virtual void HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const override;

private:
	std::vector<std::pair<ClientProxyPtr, ReadyPacketType>> mReadyPacketTransmissions;
};
typedef std::shared_ptr<ReadyPacketTransmissionData> ReadyPacketTransmissionDataPtr;