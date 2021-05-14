#include "stdafx.h"
#include "Server.h"
#include "NetworkManagerServer.h"
#include "PlayerServer.h"

void RegisterObjectCreation()
{
	//�����δ� PlayerServer, StarServer�� �ʿ��� ��
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerServer>();
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
	
	InitNetworkManager();
}

void Server::DoFrame()
{
	//��Ŷ �޾ƿ��� �Լ� ȣ��
	NetworkManagerServer::sInstance->ProcessInComingPacket();

	//���� ���� ����
	Engine::DoFrame();

	//���� ������Ʈ ������Ʈ
	UpdateGameObjects();

	//���ø����̼� ��Ŷ ����
	NetworkManagerServer::sInstance->SendOutgoingPackets();

	//Ŭ���̾�Ʈ ���� ��Ŷ ������
	NetworkManagerServer::sInstance->UpdateAllClients();
}

int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

void Server::HandleNewClient(ClientProxyPtr InClientProxy)
{
	//�ش� �÷��̾� ���̵�� �÷��̾� ���� 
	int PlayerId = InClientProxy->GetPlayerId();
	SpawnPlayer(PlayerId);
}

void Server::HandleLostClient(ClientProxyPtr InClientProxy)
{
	//�÷��̾ ����
	int PlayerId = InClientProxy->GetPlayerId();

	Player* Player_ = GetPlayerWithPlayerId(PlayerId);
	if (Player_)
	{
		//�÷��̾� ���� ����.. �ٸ� ��ϵ� ��(LinkingContext)������ ��������� 
		//�׸��� ���ø����̼��� ��Ͽ� �߰��ؾ� ��
		ReplicationCommand RC;
		RC.NetworkId = LinkingContext::Get().GetNetworkId(Player_, false);
		RC.RA = ReplicationAction::RA_Destroy;

		LinkingContext::Get().AddUnprocessedRA(RC);
		delete Player_; //���⼭ LinkingContext������ ����.
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
	//static_pointer_cast �Լ��� std::shared_ptr�� ����Ű�� ��ü ���� ��ȯ�� �� ����.
	PlayerServer* Player_ = static_cast<PlayerServer*>(ObjectCreationRegistry::Get().CreateGameObject('PLYR'));
	
	if (Player_)
	{ 
		//��ŷ ���ؽ�Ʈ�� �߰�
		int NetworkId = LinkingContext::Get().GetNetworkId(Player_, true);

		//�÷��̾� �Ӽ� ���� - ���̵� ����, ��Ʈ��ũ ���̵� ����, ��ġ ����
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
