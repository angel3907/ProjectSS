#include "stdafx.h"
#include "Client.h"
#include "PlayerClient.h"
#include "NetworkManagerClient.h"
#include "SDLRenderer.h"
#include "StarClient.h"
#include "ScoreboardManager.h"

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

	return true;
}

Client::Client()
{
	RegisterObjectCreation();
	RegisterRPCs();

	std::string destination = "127.0.0.1:7000";
	SocketAddressPtr ServerAddress = SocketAddressFactory::CreateIPv4FromIPString(destination);

	std::string PlayerName = "MyName";

	//�ּҿ� �̸����� ��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	NetworkManagerClient::StaticInit(*ServerAddress, PlayerName);
}

void Client::DoFrame()
{
	InputManager::Get().Update();

	Engine::DoFrame();

	//������ ��Ŷ ó��
	NetworkManagerClient::sInstance->ProcessInComingPacket();

	//������
	Render();

	//������ ��Ŷ ó��
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::Render()
{
	//���� Ŭ����
	SDLRenderer::Get().Clear();

	//��ο�
	SDLRenderer::Get().DrawBackground();

	//��� ������Ʈ ��ο�
	for (auto GO : LinkingContext::Get().GetGameObjectSet())
	{
		GO->Render();
	}

	//���ھ� ���� ������
	ScoreboardManager::Get().RenderScoreborad();

	//ǥ��
	SDLRenderer::Get().Present();
}

void Client::HandleEvent(SDL_Event* InEvent)
{
	switch(InEvent->type)
	{
	case SDL_KEYDOWN : 
		InputManager::Get().HandleInput(EIA_Pressed, InEvent->key.keysym.sym);
		break;
	case SDL_KEYUP:
		InputManager::Get().HandleInput(EIA_Released, InEvent->key.keysym.sym);
		break;
	default:
		break;
	}
}
