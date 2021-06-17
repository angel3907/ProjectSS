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
		//�ٷ� �ٽ� ������
		NetworkManagerServer::sInstance->SendReadyPacket(Rt.first, Rt.second);
	}
}

void ReadyPacketTransmissionData::HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	//���� �� �� ����
}