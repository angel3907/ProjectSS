#pragma once
#include "ClientProxy.h"
#include "Engine.h"

class Server : public Engine
{
public:
	//�ʱ�ȭ
	static bool StaticInit();

	//���� ���� ����
	virtual void DoFrame() override;
	virtual int Run();

	//�� Ŭ���̾�Ʈ�� ���� �� ó�� / �� Ŭ���̾�Ʈ�� ������� �� ó��
	void HandleNewClient(ClientProxyPtr InClientProxy);
	void HandleLostClient(ClientProxyPtr InClientProxy);

	//Ư�� ���̵� �÷��̾� �������� / �÷��̾� ���� ����
	Player* GetPlayerWithPlayerId(int InPlayerId);
	void SpawnPlayer(int InPlayerId);

private:
	Server();

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	bool InitNetworkManager();

	//���� �ʱ�ȭ
	void SetupWorld();

	//���ӿ�����Ʈ ������Ʈ
	void UpdateGameObjects();
};