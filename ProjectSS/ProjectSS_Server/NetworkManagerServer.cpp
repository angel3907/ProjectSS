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
	mClientDisconnectTimeout(2.0f) //TODO ����������
{

}

size_t NetworkManagerServer::SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager, ReplicationManagerTransmissionDataPtr InTransmissionData, DeliveryNotificationManager* InDeliveryNotificationManager, ReplicationAction InReplicationAction, GameObject* InGameObject, int InNetworkId, RPCParams* InRPCParams)
{
	OutputMemoryBitStream OutputStream;
	OutputStream.Write(kStateCC);

	InFlightPacket* InFlightPacket_ = InDeliveryNotificationManager->WriteState(OutputStream);

	//Transmission Data ����
	//���� ��Ʈ�� ó�� ���ϰ� ������ 0���� �Ѱ���.
	InFlightPacket_->SetTransmissionData('RPLM', InTransmissionData);
	InTransmissionData->AddTransmission(InNetworkId, InReplicationAction, 0);

	//���ø����̼ǿ��̶�� �̸� ǥ��
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
	//�̹� �ƴ� Ŭ���̾�Ʈ���� �˻�
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it == mAddressToClientMap.end()) //ó�� ����� Ŭ���̾�Ʈ��
	{
		//��� ��Ŷ�� ó��
		HandlePacketFromNewClient(InInputStream, InFromAddress);
	}
	else
	{
		//�Է� ��Ŷ�� ó��
		ProcessPacket((*it).second, InInputStream);
	}
}

void NetworkManagerServer::HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	//�� Ŭ���̾�Ʈ��� Hello�� ���� �޾ƾ� ��
	if (PacketType == kHelloCC)
	{
		//���� ���� ���ų�, ������ ���۵� ��쿡�� �� Ŭ���̾�Ʈ ó���� �ؼ� �ȵǰ� ���� �Ұ� ��Ŷ�� �ش�.	
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

			//Ŭ���̾�Ʈ ���Ͻ� ����
			ClientProxyPtr NewClientProxy = std::make_shared<ClientProxy>(InFromAddress, Name, Color, mNewPlayerId++);

			//�� �ʿ� �߰�
			mAddressToClientMap[InFromAddress] = NewClientProxy;
			mPlayerIdToClientMap[NewClientProxy->GetPlayerId()] = NewClientProxy;

			//Ŭ���̾�Ʈ ����...
			SendWelcomePacket(NewClientProxy);

			//Ŭ���̾�Ʈ�� ���� �÷��̾� ����
			static_cast<Server*> (Engine::sInstance.get())->HandleNewClient(NewClientProxy);

			Player* NewPlayer = static_cast<Server*> (Engine::sInstance.get())->GetPlayerWithPlayerId(NewClientProxy->GetPlayerId());

			if (NewPlayer == nullptr)
			{
				LOG("�� �÷��̾ �������� �ʾҽ��ϴ�. Player Id�� %d\n", NewClientProxy->GetPlayerId());
				return;
			}

			//Ÿ Ŭ�󿡵� �ش� �÷��̾ ���ø����̼�
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

			//�� Ŭ�� ���� ���ø����̼� �����ڸ� �����ͼ�
			//�׸��� ���ݱ��� ���忡 �ִ� �� ��� �������� ���ø����̼�����.
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
		//��Ŷ�� �߸��ƴٰ� �α� ����
		LOG("Bad Incoming packet from unknown client at socket %s", InFromAddress.ToString().c_str());
	}
}

void NetworkManagerServer::ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	//��Ŷ �� �ð� ���
	InClientProxy->UpdateLastPacketTime();

	//��Ŷ ������ ���� ó��
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	switch(PacketType)
	{
	case kHelloCC:
		//�̹� ������ ������µ� �ٽ� �����޶�� �� ��Ȳ.
		//�ٽ� ���� ��Ŷ�� ����.
		SendWelcomePacket(InClientProxy);
		break;
	case kReadyCC:
		//Ŭ�󿡰� ���� �غ��ϰڴٰ� ��ȣ�� �� ��Ȳ
		HandleReadyPacket(InClientProxy, InInputStream);
		break;
	case kInputCC:	
		if (InClientProxy->GetDeliveryNotificationManager().ReadAndProcessState(InInputStream))
		{
			//�Է� ��Ŷ ó��
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
	//�̵� ��� ���� ��Ŷ�� ����
	//�̵� ���°� ���� �� ������...
	for (auto AddressToClient : mAddressToClientMap)
	{
		ClientProxyPtr ClientProxyValue = AddressToClient.second;
		//Ÿ�Ӿƿ��� ��Ŷ ó��
		ClientProxyValue->GetDeliveryNotificationManager().ProcessTimedOutPackets();

		//TODO : �ڿ� Ÿ�ӽ������� Dirty�϶��� ������ �� �߰��ϴµ� �׶� ����
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

	//�ϴ� READY_SEND Ÿ�� ��Ŷ�� �ƴ� ��� ó������ �ʴ´�
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

	//������ Ŭ���̾�Ʈ ���Ͻ� ó��
	for (ClientProxyPtr ClientProxyValue : ClientToDisconnect)
	{
		//�� �ʿ��� ����
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
	//��� Ŭ���̾�Ʈ�� ������Ʈ (���� ��Ŷ �۽�)
	float time = TimeUtil::Get().GetTimef();

	if (time > mTimeOfLastStatePackets + mTimeBetweenStatePackets)
	{
		for (auto AddressToClient : mAddressToClientMap)
		{
			//Ÿ�Ӿƿ��� ��Ŷ ó��
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
	//�� �ʿ��� ����
	mAddressToClientMap.erase(InClientProxy->GetSocketAddress());
	mPlayerIdToClientMap.erase(InClientProxy->GetPlayerId());

	//��ŷ ���ؽ�Ʈ���� ����, �÷��̾� ������Ʈ ����
	static_cast<Server*> (Engine::sInstance.get())->HandleLostClient(InClientProxy);
}

void NetworkManagerServer::HandleConnectionReset(const SocketAddress& InFromAddress)
{
	//address �ʿ��� Ŭ���̾�Ʈ�� ã�Ƽ� �÷��̾�� �ش� Ŭ���̾�Ʈ ���Ͻø� ó������
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it != mAddressToClientMap.end())
	{
		HandleLostClient(it->second);
	}
}

void NetworkManagerServer::CheckForDisconnects()
{	
	std::vector<ClientProxyPtr> ClientToDisconnect;

	//������ �����Ǵ� Ŭ���̾�Ʈ�� ��� �� �ð� ������ ��Ŷ�� ���� ���� �־�� ��.
	float MinAllowedLastPacketFromClientTime = TimeUtil::Get().GetTimef() - mClientDisconnectTimeout;

	for (const auto& Pair : mAddressToClientMap)
	{
		//�ش� �ð� ������ ���� ��Ŷ�� �������̿��ٸ�, ������ ��Ͽ� �־���
		if (Pair.second->GetLastPacketFromClientTime() < MinAllowedLastPacketFromClientTime)
		{
			ClientToDisconnect.push_back(Pair.second);
		}
	}

	//������ Ŭ���̾�Ʈ ���Ͻ� ó��
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

	//�ϴ� �ӽô���.. RA ť�� ��ϵ� �� ���ø����̼ǽ�Ŵ
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
	//Move���� �о�ͼ� ó�� �ȵ� ���� ����Ʈ�� �־���
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