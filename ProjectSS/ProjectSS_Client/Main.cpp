	#include "stdafx.h"

	int main()
	{
		printf("this is client\n");

		GamePlayUtils::StartGame();

		UDPSocketPtr UDPClientSocket = SocketUtil::CreateUDPSocket(INET);
		//UDPClientSocket->SetNonBlockingMode(true);

		SocketAddressPtr MyAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:8000");
		UDPClientSocket->Bind(*MyAddressPtr);
	
		SocketAddressPtr ToAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
		if (ToAddressPtr == nullptr)
		{
			return -1;
		}

		Player ServerPlayer(10, true, "Icon");
 		ServerPlayer.SetPos(Vector2(-223.7f, 491.2f));

		Star StarTest(StarStatus(3, true));
		StarTest.SetPos(Vector2(0, 30));

		Player ServerPlayer2(40, false, "This Is Me");
		ServerPlayer2.SetPos(Vector2(-300.f, 100.8f));

		Star StarTest2(StarStatus(1, false));
		StarTest2.SetPos(Vector2(-100.3f, -35.6f));

		ServerPlayer.SetProperties(PLR_Name | PLR_StarCount | PLR_TestValue);
		//vector<GameObject*> GameObjects = {&ServerPlayer, &StarTest, &ServerPlayer2, &StarTest2 };

		uint32_t SentByteCnt = SocketUtil::SendChangedGameObject(UDPClientSocket, *ToAddressPtr, &ServerPlayer, ReplicationAction::RA_Create);

		if (SentByteCnt > 0)
		{
	 		printf("I Sent ByteCnt %d\n", SentByteCnt);
		
			printf("Player StarCnt : %d, TestValue : %d, Name : %s", ServerPlayer.GetStarCount(), ServerPlayer.GetTestValue(), ServerPlayer.GetName().c_str());
			printf(" Pos : %f, %f\n", ServerPlayer.GetPos().PosX, ServerPlayer.GetPos().PosY);
		} 

		ServerPlayer.SetPos(Vector2(-111.7f, 200.2f));

		SentByteCnt = SocketUtil::SendChangedGameObject(UDPClientSocket, *ToAddressPtr, &ServerPlayer, ReplicationAction::RA_Update);

		if (SentByteCnt > 0)
		{
			printf("I Sent ByteCnt %d\n", SentByteCnt);

			printf("Player StarCnt : %d, TestValue : %d, Name : %s", ServerPlayer.GetStarCount(), ServerPlayer.GetTestValue(), ServerPlayer.GetName().c_str());
			printf(" Pos : %f, %f\n", ServerPlayer.GetPos().PosX, ServerPlayer.GetPos().PosY);
		}

		SentByteCnt = SocketUtil::SendChangedGameObject(UDPClientSocket, *ToAddressPtr, &ServerPlayer, ReplicationAction::RA_Destroy);

		if (SentByteCnt > 0)
		{
			printf("I Sent ByteCnt %d\n", SentByteCnt);
			printf("I Destroyed Player\n");
		}

	// 	uint32_t SentByteCnt = SocketUtil::SendPacket(UDPClientSocket, *ToAddressPtr, GameObjects);
	// 
	// 	//printf("I Sent BitCnt %d\n", SentByteCnt*8);
	// 	
	// 	if (SentByteCnt > 0)
	// 	{
	// 	 	printf("I Sent ByteCnt %d\n", SentByteCnt);
	// 
	// 	 	printf("Player StarCnt : %d, TestValue : %d, Name : %s", ServerPlayer.GetStarCount(), ServerPlayer.GetTestValue(), ServerPlayer.GetName().c_str());
	// 	 	printf(" Pos : %f, %f\n", ServerPlayer.GetPos().PosX, ServerPlayer.GetPos().PosY);
	// 
	// 		printf("Star Value : %d, IsHidden : %d", StarTest.GetStarStatus().Value, StarTest.GetStarStatus().bHidden);
	// 		printf(" Pos : %f, %f\n", StarTest.GetPos().PosX, StarTest.GetPos().PosY);
	// 
	// 		printf("Player StarCnt : %d, TestValue : %d, Name : %s", ServerPlayer2.GetStarCount(), ServerPlayer2.GetTestValue(), ServerPlayer2.GetName().c_str());
	// 		printf(" Pos : %f, %f\n", ServerPlayer2.GetPos().PosX, ServerPlayer2.GetPos().PosY);
	// 
	// 		printf("Star Value : %d, IsHidden : %d", StarTest2.GetStarStatus().Value, StarTest2.GetStarStatus().bHidden);
	// 		printf(" Pos : %f, %f\n", StarTest2.GetPos().PosX, StarTest2.GetPos().PosY);
	// 	}
	// 	

		GamePlayUtils::EndGame();
	}