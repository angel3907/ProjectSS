#include "NetworkManagerServer.h"
#include "stdafx.h"
#include "ClientProxy.h"

void NetworkManagerServer::ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress)
{
	//이미 아는 클라이언트인지 검사
	/*
	auto it = mAddressToClientMap.find(InFromAddress);
	if (it == mAddressToClientMap.end()) //처음 통신한 클라이언트면
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
		//새 클라이언트라면 Hello를 먼저 받아야 함
		string name;
		InInputStream.Read(name);

		//클라이언트 프록시 생성
		//...
		//ClientProxyPtr NewClientProxy = new ClientProxy();
		

		//클라이언트 마중...
		//SendWelcomePacket(NewClientProxy);

		//이 클라에 대한 리플리케이션 관리자 생성
		//...
	}
	else
	{
		//패킷이 잘못됐다고 로그 남김
		LOG("Bad Incoming packet from unknown client at socket %s", InFromAddress.ToString().c_str());
	}
}

void NetworkManagerServer::ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream)
{
	//TODO : 패킷 종류에 따라 처리
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
	//TODO 나가는 패킷을 보냄
}

void NetworkManagerServer::UpdateAllClients()
{
	//TODO : 모든 클라이언트를 업데이트 (상태 패킷 송신)
}

void NetworkManagerServer::SendStatePacketToClient(ClientProxyPtr InClientProxy)
{
	//TODO : 상태패킷을 특정 클라이언트에게 송신
}
