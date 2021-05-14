#include "stdafx.h"
#include "Server.h"
#include "NetworkManagerServer.h"
#include "PlayerServer.h"

void RegisterObjectCreation()
{
	//실제로는 PlayerServer, StarServer가 필요할 것
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerServer>();
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
	
	InitNetworkManager();
}

void Server::DoFrame()
{
	//패킷 받아오는 함수 호출
	NetworkManagerServer::sInstance->ProcessInComingPacket();

	//게임 루프 수행
	Engine::DoFrame();

	//게임 오브젝트 업데이트
	UpdateGameObjects();

	//리플리케이션 패킷 전송
	NetworkManagerServer::sInstance->SendOutgoingPackets();

	//클라이언트 상태 패킷 보내기
	NetworkManagerServer::sInstance->UpdateAllClients();
}

int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

void Server::HandleNewClient(ClientProxyPtr InClientProxy)
{
	//해당 플레이어 아이디로 플레이어 생성 
	int PlayerId = InClientProxy->GetPlayerId();
	SpawnPlayer(PlayerId);
}

void Server::HandleLostClient(ClientProxyPtr InClientProxy)
{
	//플레이어를 삭제
	int PlayerId = InClientProxy->GetPlayerId();

	Player* Player_ = GetPlayerWithPlayerId(PlayerId);
	if (Player_)
	{
		//플레이어 삭제 예약.. 다른 등록된 곳(LinkingContext)에서도 없애줘야함 
		//그리고 리플리케이션할 목록에 추가해야 함
		ReplicationCommand RC;
		RC.NetworkId = LinkingContext::Get().GetNetworkId(Player_, false);
		RC.RA = ReplicationAction::RA_Destroy;

		LinkingContext::Get().AddUnprocessedRA(RC);
		delete Player_; //여기서 LinkingContext에서도 빠짐.
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

void Server::SpawnPlayer(int InPlayerId)
{
	//static_pointer_cast 함수로 std::shared_ptr이 가리키는 객체 형을 변환할 수 있음.
	PlayerServer* Player_ = static_cast<PlayerServer*>(ObjectCreationRegistry::Get().CreateGameObject('PLYR'));
	
	if (Player_)
	{ 
		//링킹 컨텍스트에 추가
		int NetworkId = LinkingContext::Get().GetNetworkId(Player_, true);

		//플레이어 속성 설정 - 아이디 설정, 네트워크 아이디 설정, 위치 설정
		Player_->SetPlayerId(InPlayerId);
		Player_->SetNetworkId(NetworkId);
		Player_->SetPos(Vector2(0,0));
	}
	else
	{
		LOG("Failed To Spawn Player ID : %d", InPlayerId);
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
