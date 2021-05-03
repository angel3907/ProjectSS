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

	uint32_t RecvByteCnt = SocketUtil::ReceivePlayerWithBitStream(UDPServerSocket, &ServerPlayer);
	if (RecvByteCnt > 0)
	{ 
		printf("Recv Star Count is %d, TestValue is %d\n", ServerPlayer.GetStarCount(), ServerPlayer.GetTestValue());
	}

	SocketUtil::EndUsingSocket();
}