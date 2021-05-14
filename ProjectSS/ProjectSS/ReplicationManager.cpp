#include "stdafx.h"

//출력 스트림에 객체 컬렉션을 리플리케이션 해줌
void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects)
{
	//리플리케이션용이라고 미리 표시
	InStream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));
	
	//각 객체를 하나씩 기록
	for (int i = 0; i < InAllObjects.size(); i++)
	{ 
		GameObject* Go = InAllObjects[i];
		ReplicateIntoStream(InStream, Go);

		//+ 게임오브젝트 하나를 썼다면, 한 비트를 더 써줌 (패킷의 마지막인지를 나타냄)
		bool bEndOfPacket = false;
		
		if (i == InAllObjects.size() - 1)
		{
			bEndOfPacket = true;
		}

		InStream.Write(bEndOfPacket);
	}
	
	printf("Real Send Cnt %d\n", InStream.GetBitLength());
}

void ReplicationManager::ReceiveWorld(InputMemoryBitStream& InStream)
{
	std::unordered_set<GameObject*> ReceivedObjects;

	printf("Rcv Bit Count is %d\n", InStream.GetRemainingBitCount());
	//스트림의 모든 데이터를 다 읽을 때까지
	while (InStream.GetRemainingBitCount() > 0)
	{
		GameObject* ReceivedGameObject = ReceiveReplicatedObject(InStream);
		ReceivedObjects.insert(ReceivedGameObject);

		//+ 게임오브젝트 하나를 읽어왔다면 한 비트를 더 읽어옴 (패킷의 마지막인지를 나타냄)
		bool bEndOfPacket;
		InStream.Read(bEndOfPacket);

		if (bEndOfPacket)
		{
			break;
		}
	}

	//이제 mObjectsReplicatedToMe 집합을 순회하며
	//새로 리플리케이션 받은 집합에서 누락된 객체가 있으면 삭제
	for (GameObject* Go : mObjectsReplicatedToMe)
	{
		//누락되었다면 삭제
		if (ReceivedObjects.find(Go) == ReceivedObjects.end())
		{
			LinkingContext::Get().RemoveGameObject(Go);
			delete Go; //TODO? 나중에 Destroy 함수 같은 걸로 대체해야 할까?
		}
	}

	mObjectsReplicatedToMe = ReceivedObjects;
}

void ReplicationManager::ReplicateCreate(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	//리플리케이션 헤더 쓰고 데이터 변경점을 기록해줌
	ReplicationHeader Rh(ReplicationAction::RA_Create, LinkingContext::Get().GetNetworkId(InGameObject, true), InGameObject->GetClassId());
	Rh.Write(InStream);

	InGameObject->Write(InStream);
	//부분 리플리케이션
	//InGameObject->WriteChanged(InStream);
}

void ReplicationManager::ReplicateUpdate(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	ReplicationHeader Rh(ReplicationAction::RA_Update, LinkingContext::Get().GetNetworkId(InGameObject, false), InGameObject->GetClassId());
	Rh.Write(InStream);

	InGameObject->Write(InStream);
	//부분 리플리케이션
	//InGameObject->WriteChanged(InStream);
}

void ReplicationManager::ReplicateDestroy(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	ReplicationHeader Rh(ReplicationAction::RA_Destroy, LinkingContext::Get().GetNetworkId(InGameObject, false));
	Rh.Write(InStream);
}

void ReplicationManager::RPC(OutputMemoryBitStream& InStream, RPCParams* InRPCParams)
{
	ReplicationHeader Rh(ReplicationAction::RA_RPC);
	Rh.Write(InStream);
	InStream.Write(InRPCParams->GetRPCId());

	RPCManager::Get().ProcessRPC(InStream, InRPCParams);
}

void ReplicationManager::RMI(OutputMemoryBitStream& InStream, GameObject* InGameObject, RPCParams* InRPCParams)
{
	ReplicationHeader Rh(ReplicationAction::RA_RMI, LinkingContext::Get().GetNetworkId(InGameObject, false));
	Rh.Write(InStream);
	InStream.Write(InRPCParams->GetRPCId());

	RPCManager::Get().ProcessRMI(InStream, InRPCParams, InGameObject);
}

void ReplicationManager::ProcessReplicationAction(InputMemoryBitStream& InStream)
{
	ReplicationHeader Rh;
	Rh.Read(InStream);

	switch (Rh.mReplicationAction)
	{
		case ReplicationAction::RA_Create:
		{
			GameObject* Go = ObjectCreationRegistry::Get().CreateGameObject(Rh.mClassId);
			LinkingContext::Get().AddGameObject(Go, Rh.mNetworkId);
			Go->SetNetworkId(Rh.mNetworkId);
			
			Go->Read(InStream);
			//부분 리플리케이션
			//Go->ReadChanged(InStream);
		}
		break;
		case ReplicationAction::RA_Update:
		{
			GameObject* Go = LinkingContext::Get().GetGameObject(Rh.mNetworkId);
			if (Go)
			{
				Go->Read(InStream);
				//부분 리플리케이션
				//Go->ReadChanged(InStream);
			}
			else
			{
				//생성 동작을 아직 받지 못한 것 같음.
				//그러므로 더미 객체를 만들어 읽은 다음 폐기함
				uint32_t ClassId = Rh.mClassId;
				Go = ObjectCreationRegistry::Get().CreateGameObject(ClassId);
				Go->Read(InStream);
				//부분 리플리케이션
				//Go->ReadChanged(InStream);
				delete Go;
			}
		}
		break;
		case ReplicationAction::RA_Destroy:
		{
			GameObject* Go = LinkingContext::Get().GetGameObject(Rh.mNetworkId);
			if (Go)
			{ 
				LinkingContext::Get().RemoveGameObject(Go);
				delete Go;
			}
		}
		break;
		case ReplicationAction::RA_RPC:
		{
			RPCManager::Get().ProcessRPC(InStream);
		}
		break;
		case ReplicationAction::RA_RMI:
		{	
			RPCManager::Get().ProcessRMI(InStream, Rh.mNetworkId);
		}
		default:
		//이 동작은 여기서 처리 안 함.
		break;
	}
}

void ReplicationManager::ReplicateIntoStream(OutputMemoryBitStream& InStream, GameObject* InGameObject)
{
	//1. 게임 객체 네트워크 id 기록
	InStream.Write(LinkingContext::Get().GetNetworkId(InGameObject, true));

	//2. 게임 객체 클래스 식별자 기록
	uint32_t ClassId = InGameObject->GetClassId();
	InStream.Write(ClassId);

	//3. 객체 데이터 기록
	InGameObject->Write(InStream);
}

GameObject* ReplicationManager::ReceiveReplicatedObject(InputMemoryBitStream& InStream)
{
	uint32_t NetworkId = 0;
	uint32_t ClassId = 0;
	InStream.Read(NetworkId);
	InStream.Read(ClassId);

	//게임오브젝트 존재하지 않으면 만듦
	GameObject* Go = LinkingContext::Get().GetGameObject(NetworkId);
	if (Go == nullptr)
	{
		Go = ObjectCreationRegistry::Get().CreateGameObject(ClassId);
		LinkingContext::Get().AddGameObject(Go, NetworkId); //맵에도 추가
	}

	//객체를 스트림에서 읽어들임
	Go->Read(InStream);

	//삭제 여부 추적을 위해 수신한 객체를 리턴
	return Go;
}