#pragma once
#include <memory>
#include "SocketAddress.h"
#include "MoveList.h"
#include "ReplicationManager.h"

class ClientProxy
{
public:
	ClientProxy(const SocketAddress& InSocketAddress, const string& InName, int InPlayerId)
	:mSocketAddress(InSocketAddress),
	mName(InName),
	mPlayerId(InPlayerId)
	{}

	~ClientProxy(){}

	SocketAddress& GetSocketAddress() {return mSocketAddress;}
	int GetPlayerId() {return mPlayerId;}
	string GetName() {return mName;}
	
	ReplicationManager& GetReplicationManagerServer() {return mReplicationManagerServer;}
	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }
	const MoveList& GetUnprocessedMoveList() const { return mUnprocessedMoveList; }

	//이동 타임스탬프가 변경되었는지.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//전용 리플리케이션 매니저
	ReplicationManager mReplicationManagerServer;

	//소켓주소, 플레이어 이름, 아이디
	SocketAddress mSocketAddress;
	string mName;
	int mPlayerId;

	MoveList mUnprocessedMoveList; //처리 안된 이동 리스트
	//bool mIsLastMoveTimestampDirty;
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;