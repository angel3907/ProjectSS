#include "stdafx.h"
#include "Client.h"
#include "PlayerClient.h"
#include "NetworkManagerClient.h"

void RegisterObjectCreation()
{
	//�����δ� PlayerClient, StarClient�� �ʿ��� ��
	//ObjectCreationRegistry::Get().RegisterCreationFunction<GameObject>();
	ObjectCreationRegistry::Get().RegisterCreationFunction<PlayerClient>();
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

	//�Է� �Ŵ��� �ʱ�ȭ���ִ°� �ִµ� �׳� ���� �̱��� ������

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

	//������ ��Ŷ ó��
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}
