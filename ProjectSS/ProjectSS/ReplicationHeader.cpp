#include "stdafx.h"

void ReplicationHeader::Write(OutputMemoryBitStream& InStream)
{
	InStream.WriteBits(&mReplicationAction, GetRequiredBits(static_cast<int32_t>(ReplicationAction::RA_MAX)));
	
	//RPC를 보낼 땐 네트워크 아이디가 필요 없음
	if (mReplicationAction != ReplicationAction::RA_RPC)
		InStream.Write(mNetworkId);

	//파괴할 땐 클래스 식별자 필요 없음
	if (mReplicationAction != ReplicationAction::RA_Destroy && mReplicationAction != ReplicationAction::RA_RPC)
		InStream.Write(mClassId);
}

void ReplicationHeader::Read(InputMemoryBitStream& InStream)
{
	InStream.ReadBits(&mReplicationAction, GetRequiredBits(static_cast<int32_t>(ReplicationAction::RA_MAX)));
	
	if (mReplicationAction != ReplicationAction::RA_RPC)
		InStream.Read(mNetworkId);

	if (mReplicationAction != ReplicationAction::RA_Destroy && mReplicationAction != ReplicationAction::RA_RPC)
		InStream.Read(mClassId);
}