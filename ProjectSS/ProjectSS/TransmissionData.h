#pragma once
class DeliveryNotificationManager;
#include <memory>

class TransmissionData
{
public:
	virtual void HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const = 0;
	virtual void HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const = 0;
};
typedef std::shared_ptr<TransmissionData> TransmissionDataPtr;