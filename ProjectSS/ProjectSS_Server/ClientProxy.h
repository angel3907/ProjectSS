#pragma once
#include <memory>
#include "SocketAddress.h"
#include "MoveList.h"
#include "ReplicationManager.h"
#include "Player.h"
#include "DeliveryNotificationManager.h"

class ClientProxy
{
public:
	ClientProxy(const SocketAddress& InSocketAddress, const std::string& InName, PlayerColor InPlayerColor, int InPlayerId)
	:mSocketAddress(InSocketAddress),
	mName(InName),
	mPlayerColor(InPlayerColor),
	mPlayerId(InPlayerId),
	mDeliveryNotificationManager(false, true)
	{
		UpdateLastPacketTime();
	}

	~ClientProxy(){}

	SocketAddress& GetSocketAddress() {return mSocketAddress;}
	int GetPlayerId() {return mPlayerId;}
	std::string GetName() {return mName;}
	PlayerColor GetPlayerColor() {return mPlayerColor;}
	bool IsReady(){return bReady;}
	void SetReady(bool bInReady){bReady = bInReady;}
	
	ReplicationManager& GetReplicationManagerServer() {return mReplicationManagerServer;}
	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }
	const MoveList& GetUnprocessedMoveList() const { return mUnprocessedMoveList; }

	void UpdateLastPacketTime() {mLastPacketFromClientTime = TimeUtil::Get().GetTimef();}

	float GetLastPacketFromClientTime() const {return mLastPacketFromClientTime; }

	DeliveryNotificationManager& GetDeliveryNotificationManager() { return mDeliveryNotificationManager; }

	//�̵� Ÿ�ӽ������� ����Ǿ�����.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//��� ���� ������
	DeliveryNotificationManager mDeliveryNotificationManager;

	//���� ���ø����̼� �Ŵ���
	ReplicationManager mReplicationManagerServer;

	//�����ּ�, �÷��̾� �̸�, ���̵�
	SocketAddress mSocketAddress;
	std::string mName;
	int mPlayerId;
	PlayerColor mPlayerColor;

	MoveList mUnprocessedMoveList; //ó�� �ȵ� �̵� ����Ʈ

	//bool mIsLastMoveTimestampDirty;

	bool bReady = false;

	float mLastPacketFromClientTime; //���������� Ŭ������ ��Ŷ�� ���� �ð�
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;