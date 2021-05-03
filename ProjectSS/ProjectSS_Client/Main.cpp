#include "stdafx.h"

int main()
{
	printf("this is client\n");

	SocketUtil::StartUsingSocket();

	UDPSocketPtr UDPClientSocket = SocketUtil::CreateUDPSocket(INET);
	//UDPClientSocket->SetNonBlockingMode(true);

	SocketAddressPtr MyAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:8000");
	UDPClientSocket->Bind(*MyAddressPtr);

	Player ServerPlayer(10, true, "Icon");
//	ServerPlayer.SetPos(Vector2(-500, 500));
// 	ServerPlayer.SetPos(Vector2(500, -500));
//	ServerPlayer.SetPos(Vector2(255.5f, -493.2f));
 	ServerPlayer.SetPos(Vector2(-223.7f, 491.2f));

	SocketAddressPtr ToAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
	if (ToAddressPtr == nullptr)
	{ 
		return -1;
	}

	uint32_t SentByteCnt = SocketUtil::SendPlayerWithBitStream(UDPClientSocket, *ToAddressPtr, &ServerPlayer);

	if (SentByteCnt > 0)
	{
		printf("I Sent ByteCnt %d\n", SentByteCnt);
		printf("StarCnt : %d, TestValue : %d, Name : %s", ServerPlayer.GetStarCount(), ServerPlayer.GetTestValue(), ServerPlayer.GetName().c_str());
		printf("Pos : %f, %f", ServerPlayer.GetPos().PosX, ServerPlayer.GetPos().PosY);
	}

	SocketUtil::EndUsingSocket();
}