#include "ReadyPacketTransmissionData.h"
#include "NetworkManagerServer.h"

void ReadyPacketTransmissionData::AddTransmission(ClientProxyPtr InClientProxy, ReadyPacketType InPacketType)
{
	mReadyPacketTransmissions.push_back({InClientProxy, InPacketType});
}

void ReadyPacketTransmissionData::HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	LOG("ReadyPacketTransmissionData::HandleDeliveryFailure", 0);

	for (const auto& Rt : mReadyPacketTransmissions)
	{
		//바로 다시 보낸다
		NetworkManagerServer::sInstance->SendReadyPacket(Rt.first, Rt.second);
	}
}

void ReadyPacketTransmissionData::HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	//딱히 할 일 없음
}