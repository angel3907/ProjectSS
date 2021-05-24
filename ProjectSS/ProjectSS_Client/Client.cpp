#include "stdafx.h"
#include "Client.h"
#include "PlayerClient.h"
#include "NetworkManagerClient.h"
#include "SDLRenderer.h"
#include "StarClient.h"
#include "ScoreboardManager.h"
#include "EntryScene.h"

void RegisterObjectCreation()
{
	//�����δ� PlayerClient, StarClient�� �ʿ��� ��
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerClient>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<StarClient>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}

void RegisterRPCs()
{
	//RPC �Լ� ���
	RPCManager::Get().RegisterUnwrapFunction('GPRT', &GameObject::UnwrapPrintSomething);
	RPCManager::Get().RegisterWrapFunction('GPRT', &GameObject::PrintSomething);
}

bool Client::StaticInit()
{
	//Ŭ���̾�Ʈ �����͸� �������. SDL�� ���� �ʱ�ȭ����� �ϱ� ����
	Client* Client_ = new Client();

	//SDL �ʱ�ȭ...
	SDLRenderer::Get().InitSDL();
	SDLRenderer::Get().LoadTextures();
	SDLRenderer::Get().LoadFonts();

	//�Է� �Ŵ��� �ʱ�ȭ���ִ°� �ִµ� �׳� ���� �̱��� ������
	
	sInstance.reset(Client_);

	//�� �ʱ�ȭ
	Client_->InitScene();

	return true;
}

void Client::InitScene()
{
	CurrentScene = &EntryScene::Get();
}

Client::Client()
{
	RegisterObjectCreation();
	RegisterRPCs();

	std::string destination = "127.0.0.1:7000";
	SocketAddressPtr ServerAddress = SocketAddressFactory::CreateIPv4FromIPString(destination);

	std::string PlayerName = "Player" + std::to_string(GetRandomInt(1, 1000));

	//�ּҿ� �̸����� ��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	NetworkManagerClient::StaticInit(*ServerAddress, PlayerName);
}

void Client::DoFrame()
{
	InputManager::Get().Update();

	Engine::DoFrame();

	//������ ��Ŷ ó��
	NetworkManagerClient::sInstance->ProcessInComingPacket();

	//�� ������Ʈ
	CurrentScene->Update();

	//������
	Render();

	//������ ��Ŷ ó��
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::Render()
{
	//���� Ŭ����
	SDLRenderer::Get().Clear();

	//��� ������Ʈ ��ο�
	for (auto GO : LinkingContext::Get().GetGameObjectSet())
	{
		GO->Render();
	}

	//�� ������
	CurrentScene->Render();

	//ǥ��
	SDLRenderer::Get().Present();
}

void Client::HandleEvent(SDL_Event* InEvent)
{
	CurrentScene->HandleInput(InEvent);
}