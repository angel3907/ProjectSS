#pragma once
#include "ClientProxy.h"
#include "Engine.h"

class Server : public Engine
{
public:
	//초기화
	static bool StaticInit();

	//게임 루프 수행
	virtual void DoFrame() override;
	virtual int Run();

	//새 클라이언트가 왔을 때 처리 / 새 클라이언트가 사라졌을 때 처리
	void HandleNewClient(ClientProxyPtr InClientProxy);
	void HandleLostClient(ClientProxyPtr InClientProxy);

	//특정 아이디 플레이어 가져오기 / 플레이어 새로 스폰
	Player* GetPlayerWithPlayerId(int InPlayerId);
	void SpawnPlayer(int InPlayerId);

private:
	Server();

	//네트워크 매니저 초기화
	bool InitNetworkManager();

	//월드 초기화
	void SetupWorld();

	//게임오브젝트 업데이트
	void UpdateGameObjects();
};