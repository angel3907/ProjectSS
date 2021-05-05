#include "stdafx.h"

int main()
{
	printf("this is server\n");

	GamePlayUtils::StartGame();;

	UDPSocketPtr UDPServerSocket = SocketUtil::CreateUDPSocket(INET);
	//UDPServerSocket->SetNonBlockingMode(true);

	SocketAddressPtr MyAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
	UDPServerSocket->Bind(*MyAddressPtr);
 
	SocketAddress FromSocketAddress;

	printf("Waiting for receiving..\n");

	SocketAddressPtr ToAddressPtr = SocketAddressFactory::CreateIPv4FromIPString("127.0.0.1:7000");
	if (ToAddressPtr == nullptr)
	{
		return -1;
	}

	uint32_t RecvByteCnt = SocketUtil::ReceivePacket(UDPServerSocket);

	//1, 2번일거고
	//Player 먼저쓰니까 Player가 1번
	Player* PlayerObj = nullptr;
	Star* StarObj = nullptr;

	Player* PlayerObj2 = nullptr;
	Star* StarObj2 = nullptr;
	
	printf("I Recved BitCnt %d\n", RecvByteCnt * 8);

	/*
	if (SentByteCnt > 0)
	{
		printf("I Sent ByteCnt %d\n", SentByteCnt);

		PlayerObj = static_cast<Player*>(LinkingContext::Get().GetGameObject(1));
		if (PlayerObj != nullptr)
		{ 
			printf("Player StarCnt : %d, TestValue : %d, Name : %s", PlayerObj->GetStarCount(), PlayerObj->GetTestValue(), PlayerObj->GetName().c_str());
			printf(" Pos : %f, %f\n", PlayerObj->GetPos().PosX, PlayerObj->GetPos().PosY);
		}

		StarObj = static_cast<Star*>(LinkingContext::Get().GetGameObject(2));
		if (StarObj != nullptr)
		{ 
			printf("Star Value : %d, IsHidden : %d", StarObj->GetStarStatus().Value, StarObj->GetStarStatus().bHidden);
			printf(" Pos : %f, %f", StarObj->GetPos().PosX, StarObj->GetPos().PosY);
		}

		PlayerObj2 = static_cast<Player*>(LinkingContext::Get().GetGameObject(3));
		if (PlayerObj2 != nullptr)
		{
			printf("Player StarCnt : %d, TestValue : %d, Name : %s", PlayerObj2->GetStarCount(), PlayerObj2->GetTestValue(), PlayerObj2->GetName().c_str());
			printf(" Pos : %f, %f\n", PlayerObj2->GetPos().PosX, PlayerObj2->GetPos().PosY);
		}

		StarObj2 = static_cast<Star*>(LinkingContext::Get().GetGameObject(4));
		if (StarObj2 != nullptr)
		{
			printf("Star Value : %d, IsHidden : %d", StarObj2->GetStarStatus().Value, StarObj2->GetStarStatus().bHidden);
			printf(" Pos : %f, %f", StarObj2->GetPos().PosX, StarObj2->GetPos().PosY);
		}
	}
	*/

	GamePlayUtils::EndGame();;
}