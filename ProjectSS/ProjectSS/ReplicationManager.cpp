#include "stdafx.h"

//출력 스트림에 객체 컬렉션을 리플리케이션 해줌
void ReplicationManager::ReplicateWorldState(OutputMemoryBitStream& InStream, const std::vector<GameObject*>& InAllObjects)
{
	//리플리케이션용이라고 미리 표시
	InStream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));
	
	//각 객체를 하나씩 기록
	for(GameObject* Go : InAllObjects)
	{
		ReplicateIntoStream(InStream, Go);
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
		uint32_t RemainedBitCount = InStream.GetRemainingBitCount();
		GameObject* ReceivedGameObject = ReceiveReplicatedObject(InStream);
		ReceivedObjects.insert(ReceivedGameObject);
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