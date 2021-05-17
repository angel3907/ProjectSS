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

	//�̵� Ÿ�ӽ������� ����Ǿ�����.
	//void SetIsLastMoveTimestampDirty(bool InIsDirty) { mIsLastMoveTimestampDirty = InIsDirty;}
	//bool IsLastMoveTimestampDirty() const {return mIsLastMoveTimestampDirty;}

private:
	//���� ���ø����̼� �Ŵ���
	ReplicationManager mReplicationManagerServer;

	//�����ּ�, �÷��̾� �̸�, ���̵�
	SocketAddress mSocketAddress;
	std::string mName;
	int mPlayerId;

	MoveList mUnprocessedMoveList; //ó�� �ȵ� �̵� ����Ʈ

	std::vector<ReplicationCommand> UnprocessedRAs; //+�߰� : RA�ؾ��� ���� ������ ���� �߰�����
	//bool mIsLastMoveTimestampDirty;

	float mLastPacketFromClientTime; //���������� Ŭ������ ��Ŷ�� ���� �ð�
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;