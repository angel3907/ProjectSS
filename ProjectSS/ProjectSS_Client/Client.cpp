#include "stdafx.h"
#include "Client.h"
#include "NetworkManagerClient.h"

void RegisterObjectCreation()
{
	//실제로는 PlayerClient, StarClient가 필요할 것
	ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Player>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<Star>();
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

	//입력 매니저 초기화해주는거 있는데 그냥 나는 싱글턴 선언함

	sInstance.reset(Client_);

	return true;
}

Client::Client()
{
	RegisterObjectCreation();
	RegisterRPCs();

	string destination = "10.99.0.28:7000";
	SocketAddressPtr ServerAddress = SocketAddressFactory::CreateIPv4FromIPString(destination);

	string PlayerName = "MyName";

	//주소와 이름으로 네트워크 매니저 초기화
	NetworkManagerClient::StaticInit(*ServerAddress, PlayerName);
}

void Client::DoFrame()
{
	InputManager::Get().Update();

	Engine::DoFrame();

	//들어오는 패킷 처리
	//NetworkManagerClient::sInstance->ProcessPacket()

	//렌더링

	//나가는 패킷 처리
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}
