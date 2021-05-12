#pragma once
#include <memory>
#include "SocketAddress.h"
#include "MoveList.h"

class ClientProxy
{
public:
	ClientProxy(const SocketAddress& InSocketAddress, const string& InName, int InPlayerId)
	:mSocketAddress(InSocketAddress),
	mName(InName),
	mPlayerId(InPlayerId)
	{}

	~ClientProxy(){}

	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }

private:
	SocketAddress mSocketAddress;
	string mName;
	int mPlayerId;

	MoveList mUnprocessedMoveList;
	bool mIsLastMoveTimestampDirty;
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;