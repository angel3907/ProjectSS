#include "NetworkManagerServer.h"
#include "stdafx.h"
#include "ClientProxy.h"
#include "Server.h"

NetworkManagerServer* NetworkManagerServer::sInstance;

NetworkManagerServer::NetworkManagerServer() : 
	mNewPlayerId(1),
	mTimeBetweenStatePackets(0.033f),
	mTimeOfLastStatePackets(0.f),
	mClientDisconnectTimeout(3.0f)
{

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
				ClientProxyValue.second->AddUnprocessedRA(RA);
			}
		}

		//�� Ŭ�� ���� ���ø����̼� �����ڸ� �����ͼ�
		//�׸��� ���ݱ��� ���忡 �ִ� �� ��� �������� ���ø����̼�����.
		for (auto GO : LinkingContext::Get().GetGameObjectSet())
		{
			SendReplicated(InFromAddress, NewClientProxy->GetReplicationManagerServer(), ReplicationAction::RA_Create, GO, nullptr);	
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
	case kInputCC:	
		//�Է� ��Ŷ ó��
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
	//�̵� ��� ���� ��Ŷ�� ����
	//�̵� ���°� ���� �� ������...
	for (auto AddressToClient : mAddressToClientMap)
	{
		ClientProxyPtr ClientProxyValue = AddressToClient.second;
		
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

void NetworkManagerServer::UpdateAllClients()
{
	//��� Ŭ���̾�Ʈ�� ������Ʈ (���� ��Ŷ �۽�)
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

void NetworkManagerServer::AddUnprocessedRAToAllClients(ReplicationCommand& RA)
{
	for (auto AddressToClientProxy : mAddressToClientMap)
	{
		AddressToClientProxy.second->AddUnprocessedRA(RA);
	}
}

void NetworkManagerServer::SendReplicatedToAllClients(ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams)
{
	for (auto AddressToClientProxy : mAddressToClientMap)
	{
		ClientProxyPtr Cp = AddressToClientProxy.second;
		SendReplicated(Cp->GetSocketAddress(), Cp->GetReplicationManagerServer(), InReplicationAction, InGameObject, InRPCParams);
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

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	//������Ŷ�� Ư�� Ŭ���̾�Ʈ���� �۽�
	OutputMemoryBitStream StatePacket;

	//�ϴ� �ӽô���.. RA ť�� ��ϵ� �� ���ø����̼ǽ�Ŵ
	for (auto RA : InClientProxy->GetUnprocessedRAs())
	{
		GameObject* GO = LinkingContext::Get().GetGameObject(RA.NetworkId);
		SendReplicated(InClientProxy->GetSocketAddress(), InClientProxy->GetReplicationManagerServer(), RA.RA, GO, nullptr);
		printf("Update Client : GameObject Network Id %d\n", RA.NetworkId);
	}

	InClientProxy->ClearUnprocessedRAs();
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
