#include "stdafx.h"

int main()
{
	printf("this is server\n");

	SocketUtil::StartUsingSocket();

	UDPSocketPtr UDPServerSocket = SocketUtil::CreateUDPSocket(INET);
	//UDPServerSocket->SetNonBlockingMode(true);

	SocketAddressPtr MyAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
	UDPServerSocket->Bind(*MyAddressPtr);
 
	Player ServerPlayer;
	SocketAddress FromSocketAddress;

	printf("Waiting for receiving..\n");
	int RecvByteCnt = UDPServerSocket->ReceiveFrom(&ServerPlayer, sizeof(ServerPlayer), FromSocketAddress);

	if (RecvByteCnt > 0)
	{
		printf("Recv Star Count is %d\n", ServerPlayer.GetStarCount());
	}

	SocketUtil::EndUsingSocket();
}