#pragma once
#include "NetworkManager.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include <unordered_map>
#include "ClientProxy.h"

class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer& Get()
	{
		static NetworkManagerServer sInstance;
		return sInstance;
	}

	NetworkManagerServer(){}
	virtual ~NetworkManagerServer(){}

	void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);
	void HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);
	
	void ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);
	void SendWelcomePacket(ClientProxyPtr InClientProxy);

	void SendOutgoingPackets();
	void UpdateAllClients();
	void SendStatePacketToClient(ClientProxyPtr InClientProxy);

private:
	//unordered_map<SocketAddress, ClientProxyPtr> mAddressToClientMap;
};