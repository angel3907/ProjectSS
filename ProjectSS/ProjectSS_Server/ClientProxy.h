#pragma once
#include <memory>
#include "SocketAddress.h"
#include "MoveList.h"
#include "ReplicationManager.h"

class ClientProxy
{
public:
	ClientProxy(const SocketAddress& InSocketAddress, const std::string& InName, int InPlayerId)
	:mSocketAddress(InSocketAddress),
	mName(InName),
	mPlayerId(InPlayerId)
	{
		UpdateLastPacketTime();
	}

	~ClientProxy(){}

	SocketAddress& GetSocketAddress() {return mSocketAddress;}
	int GetPlayerId() {return mPlayerId;}
	std::string GetName() {return mName;}
	
	ReplicationManager& GetReplicationManagerServer() {return mReplicationManagerServer;}
	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }
	const MoveList& GetUnprocessedMoveList() const { return mUnprocessedMoveList; }

	std::vector<ReplicationCommand>& GetUnprocessedRAs() { return UnprocessedRAs; }
	void AddUnprocessedRA(ReplicationCommand& RC) { UnprocessedRAs.push_back(RC); }
	void ClearUnprocessedRAs() { UnprocessedRAs.clear(); }

	void UpdateLastPacketTime() {mLastPacketFromClientTime = TimeUtil::Get().GetTimef();}

	float GetLastPacketFromClientTime() const {return mLastPacketFromClientTime; }

	//이동 타임스탬프가 변경되었는지.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//전용 리플리케이션 매니저
	ReplicationManager mReplicationManagerServer;

	//소켓주소, 플레이어 이름, 아이디
	SocketAddress mSocketAddress;
	std::string mName;
	int mPlayerId;

	MoveList mUnprocessedMoveList; //처리 안된 이동 리스트

	std::vector<ReplicationCommand> UnprocessedRAs; //+추가 : RA해야할 일이 있으면 여기 추가해줌
	//bool mIsLastMoveTimestampDirty;

	float mLastPacketFromClientTime; //마지막으로 클라한테 패킷을 받은 시간
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;