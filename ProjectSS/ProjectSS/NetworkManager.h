#pragma once
#include <stdint.h>
#include "UDPSocket.h"
class UDPSocket;
class OutputMemoryBitStream;
class SocketAddress;
static const uint32_t kHelloCC = 'HELO';
static const uint32_t kWelcomeCC = 'WLCM';
static const uint32_t kStateCC = 'STAT';
static const uint32_t kInputCC = 'INPT';

class NetworkManager
{
public:
	NetworkManager(){}
	virtual ~NetworkManager(){}

	void SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress);

private:
	UDPSocketPtr mSocket;
};