#include "NetworkManagerServer.h"
#include "stdafx.h"
#include "ClientProxy.h"

void NetworkManagerServer::ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	//�̹� �ƴ� Ŭ���̾�Ʈ���� �˻�
	/*
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it == mAddressToClientMap.end()) //ó�� ����� Ŭ���̾�Ʈ��
	{
		HandlePacketFromNewClient(InInputStream, InFromAddress);
	}
	else
	{
		ProcessPacket((*it).second, InInputStream);
	}
	*/
}

void NetworkManagerServer::HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	uint32_t PacketType;
	InInputStream.Read(PacketType);

	if (PacketType == kHelloCC)
	{
		//�� Ŭ���̾�Ʈ��� Hello�� ���� �޾ƾ� ��
		string name;
		InInputStream.Read(name);

		//Ŭ���̾�Ʈ ���Ͻ� ����
		//...
		//ClientProxyPtr NewClientProxy = new ClientProxy();
		

		//Ŭ���̾�Ʈ ����...
		//SendWelcomePacket(NewClientProxy);

		//�� Ŭ�� ���� ���ø����̼� ������ ����
		//...
	}
	else
	{
		//��Ŷ�� �߸��ƴٰ� �α� ����
		LOG("Bad Incoming packet from unknown client at socket %s", InFromAddress.ToString().c_str());
	}
}

void NetworkManagerServer::ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	//TODO : ��Ŷ ������ ���� ó��
}


void NetworkManagerServer::SendWelcomePacket(ClientProxyPtr InClientProxy)
{
	OutputMemoryBitStream WelcomePacket;

	WelcomePacket.Write(kWelcomeCC);
	//WelcomePacket.Write(InClientProxy->GetPlayerId());

	//LOG("Server Welcoming, new Client '%s' as player %d", InClientProxy->GetName().c_str(), InClientProxy->GetPlayerId());

	//SendPacket(WelcomePacket, InClientProxy->GetSocketAddress());
}

void NetworkManagerServer::SendOutgoingPackets()
{
	//TODO ������ ��Ŷ�� ����
}

void NetworkManagerServer::UpdateAllClients()
{
	//TODO : ��� Ŭ���̾�Ʈ�� ������Ʈ (���� ��Ŷ �۽�)
}

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	//TODO : ������Ŷ�� Ư�� Ŭ���̾�Ʈ���� �۽�
}
