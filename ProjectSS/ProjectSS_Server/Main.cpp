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

// 	uint32_t RecvByteCnt = SocketUtil::ReceivePlayerWithBitStream(UDPServerSocket, &ServerPlayer);
// 	if (RecvByteCnt > 0)
// 	{ 
// 		printf("I Recv %d Bytes, Recv Star Count is %d, TestValue is %d, Name is %s\n", RecvByteCnt, ServerPlayer.GetStarCount(), 
// 			ServerPlayer.GetTestValue(), ServerPlayer.GetName().c_str());
// 		printf("Pos : %f, %f", ServerPlayer.GetPos().PosX, ServerPlayer.GetPos().PosY);
// 	}

	//서버에서 정보를 받을 별 객체를 하나 만듦.
	Star StarTest(StarStatus(1, false));
	StarStatus::InitDataType(); //적절한 타이밍에 데이터 타입 초기화

	uint32_t RecvByteCnt = SocketUtil::ReceivePODWithBitStream(UDPServerSocket, StarStatus::sDataType, (uint8_t*)(&StarTest.GetStarStatus()));

	if (RecvByteCnt > 0)
	{
		printf("I Recv ByteCnt %d\n", RecvByteCnt);
		printf("StarValue : %d, IsHidden : %d", StarTest.GetStarStatus().Value, StarTest.GetStarStatus().bHidden);
	}

	SocketUtil::EndUsingSocket();
}