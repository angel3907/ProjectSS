#include "NetworkManagerServer.h"
#include "stdafx.h"
#include "ClientProxy.h"
#include "Server.h"
#include "DeliveryNotificationManager.h"

NetworkManagerServer* NetworkManagerServer::sInstance;

NetworkManagerServer::NetworkManagerServer() : 
	mNewPlayerId(1),
	mTimeBetweenStatePackets(0.033f),
	mTimeOfLastStatePackets(0.f),
	mClientDisconnectTimeout(2.0f) //TODO 서밋전수정
{

}

size_t NetworkManagerServer::SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager, ReplicationManagerTransmissionDataPtr InTransmissionData, DeliveryNotificationManager* InDeliveryNotificationManager, ReplicationAction InReplicationAction, GameObject* InGameObject, int InNetworkId, RPCParams* InRPCParams)
{
	OutputMemoryBitStream OutputStream;
	OutputStream.Write(kStateCC);

	InFlightPacket* InFlightPacket_ = InDeliveryNotificationManager->WriteState(OutputStream);

	//Transmission Data 세팅
	//상태 비트는 처리 안하고 있으니 0으로 넘겨줌.
	InFlightPacket_->SetTransmissionData('RPLM', InTransmissionData);
	InTransmissionData->AddTransmission(InNetworkId, InReplicationAction, 0);

	//리플리케이션용이라고 미리 표시
	OutputStream.WriteBits(static_cast<uint8_t>(PacketType::PT_ReplicationData), GetRequiredBits(static_cast<int32_t>(PacketType::PT_MAX)));

	switch (InReplicationAction)
	{
	case ReplicationAction::RA_Create:
		if (InGameObject)
		{
			InReplicationManager.ReplicateCreate(OutputStream, InGameObject);
		}
		break;
	case ReplicationAction::RA_Update:
		if (InGameObject)
		{
			InReplicationManager.ReplicateUpdate(OutputStream, InGameObject);
		}
		break;
	case ReplicationAction::RA_Destroy:
		InReplicationManager.ReplicateDestroy(OutputStream, InNetworkId);
		break;
	case ReplicationAction::RA_RPC:
		InReplicationManager.RPC(OutputStream, InRPCParams);
		break;
	case ReplicationAction::RA_RMI:
		if (InGameObject)
		{
			InReplicationManager.RMI(OutputStream, InGameObject, InRPCParams);
		}
		break;
	default:
		break;
	}

	size_t SentByteCount = mSocket->SendTo(OutputStream.GetBufferPtr(), OutputStream.GetByteLength(), InToAddress);
	return SentByteCount;
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
		//방이 가득 차거나, 게임이 시작된 경우에는 새 클라이언트 처리를 해선 안되고 입장 불가 패킷을 준다.	
		Server* Server_ = static_cast<Server*> (Engine::sInstance.get());
		if (Server_->IsNoAdmittance())
		{
			SendNoAdmittancePacket(InFromAddress, Server_->GetNoAdmittanceReason());
		}
		else
		{ 
			std::string Name = "";
			InInputStream.Read(Name);

			PlayerColor Color;
			InInputStream.Read(Color);

			//클라이언트 프록시 생성
			ClientProxyPtr NewClientProxy = std::make_shared<ClientProxy>(InFromAddress, Name, Color, mNewPlayerId++);

			//각 맵에 추가
			mAddressToClientMap[InFromAddress] = NewClientProxy;
			mPlayerIdToClientMap[NewClientProxy->GetPlayerId()] = NewClientProxy;

			//클라이언트 마중...
			SendWelcomePacket(NewClientProxy);

			//클라이언트를 위한 플레이어 생성
			static_cast<Server*> (Engine::sInstance.get())->HandleNewClient(NewClientProxy);

			Player* NewPlayer = static_cast<Server*> (Engine::sInstance.get())->GetPlayerWithPlayerId(NewClientProxy->GetPlayerId());

			if (NewPlayer == nullptr)
			{
				LOG("새 플레이어가 생성되지 않았습니다. Player Id는 %d\n", NewClientProxy->GetPlayerId());
				return;
			}

			//타 클라에도 해당 플레이어를 리플리케이션
			for (auto ClientProxyValue : mPlayerIdToClientMap)
			{
				if (ClientProxyValue.first != NewClientProxy->GetPlayerId())
				{
					ReplicationCommand RA;
					RA.NetworkId = NewPlayer->GetNetworkId();
					RA.RA = ReplicationAction::RA_Create;
					ClientProxyValue.second->GetReplicationManagerServer().AddUnprocessedRA(RA);
				}
			}


			ReplicationManagerTransmissionDataPtr RepTransData = nullptr;

			//이 클라에 대한 리플리케이션 관리자를 가져와서
			//그리고 지금까지 월드에 있는 걸 모두 생성으로 리플리케이션해줌.
			for (auto GO : LinkingContext::Get().GetGameObjectSet())
			{
				if (!RepTransData)
				{
					RepTransData = std::make_shared<ReplicationManagerTransmissionData>(&NewClientProxy->GetReplicationManagerServer());
				}

				SendReplicated(InFromAddress, NewClientProxy->GetReplicationManagerServer(), RepTransData, &NewClientProxy->GetDeliveryNotificationManager(),
					ReplicationAction::RA_Create, GO, GO->GetNetworkId(), nullptr);
			}
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
	//패킷 온 시간 기록
	InClientProxy->UpdateLastPacketTime();

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
	case kReadyCC:
		//클라에게 게임 준비하겠다고 신호가 온 상황
		HandleReadyPacket(InClientProxy, InInputStream);
		break;
	case kInputCC:	
		if (InClientProxy->GetDeliveryNotificationManager().ReadAndProcessState(InInputStream))
		{
			//입력 패킷 처리
			HandleInputPacket(InClientProxy, InInputStream);
		}
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
		//타임아웃된 패킷 처리
		ClientProxyValue->GetDeliveryNotificationManager().ProcessTimedOutPackets();

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

void NetworkManagerServer::HandleReadyPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	uint8_t ReadyPacketType_;
	InInputStream.Read(ReadyPacketType_);

	//일단 READY_SEND 타입 패킷이 아닌 경우 처리하지 않는다
	if (ReadyPacketType_ != READY_SEND)
	{
		return;
	}

	InClientProxy->SetReady(true);
	SendReadyPacket(InClientProxy, READY_ACK);

	Server* Server_ = static_cast<Server*> (Engine::sInstance.get());
	Server_->CheckReady();

	if (Server_->IsGameStarted())
	{
		SendReadyPacketToAllClient(START);
	}
}

void NetworkManagerServer::SendReadyPacket(ClientProxyPtr InClientProxy, ReadyPacketType InReadyPacketType)
{
	OutputMemoryBitStream ReadyAckPacket;

	ReadyAckPacket.Write(kReadyCC);
	ReadyAckPacket.Write(InReadyPacketType);

	LOG("Server Ready packet, Client '%s' as player %d, Ready Packet Type is %d", 
		InClientProxy->GetName().c_str(), InClientProxy->GetPlayerId(), InReadyPacketType);

	SendPacket(ReadyAckPacket, InClientProxy->GetSocketAddress());
}

void NetworkManagerServer::SendReadyPacketToAllClient(ReadyPacketType InReadyPacketType)
{
	for (auto AddressToClient : mAddressToClientMap)
	{
		SendReadyPacket(AddressToClient.second, InReadyPacketType);
	}
}

void NetworkManagerServer::HandleGameEnd()
{
	std::vector<ClientProxyPtr> ClientToDisconnect;

	for (const auto& Pair : mAddressToClientMap)
	{
		ClientToDisconnect.push_back(Pair.second);
	}

	//삭제할 클라이언트 프록시 처리
	for (ClientProxyPtr ClientProxyValue : ClientToDisconnect)
	{
		//각 맵에서 제거
		mAddressToClientMap.erase(ClientProxyValue->GetSocketAddress());
		mPlayerIdToClientMap.erase(ClientProxyValue->GetPlayerId());
	}
}

void NetworkManagerServer::SendNoAdmittancePacket(const SocketAddress& InFromAddress, NoAdmittanceReason InReason)
{
	OutputMemoryBitStream NoAdmittancePacket;

	NoAdmittancePacket.Write(kNoAdmittanceCC);
	NoAdmittancePacket.Write((uint8_t)InReason);

	switch (InReason)
	{
	case NoAdmittanceReason::NONE:
		printf("Error : NoAdmittanceReason should not be none.\n");
		break;
	case NoAdmittanceReason::FULL_PLAYER:
		printf("No Admittance : Full Player\n");
		break;
	case NoAdmittanceReason::GAME_STARTED:
		printf("No Admittance : Game Started\n");
		break;
	default:
		break;
	}

	SendPacket(NoAdmittancePacket, InFromAddress);
}

void NetworkManagerServer::UpdateAllClients()
{
	//모든 클라이언트를 업데이트 (상태 패킷 송신)
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastStatePackets + mTimeBetweenStatePackets)
	{
		for (auto AddressToClient : mAddressToClientMap)
		{
			//타임아웃된 패킷 처리
			AddressToClient.second->GetDeliveryNotificationManager().ProcessTimedOutPackets();
			SendStatePacketToClient(AddressToClient.second);
		}
		
		mTimeOfLastStatePackets = time;
	}
}

void NetworkManagerServer::AddUnprocessedRAToAllClients(ReplicationCommand& RA)
{
	for (auto AddressToClientProxy : mAddressToClientMap)
	{
		AddressToClientProxy.second->GetReplicationManagerServer().AddUnprocessedRA(RA);
	}
}

void NetworkManagerServer::SendReplicatedToAllClients(ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams)
{
	for (auto AddressToClientProxy : mAddressToClientMap)
	{
		ClientProxyPtr Cp = AddressToClientProxy.second;
		ReplicationManagerTransmissionDataPtr RepTransData = std::make_shared<ReplicationManagerTransmissionData>(&Cp->GetReplicationManagerServer());
		SendReplicated(Cp->GetSocketAddress(), Cp->GetReplicationManagerServer(), RepTransData,
					   &Cp->GetDeliveryNotificationManager(), InReplicationAction, InGameObject, InGameObject->GetNetworkId(), InRPCParams);
	}
}

void NetworkManagerServer::HandleLostClient(ClientProxyPtr InClientProxy)
{
	//각 맵에서 제거
	mAddressToClientMap.erase(InClientProxy->GetSocketAddress());
	mPlayerIdToClientMap.erase(InClientProxy->GetPlayerId());

	//링킹 컨텍스트에서 제거, 플레이어 오브젝트 삭제
	static_cast<Server*> (Engine::sInstance.get())->HandleLostClient(InClientProxy);
}

void NetworkManagerServer::HandleConnectionReset(const SocketAddress& InFromAddress)
{
	//address 맵에서 클라이언트를 찾아서 플레이어와 해당 클라이언트 프록시를 처리해줌
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it != mAddressToClientMap.end())
	{
		HandleLostClient(it->second);
	}
}

void NetworkManagerServer::CheckForDisconnects()
{	
	std::vector<ClientProxyPtr> ClientToDisconnect;

	//연결이 유지되는 클라이언트는 적어도 이 시간 전에는 패킷을 보낸 적이 있어야 함.
	float MinAllowedLastPacketFromClientTime = TimeUtil::Get().GetTimef() - mClientDisconnectTimeout;

	for (const auto& Pair : mAddressToClientMap)
	{
		//해당 시간 이전에 보낸 패킷이 마지막이였다면, 삭제할 목록에 넣어줌
		if (Pair.second->GetLastPacketFromClientTime() < MinAllowedLastPacketFromClientTime)
		{
			ClientToDisconnect.push_back(Pair.second);
		}
	}

	//삭제할 클라이언트 프록시 처리
	for (ClientProxyPtr ClientProxyValue : ClientToDisconnect)
	{
		HandleLostClient(ClientProxyValue);
	}
}

bool NetworkManagerServer::IsAllPlayersReady()
{
	bool bAllPlayerReady = true;

	for (auto AddressToClient : mAddressToClientMap)
	{
		if (!AddressToClient.second->IsReady())
		{
			bAllPlayerReady = false;
			break;
		}
	}

	return bAllPlayerReady;
}

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	ReplicationManagerTransmissionDataPtr RepTransData = nullptr;

	//일단 임시대응.. RA 큐에 등록된 걸 리플리케이션시킴
	for (auto RA : InClientProxy->GetReplicationManagerServer().GetUnprocessedRAs())
	{
		if (!RepTransData)
		{
			RepTransData = std::make_shared<ReplicationManagerTransmissionData>(&InClientProxy->GetReplicationManagerServer());
		}

		GameObject* GO = LinkingContext::Get().GetGameObject(RA.NetworkId);
		SendReplicated(InClientProxy->GetSocketAddress(), InClientProxy->GetReplicationManagerServer(), RepTransData,
			&InClientProxy->GetDeliveryNotificationManager(), RA.RA, GO, RA.NetworkId, nullptr);
		
		printf("Update Client : GameObject Network Id %d\n", RA.NetworkId);
	}

	InClientProxy->GetReplicationManagerServer().ClearUnprocessedRAs();
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

		//printf("Move Value is hor : %f, ver : %f, IsAttacking : %d\n", MoveValue.GetInputState().GetDesiredHorizontalDelta(), MoveValue.GetInputState().GetDesiredVerticallDelta(), MoveValue.GetInputState().IsAttacking());

		if (InClientProxy->GetUnprocessedMoveList().AddMove(MoveValue))
		{
			//InClientProxy->SetIsLastMoveTimestampDirty(true);
		}
	}
}