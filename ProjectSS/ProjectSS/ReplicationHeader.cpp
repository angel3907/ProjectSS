#include "stdafx.h"

void ReplicationHeader::Write(OutputMemoryBitStream& InStream)
{
	InStream.WriteBits(&mReplicationAction, GetRequiredBits(static_cast<int32_t>(ReplicationAction::RA_MAX)));
	InStream.Write(mNetworkId);

	//�ı��� �� Ŭ���� �ĺ��� �ʿ� ����
	if (mReplicationAction != ReplicationAction::RA_Destroy)
		InStream.Write(mClassId);
}

void ReplicationHeader::Read(InputMemoryBitStream& InStream)
{
	InStream.ReadBits(&mReplicationAction, GetRequiredBits(static_cast<int32_t>(ReplicationAction::RA_MAX)));
	InStream.Read(mNetworkId);

	if (mReplicationAction != ReplicationAction::RA_Destroy)
		InStream.Read(mClassId);
}