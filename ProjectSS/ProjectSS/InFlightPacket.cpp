#include "stdafx.h"
#include "InFlightPacket.h"

InFlightPacket::InFlightPacket(PacketSequenceNumber InSequenceNumber) :
mSequenceNumber(InSequenceNumber),
mTimeDispatched(TimeUtil::Get().GetTimef())
{

}

void InFlightPacket::HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliveryFailure(InDeliveryNotificationManager);
	}
}

void InFlightPacket::HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	for (const auto& pair : mTransmissionDataMap)
	{
		pair.second->HandleDeliverySuccess(InDeliveryNotificationManager);
	}
}
