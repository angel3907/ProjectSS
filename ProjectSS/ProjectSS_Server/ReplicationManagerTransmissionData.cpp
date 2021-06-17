#include "ReplicationManagerTransmissionData.h"
#include "LinkingContext.h"
#include "StringUtils.h"
#include "DeliveryNotificationManager.h"

void ReplicationManagerTransmissionData::AddTransmission(int InNetworkId, ReplicationAction InAction, uint32_t InState)
{
	mTransmissions.emplace_back(InNetworkId, InAction, InState);
}

void ReplicationManagerTransmissionData::HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	for (const ReplicationTransmission& Rt : mTransmissions)
	{
		int NetworkId = Rt.GetNetworkId();

		switch (Rt.GetAction())
		{
		case ReplicationAction::RA_Create :
			HandleCreateDeliveryFailure(NetworkId);
			LOG("HandleCreateDeliveryFailure, Network Id : %d", NetworkId);
			break;
		case ReplicationAction::RA_Update:
			HandleUpdateStateDeliveryFailure(NetworkId, Rt.GetState(), InDeliveryNotificationManager);
			LOG("HandleUpdateStateDeliveryFailure, Network Id : %d", NetworkId);
			break;
		case ReplicationAction::RA_Destroy:
			HandleDestroyDeliveryFailure(NetworkId);
			LOG("HandleDestroyDeliveryFailure, Network Id : %d", NetworkId);
			break;
		}
	}
}

void ReplicationManagerTransmissionData::HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const
{
	for (const ReplicationTransmission& Rt : mTransmissions)
	{
		int NetworkId = Rt.GetNetworkId();

		switch (Rt.GetAction())
		{
		case ReplicationAction::RA_Create:
			HandleCreateDeliverySuccess(NetworkId);
			break;
		case ReplicationAction::RA_Destroy:
			HandleDestroyDeliverySuccess(NetworkId);
			break;
		}
	}
}

void ReplicationManagerTransmissionData::HandleCreateDeliveryFailure(int InNetworkId) const
{
	//아직도 오브젝트가 존재한다면 다시 create 리플리케이션을 보낸다
	GameObject* GameObject_ = LinkingContext::Get().GetGameObject(InNetworkId);
	if (GameObject_)
	{
		ReplicationCommand RC;
		RC.NetworkId = InNetworkId;
		RC.RA =  ReplicationAction::RA_Create;
		mReplicationManager->AddUnprocessedRA(RC);
	}
}

void ReplicationManagerTransmissionData::HandleUpdateStateDeliveryFailure(int InNetworkId, uint32_t InState, DeliveryNotificationManager* inDeliveryNotificationManager) const
{
	GameObject* Go = LinkingContext::Get().GetGameObject(InNetworkId);

	if (Go)
	{
		for (const auto& InFlightPacket_ : inDeliveryNotificationManager->GetInFlightPackets())
		{
			ReplicationManagerTransmissionDataPtr RMTDP 
			= std::static_pointer_cast<ReplicationManagerTransmissionData>(InFlightPacket_.GetTransmissionData('RPLM'));

			if (RMTDP && &(*RMTDP) != this)
			{
				for (const ReplicationTransmission& OtherRT : RMTDP->mTransmissions)
				{
					if (OtherRT.GetNetworkId() == InNetworkId && OtherRT.GetAction() == ReplicationAction::RA_Update)
					{
						//이미 업데이트 패킷을 보낸게 있다면 재전송하지 않는다.
						return;
					}
				}
			}
		}
	}

	//현재 코드에서 딱히 상태 비트를 사용하고 있지 않아 이렇게 구현하였다.
	ReplicationCommand RC;
	RC.NetworkId = InNetworkId;
	RC.RA = ReplicationAction::RA_Update;
	mReplicationManager->AddUnprocessedRA(RC);
}

void ReplicationManagerTransmissionData::HandleDestroyDeliveryFailure(int InNetworkId) const
{
	ReplicationCommand RC;
	RC.NetworkId = InNetworkId;
	RC.RA = ReplicationAction::RA_Destroy;
	mReplicationManager->AddUnprocessedRA(RC);
}

void ReplicationManagerTransmissionData::HandleCreateDeliverySuccess(int InNetworkId) const
{
	//원래 프로젝트에서는 리플리케이션 액션을 Update로 설정해주었음
}

void ReplicationManagerTransmissionData::HandleDestroyDeliverySuccess(int InNetworkId) const
{
	//원래 프로젝트에서는 리플리케이션 커맨드 목록에서 제거했음
}
