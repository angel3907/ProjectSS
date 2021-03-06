#pragma once
#include "stdafx.h"

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& InAddress);
	int Bind (const SocketAddress& InToAddress);
	int Listen(int InBackLog = 32);
	TCPSocket* Accept(SocketAddress& InFromAddress);
	int Send (const void* InData, int InLen);
	int Receive(void* InBuffer, int InLen);

private:
	friend class SocketUtil;
	TCPSocket(SOCKET InSocket) : mSocket(InSocket) {}
	SOCKET mSocket;
};