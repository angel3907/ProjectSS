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

	//이동 타임스탬프가 변경되었는지.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//배달 통지 관리자
	DeliveryNotificationManager mDeliveryNotificationManager;

	//전용 리플리케이션 매니저
	ReplicationManager mReplicationManagerServer;

	//소켓주소, 플레이어 이름, 아이디
	SocketAddress mSocketAddress;
	std::string mName;
	int mPlayerId;
	PlayerColor mPlayerColor;

	MoveList mUnprocessedMoveList; //처리 안된 이동 리스트

	//bool mIsLastMoveTimestampDirty;

	bool bReady = false;

	float mLastPacketFromClientTime; //마지막으로 클라한테 패킷을 받은 시간
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;