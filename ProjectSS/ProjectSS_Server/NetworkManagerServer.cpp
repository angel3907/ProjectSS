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
		string Name;
		InInputStream.Read(Name);

		//Ŭ���̾�Ʈ ���Ͻ� ����
		ClientProxyPtr NewClientProxy = std::make_shared<ClientProxy>(InFromAddress, Name, mNewPlayerId);

		//�� �ʿ� �߰�
		mAddressToClientMap[InFromAddress] = NewClientProxy;
		mPlayerIdToClientMap[NewClientProxy->GetPlayerId()] = NewClientProxy;

		//Ŭ���̾�Ʈ ����...
		SendWelcomePacket(NewClientProxy);

		//Ŭ���̾�Ʈ�� ���� �÷��̾� ����
		static_cast<Server*> (Engine::sInstance.get())->HandleNewClient(NewClientProxy);

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

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	//������Ŷ�� Ư�� Ŭ���̾�Ʈ���� �۽�
	OutputMemoryBitStream StatePacket;

	//�ϴ� �ӽô���.. RA ť�� ��ϵ� �� ���ø����̼ǽ�Ŵ
	for (auto RA : LinkingContext::Get().GetUnprocessedRAs())
	{
		GameObject* GO = LinkingContext::Get().GetGameObject(RA.NerworkId);
		SendReplicated(InClientProxy->GetSocketAddress(), InClientProxy->GetReplicationManagerServer(), RA.RA, GO, nullptr);
	}

	LinkingContext::Get().ClearUnprocessedRAs();
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

		if (InClientProxy->GetUnprocessedMoveList().AddMove(MoveValue))
		{
			//InClientProxy->SetIsLastMoveTimestampDirty(true);
		}
	}
}
