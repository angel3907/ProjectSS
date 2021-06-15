#pragma once
#include "ClientProxy.h"
#include "Engine.h"
#include "NetworkManager.h"

class Server : public Engine
{
public:
	//�ʱ�ȭ
	static bool StaticInit();

	//���� ���� ����
	virtual void DoFrame() override;
	virtual int Run();
	void DoGameStartLoop();

	//�� Ŭ���̾�Ʈ�� ���� �� ó�� / �� Ŭ���̾�Ʈ�� ������� �� ó��
	void HandleNewClient(ClientProxyPtr InClientProxy);
	void HandleLostClient(ClientProxyPtr InClientProxy);

	//Ư�� ���̵� �÷��̾� �������� / �÷��̾� ���� ����
	Player* GetPlayerWithPlayerId(int InPlayerId);
	void SpawnPlayer(ClientProxyPtr InClientProxy);

	void CheckReady();
	void CheckGamePlayTime();

	//����Ұ� ���� ��������
	bool IsNoAdmittance() {return IsPlayerFull() || IsGameStarted();}

	NoAdmittanceReason GetNoAdmittanceReason()
	{
		if (IsGameStarted())
		{
			return NoAdmittanceReason::GAME_STARTED;
		}
		else if (IsPlayerFull())
		{
			return NoAdmittanceReason::FULL_PLAYER;
		}
		else
		{
			return NoAdmittanceReason::NONE;
		}
	}

	//���� ���� ���� �˾ƿ���
	int IsGameStarted() { return bGameStarted; }

private:
	Server();

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	bool InitNetworkManager();

	//���� �ʱ�ȭ
	void SetupWorld();

	//���ӿ�����Ʈ ������Ʈ
	void UpdateGameObjects();

	//���� ���� �ʱ�ȭ
	void InitGameState();

	bool IsPlayerFull() { return PlayerNum == MaxPlayerNum; }

	//���� ���� ����
	bool bGameStarted;

	//������ ���� ����
	bool bGameEnded = false;

	//���� ���� �ð�
	float GameStartTime = 0.f;

	//���� ���ǿ� �ɸ��� �ð�
	float GamePlayDuration = 5.f; //TODO ������ ����

	//���� ������ ��
	int PlayerNum;

	const int MaxPlayerNum = 8;
};