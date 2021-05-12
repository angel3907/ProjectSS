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

	//�̵� Ÿ�ӽ������� ����Ǿ�����.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//���� ���ø����̼� �Ŵ���
	ReplicationManager mReplicationManagerServer;

	//�����ּ�, �÷��̾� �̸�, ���̵�
	SocketAddress mSocketAddress;
	string mName;
	int mPlayerId;

	MoveList mUnprocessedMoveList; //ó�� �ȵ� �̵� ����Ʈ
	//bool mIsLastMoveTimestampDirty;
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;