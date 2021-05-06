#pragma once
#include <stdint.h>
class InputMemoryBitStream;
class OutputMemoryBitStream;

//���ø����̼� ������ ���� 3����
enum class ReplicationAction : uint8_t
{
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_MAX
};

class ReplicationHeader
{
public:
	ReplicationHeader(){}

	ReplicationHeader(ReplicationAction InRA, uint32_t InNetworkId, uint32_t InClassId = 0) :
		mReplicationAction(InRA), mNetworkId(InNetworkId), mClassId(InClassId){}
		
	ReplicationAction mReplicationAction;
	uint32_t mNetworkId;
	uint32_t mClassId;

	void Write(OutputMemoryBitStream& InStream);
	void Read(InputMemoryBitStream& InStream);
};