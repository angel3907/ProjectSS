#include "NetworkManagerServer.h"
#include "stdafx.h"
#include "ClientProxy.h"
#include "Server.h"

NetworkManagerServer* NetworkManagerServer::sInstance;

NetworkManagerServer::NetworkManagerServer() : 
	mNewPlayerId(1),
	mTimeBetweenStatePackets(0.033f),
	mTimeOfLastStatePackets(0.f)
{

}

bool NetworkManagerServer::StaticInit(uint16_t InPort)
{
	sInstance = new NetworkManagerServer();
	return sInstance->Init(InPort);
}

void NetworkManagerServer::ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	//이미 아는 클라이언트인지 검사
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it == mAddressToClientMap.end()) //처음 통신한 클라이언트면
	{
		//헬로 패킷을 처리
		HandlePacketFromNewClient(InInputStream, InFromAddress);
	}
	else
	{
		//입력 패킷을 처리
		ProcessPacket((*it).second, InInputStream);
	}
}

void NetworkManagerServer::HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	//새 클라이언트라면 Hello를 먼저 받아야 함
	if (PacketType == kHelloCC)
	{
		string Name;
		InInputStream.Read(Name);

		//클라이언트 프록시 생성
		ClientProxyPtr NewClientProxy = std::make_shared<ClientProxy>(InFromAddress, Name, mNewPlayerId);

		//각 맵에 추가
		mAddressToClientMap[InFromAddress] = NewClientProxy;
		mPlayerIdToClientMap[NewClientProxy->GetPlayerId()] = NewClientProxy;

		//클라이언트 마중...
		SendWelcomePacket(NewClientProxy);

		//클라이언트를 위한 플레이어 생성
		static_cast<Server*> (Engine::sInstance.get())->HandleNewClient(NewClientProxy);

		//이 클라에 대한 리플리케이션 관리자를 가져와서
		//그리고 지금까지 월드에 있는 걸 모두 생성으로 리플리케이션해줌.
		for (auto GO : LinkingContext::Get().GetGameObjectSet())
		{
			SendReplicated(InFromAddress, NewClientProxy->GetReplicationManagerServer(), ReplicationAction::RA_Create, GO, nullptr);	
		}
	}
	else
	{
		//패킷이 잘못됐다고 로그 남김
		LOG("Bad Incoming packet from unknown client at socket %s", InFromAddress.ToString().c_str());
	}
}

void NetworkManagerServer::ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	//패킷 종류에 따라 처리
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	switch(PacketType)
	{
	case kHelloCC:
		//이미 웰컴을 보내줬는데 다시 보내달라고 온 상황.
		//다시 웰컴 패킷을 보냄.
		SendWelcomePacket(InClientProxy);
		break;
	case kInputCC:	
		//입력 패킷 처리
		HandleInputPacket(InClientProxy, InInputStream);
		break;
	default:
		LOG("Unknown packet type received from %s", InClientProxy->GetSocketAddress().ToString().c_str());
		break;
	}
}


ClientProxyPtr NetworkManagerServer::GetClientProxy(int PlayerId) const
{
	auto it = mPlayerIdToClientMap.find(PlayerId);
	if (it!=mPlayerIdToClientMap.end())
	{
		return it->second;
	}

	return nullptr;
}

void NetworkManagerServer::SendOutgoingPackets()
{
	//이동 결과 상태 패킷을 보냄
	//이동 상태가 들어온 게 있으면...
	for (auto AddressToClient : mAddressToClientMap)
	{
		ClientProxyPtr ClientProxyValue = AddressToClient.second;
		
		//TODO : 뒤에 타임스탬프가 Dirty일때만 보내는 것 추가하는데 그때 수정
		SendStatePacketToClient(ClientProxyValue);
	}
}

void NetworkManagerServer::SendWelcomePacket(ClientProxyPtr InClientProxy)
{
	OutputMemoryBitStream WelcomePacket;

	WelcomePacket.Write(kWelcomeCC);
	WelcomePacket.Write(InClientProxy->GetPlayerId());

	LOG("Server Welcoming, new Client '%s' as player %d", InClientProxy->GetName().c_str(), InClientProxy->GetPlayerId());

	SendPacket(WelcomePacket, InClientProxy->GetSocketAddress());
}

void NetworkManagerServer::UpdateAllClients()
{
	//모든 클라이언트를 업데이트 (상태 패킷 송신)
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastStatePackets + mTimeBetweenStatePackets)
	{
		for (auto AddressToClient : mAddressToClientMap)
		{
			SendStatePacketToClient(AddressToClient.second);
		}
		
		mTimeOfLastStatePackets = time;
	}
}

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	//상태패킷을 특정 클라이언트에게 송신
	OutputMemoryBitStream StatePacket;

	//일단 임시대응.. RA 큐에 등록된 걸 리플리케이션시킴
	for (auto RA : LinkingContext::Get().GetUnprocessedRAs())
	{
		GameObject* GO = LinkingContext::Get().GetGameObject(RA.NerworkId);
		SendReplicated(InClientProxy->GetSocketAddress(), InClientProxy->GetReplicationManagerServer(), RA.RA, GO, nullptr);
	}

	LinkingContext::Get().ClearUnprocessedRAs();
}

void NetworkManagerServer::HandleInputPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	//Move값을 읽어와서 처리 안된 무브 리스트에 넣어줌
	uint32_t MoveCount = 0;
	Move MoveValue;
	InInputStream.Read(MoveCount, 2);

	for (; MoveCount > 0; --MoveCount)
	{
		MoveValue.Read(InInputStream);

		if (InClientProxy->GetUnprocessedMoveList().AddMove(MoveValue))
		{
			//InClientProxy->SetIsLastMoveTimestampDirty(true);
		}
	}
}
