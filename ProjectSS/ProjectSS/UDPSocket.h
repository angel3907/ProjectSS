#pragma once
#include <WinSock2.h>
#include <memory>
class SocketUtil;
class SocketAddress;

class UDPSocket
{
public:
	~UDPSocket();
	int Bind(const SocketAddress& InBindAddress);
	int SendTo(const void* InData, int InLen, const SocketAddress& InToAddress);
	int ReceiveFrom(void* InBuffer, int InMaxLength, SocketAddress& outFromAddress);
	int SetNonBlockingMode(bool InShouldBeNonBlocking);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET InSocket) : mSocket(InSocket){}
	SOCKET mSocket;
};

using UDPSocketPtr = std::shared_ptr<UDPSocket>;