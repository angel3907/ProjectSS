#include "stdafx.h"

int main()
{
	printf("this is client\n");

	SocketUtil::StartUsingSocket();

	UDPSocketPtr UDPClientSocket = SocketUtil::CreateUDPSocket(INET);
	//UDPClientSocket->SetNonBlockingMode(true);

	SocketAddressPtr MyAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:8000");
	UDPClientSocket->Bind(*MyAddressPtr);

	Player ServerPlayer(10);
	SocketAddressPtr ToAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
	if (ToAddressPtr == nullptr)
	{ 
		return -1;
	}

	int SentByteCnt = UDPClientSocket->SendTo(&ServerPlayer, sizeof(ServerPlayer), *ToAddressPtr);

	if (SentByteCnt > 0)
	{
		printf("I Sent ByteCnt %d\n", SentByteCnt);
	}

	SocketUtil::EndUsingSocket();
}