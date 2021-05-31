#include "stdafx.h"
#include "StarManager.h"
#include "Server.h"
#include "NetworkManagerServer.h"
#include "PlayerServer.h"

void RegisterObjectCreation()
{
	//실제로는 PlayerServer, StarServer가 필요할 것
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerServer>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<StarServer>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}

void RegisterRPCs()
{
	//RPC 함수 등록
	RPCManager::Get().RegisterUnwrapFunction('GPRT', &GameObject::UnwrapPrintSomething);
	RPCManager::Get().RegisterWrapFunction('GPRT', &GameObject::PrintSomething);
}


bool Server::StaticInit()
{
	//reset은 내부 포인터의 메모리를 해제하고 nullptr로 변경함.
	//이때 인자를 new ~ 로 넣어주면 새로운 메모리 할당이 가능.
	sInstance.reset(new Server());
	return true;
}

Server::Server()
{
	RegisterObjectCreation();
	RegisterRPCs();
	InitGameState();
	InitNetworkManager();
}

void Server::DoFrame()
{
	//게임 루프 수행
	Engine::DoFrame();

	//리플리케이션 패킷 전송
	NetworkManagerServer::sInstance->SendOutgoingPackets();

	//패킷 받아오는 함수 호출
	NetworkManagerServer::sInstance->ProcessInComingPacket();

	//게임 오브젝트 업데이트
	UpdateGameObjects();

	//클라이언트 연결 상태 체크
	NetworkManagerServer::sInstance->CheckForDisconnects();

	//클라이언트 상태 패킷 보내기
	NetworkManagerServer::sInstance->UpdateAllClients();

	//게임 시작 이후 해야하는 일
	DoGameStartLoop();
}

int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

void Server::DoGameStartLoop()
{
	if (bGameStarted)
	{
		StarManager::Get().Update();
	}
}

void Server::HandleNewClient(ClientProxyPtr InClientProxy)
{
	//해당 클라 프록시 정보(이름, 아이디)로 플레이어 생성 
	SpawnPlayer(InClientProxy);
	PlayerNum++;

	//2명이 되었다면, Ready 가능을 모두에게 보내기
	if (PlayerNum == 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacketToAllClient(READY_ACTIVE);
	}
	//2명을 넘는다면 새로 온 클라에게만 보내기
	else if (PlayerNum > 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacket(InClientProxy, READY_ACTIVE);
	}
}

void Server::HandleLostClient(ClientProxyPtr InClientProxy)
{
	//플레이어를 삭제
	int PlayerId = InClientProxy->GetPlayerId();

	Player* Player_ = GetPlayerWithPlayerId(PlayerId);
	if (Player_)
	{
		ReplicationCommand RC;
		RC.NetworkId = LinkingContext::Get().GetNetworkId(Player_, false);
		RC.RA = ReplicationAction::RA_Destroy;

		//모든 클라 프록시에게 삭제 액션 리플리케이션
		NetworkManagerServer::sInstance->SendReplicatedToAllClients(RC.RA, Player_, nullptr);

		delete Player_; //여기서 LinkingContext에서도 빠짐.
	}

	PlayerNum--;

	if (PlayerNum < 0)
	{
		printf("Error : PlayerNum is negative number at Server::HandleLostClient. \n");
	}

	//플레이어가 2명 미만이 됐으면 알려주기
	if (PlayerNum < 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacketToAllClient(READY_NONACTIVE);
	}

	//해당 플레이어가 나가면서 준비가 다 되었다면 모두에게 게임 시작을 알려주기
	//TODO : 나가면 전부 Ready를 푸는 것도 방법인 것 같다.
	CheckReady();
	if (IsGameStarted())
	{
		NetworkManagerServer::sInstance->SendReadyPacketToAllClient(START);
	}
}

Player* Server::GetPlayerWithPlayerId(int InPlayerId)
{
	for (auto GO : LinkingContext::Get().GetGameObjectSet())
	{
		Player* Player_ = static_cast<Player*>(GO);
		if (Player_ && Player_->GetPlayerId() == InPlayerId)
		{
			return Player_;
		}
	}

	return nullptr;
}

void Server::SpawnPlayer(ClientProxyPtr InClientProxy)
{
	//static_pointer_cast 함수로 std::shared_ptr이 가리키는 객체 형을 변환할 수 있음.
	PlayerServer* Player_ = static_cast<PlayerServer*>(ObjectCreationRegistry::Get().CreateGameObject('PLYR'));
	
	if (Player_)
	{ 
		//링킹 컨텍스트에 추가
		int NetworkId = LinkingContext::Get().GetNetworkId(Player_, true);

		//플레이어 속성 설정 - 아이디 설정, 위치 설정
		Player_->SetPlayerId(InClientProxy->GetPlayerId());
		Player_->SetPos(Vector2(0,0));
		Player_->SetName(InClientProxy->GetName());
		Player_->SetPlayerColor(InClientProxy->GetPlayerColor());
	}
	else
	{
		LOG("Failed To Spawn Player ID : %d", InClientProxy->GetPlayerId());
	}
}

void Server::CheckReady()
{
	if (NetworkManagerServer::sInstance->IsAllPlayersReady())
	{
		bGameStarted = true;
	}
}

bool Server::InitNetworkManager()
{
	//포트번호는 일단 고정
	uint16_t Port = 7000;

	return NetworkManagerServer::StaticInit(Port);
}

void Server::SetupWorld()
{
	//월드 관련 세팅 (초기 별 생성 등..)
}

void Server::UpdateGameObjects()
{
	for (auto GO : LinkingContext::Get().GetGameObjectSet())
	{
		GO->Update();
	}
}

void Server::InitGameState()
{
	bGameStarted = false; //TODO 수정
	PlayerNum = 0;
}
