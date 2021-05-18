#include "stdafx.h"
#include "Client.h"
#include "PlayerClient.h"
#include "NetworkManagerClient.h"
#include "SDLRenderer.h"
#include "StarClient.h"
#include "ScoreboardManager.h"

void RegisterObjectCreation()
{
	//실제로는 PlayerClient, StarClient가 필요할 것
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerClient>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<StarClient>();
	//ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
}

void RegisterRPCs()
{
	//RPC 함수 등록
	RPCManager::Get().RegisterUnwrapFunction('GPRT', &GameObject::UnwrapPrintSomething);
	RPCManager::Get().RegisterWrapFunction('GPRT', &GameObject::PrintSomething);
}

bool Client::StaticInit()
{
	//클라이언트 포인터를 만들어줌. SDL을 먼저 초기화해줘야 하기 때문
	Client* Client_ = new Client();

	//SDL 초기화...
	SDLRenderer::Get().InitSDL();
	SDLRenderer::Get().LoadTextures();
	SDLRenderer::Get().LoadFonts();

	//입력 매니저 초기화해주는거 있는데 그냥 나는 싱글턴 선언함

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

	//주소와 이름으로 네트워크 매니저 초기화
	NetworkManagerClient::StaticInit(*ServerAddress, PlayerName);
}

void Client::DoFrame()
{
	InputManager::Get().Update();

	Engine::DoFrame();

	//들어오는 패킷 처리
	NetworkManagerClient::sInstance->ProcessInComingPacket();

	//렌더링
	Render();

	//나가는 패킷 처리
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::Render()
{
	//렌더 클리어
	SDLRenderer::Get().Clear();

	//드로우
	SDLRenderer::Get().DrawBackground();

	//모든 오브젝트 드로우
	for (auto GO : LinkingContext::Get().GetGameObjectSet())
	{
		GO->Render();
	}

	//스코어 보드 렌더링
	ScoreboardManager::Get().RenderScoreborad();

	//표시
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
