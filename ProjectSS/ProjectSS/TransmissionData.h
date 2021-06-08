#pragma once
class DeliveryNoficationManager;
#include <memory>

class TransmissionData
{
public:
	virtual void HandleDeliveryFailure(DeliveryNoficationManager* InDeliveryNotificationManager) const = 0;
	virtual void HandleDeliverySuccess(DeliveryNoficationManager* InDeliveryNotificationManager) const = 0;
};
typedef std::shared_ptr<TransmissionData> TransmissionDataPtr;