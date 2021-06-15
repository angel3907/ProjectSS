#pragma once
#include "TransmissionData.h"
#include "ReplicationManager.h"
#include <vector>
#include <memory>

class ReplicationManagerTransmissionData : public TransmissionData
{
public:
	ReplicationManagerTransmissionData(ReplicationManager* InReplicationManager) :
	mReplicationManager(InReplicationManager){}

	class ReplicationTransmission
	{
	public:
		ReplicationTransmission(int InNetworkId, ReplicationAction InAction, uint32_t InState) :
		mNetworkId(InNetworkId),
		mAction(InAction),
		mState(InState)
		{}

		int GetNetworkId() const {return mNetworkId;}
		ReplicationAction GetAction() const {return mAction;}
		uint32_t GetState() const {return mState;}

	private:
		int mNetworkId;
		ReplicationAction mAction;
		uint32_t mState;
	};

	void AddTransmission(int InNetworkId, ReplicationAction InAction, uint32_t InState);

	virtual void HandleDeliveryFailure(DeliveryNotificationManager* InDeliveryNotificationManager) const override;
	virtual void HandleDeliverySuccess(DeliveryNotificationManager* InDeliveryNotificationManager) const override;

private:
	void HandleCreateDeliveryFailure(int InNetworkId) const;
	void HandleUpdateStateDeliveryFailure(int InNetworkId, uint32_t InState, DeliveryNotificationManager* inDeliveryNotificationManager) const;
	void HandleDestroyDeliveryFailure(int InNetworkId) const;
	void HandleCreateDeliverySuccess(int InNetworkId) const;
	void HandleDestroyDeliverySuccess(int InNetworkId) const;

	ReplicationManager* mReplicationManager;
	std::vector<ReplicationTransmission> mTransmissions;
};
typedef std::shared_ptr<ReplicationManagerTransmissionData> ReplicationManagerTransmissionDataPtr;