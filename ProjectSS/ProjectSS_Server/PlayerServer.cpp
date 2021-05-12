#include "stdafx.h"
#include "PlayerServer.h"
#include "ClientProxy.h"
#include "NetworkManagerServer.h"

void PlayerServer::Update()
{
	Player::Update();

	ClientProxyPtr Client = nullptr;// TODO 주석살리기 = NetworkManagerServer::Get().GetClientProxy(GetPlayerId());

	if (Client)
	{
		MoveList& MoveListValue = Client->GetUnprocessedMoveList();
		
		for (const Move& UnprocessedMove : MoveListValue)
		{
			const InputState& CurrentState = UnprocessedMove.GetInputState();
			float DeltaTime = UnprocessedMove.GetDeltaTime();

			ProcessInput(DeltaTime, CurrentState);
			SimulateMovement(DeltaTime);
		}

		MoveListValue.Clear();
	}
}

void PlayerServer::ProcessInput(float InDeltaTime, const InputState& InCurrentState)
{
	//TODO : 입력 처리
}

void PlayerServer::SimulateMovement(float InDeltaTime)
{
	//TODO : 이동 시뮬레이션
}
