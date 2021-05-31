#include "stdafx.h"
#include "StarManager.h"
#include "Server.h"
#include "NetworkManagerServer.h"
#include "PlayerServer.h"

void RegisterObjectCreation()
{
	//�����δ� PlayerServer, StarServer�� �ʿ��� ��
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerServer>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<StarServer>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}

void RegisterRPCs()
{
	//RPC �Լ� ���
	RPCManager::Get().RegisterUnwrapFunction('GPRT', &GameObject::UnwrapPrintSomething);
	RPCManager::Get().RegisterWrapFunction('GPRT', &GameObject::PrintSomething);
}


bool Server::StaticInit()
{
	//reset�� ���� �������� �޸𸮸� �����ϰ� nullptr�� ������.
	//�̶� ���ڸ� new ~ �� �־��ָ� ���ο� �޸� �Ҵ��� ����.
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
	//���� ���� ����
	Engine::DoFrame();

	//���ø����̼� ��Ŷ ����
	NetworkManagerServer::sInstance->SendOutgoingPackets();

	//��Ŷ �޾ƿ��� �Լ� ȣ��
	NetworkManagerServer::sInstance->ProcessInComingPacket();

	//���� ������Ʈ ������Ʈ
	UpdateGameObjects();

	//Ŭ���̾�Ʈ ���� ���� üũ
	NetworkManagerServer::sInstance->CheckForDisconnects();

	//Ŭ���̾�Ʈ ���� ��Ŷ ������
	NetworkManagerServer::sInstance->UpdateAllClients();

	//���� ���� ���� �ؾ��ϴ� ��
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
	//�ش� Ŭ�� ���Ͻ� ����(�̸�, ���̵�)�� �÷��̾� ���� 
	SpawnPlayer(InClientProxy);
	PlayerNum++;

	//2���� �Ǿ��ٸ�, Ready ������ ��ο��� ������
	if (PlayerNum == 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacketToAllClient(READY_ACTIVE);
	}
	//2���� �Ѵ´ٸ� ���� �� Ŭ�󿡰Ը� ������
	else if (PlayerNum > 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacket(InClientProxy, READY_ACTIVE);
	}
}

void Server::HandleLostClient(ClientProxyPtr InClientProxy)
{
	//�÷��̾ ����
	int PlayerId = InClientProxy->GetPlayerId();

	Player* Player_ = GetPlayerWithPlayerId(PlayerId);
	if (Player_)
	{
		ReplicationCommand RC;
		RC.NetworkId = LinkingContext::Get().GetNetworkId(Player_, false);
		RC.RA = ReplicationAction::RA_Destroy;

		//��� Ŭ�� ���Ͻÿ��� ���� �׼� ���ø����̼�
		NetworkManagerServer::sInstance->SendReplicatedToAllClients(RC.RA, Player_, nullptr);

		delete Player_; //���⼭ LinkingContext������ ����.
	}

	PlayerNum--;

	if (PlayerNum < 0)
	{
		printf("Error : PlayerNum is negative number at Server::HandleLostClient. \n");
	}

	//�÷��̾ 2�� �̸��� ������ �˷��ֱ�
	if (PlayerNum < 2)
	{
		NetworkManagerServer::sInstance->SendReadyPacketToAllClient(READY_NONACTIVE);
	}

	//�ش� �÷��̾ �����鼭 �غ� �� �Ǿ��ٸ� ��ο��� ���� ������ �˷��ֱ�
	//TODO : ������ ���� Ready�� Ǫ�� �͵� ����� �� ����.
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
	//static_pointer_cast �Լ��� std::shared_ptr�� ����Ű�� ��ü ���� ��ȯ�� �� ����.
	PlayerServer* Player_ = static_cast<PlayerServer*>(ObjectCreationRegistry::Get().CreateGameObject('PLYR'));
	
	if (Player_)
	{ 
		//��ŷ ���ؽ�Ʈ�� �߰�
		int NetworkId = LinkingContext::Get().GetNetworkId(Player_, true);

		//�÷��̾� �Ӽ� ���� - ���̵� ����, ��ġ ����
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
	//��Ʈ��ȣ�� �ϴ� ����
	uint16_t Port = 7000;

	return NetworkManagerServer::StaticInit(Port);
}

void Server::SetupWorld()
{
	//���� ���� ���� (�ʱ� �� ���� ��..)
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
	bGameStarted = false; //TODO ����
	PlayerNum = 0;
}
