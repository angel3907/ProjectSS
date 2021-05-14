#include "stdafx.h"
#include "PlayerServer.h"
#include "ClientProxy.h"
#include "NetworkManagerServer.h"

PlayerServer::PlayerServer() :
	mTimeOfNextAttack(0.0f),
	mTimeBetweenAttack(0.2f)
{
}

void PlayerServer::Update()
{
	Player::Update();

	Vector2 OldPos = Pos;

	ClientProxyPtr Client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());

	//처리 안된 이동 리스트를 가져와서 처리함
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

	if (!Pos.IsEqual(OldPos))
	{
		ReplicationCommand RA;
		RA.NetworkId = GetNetworkId();
		RA.RA = ReplicationAction::RA_Update;
		LinkingContext::Get().AddUnprocessedRA(RA);
	}

	HandleAttacking();
	//여기에 이동상태가 변경되었으면 세팅해주는게 있는데 일단 패스
}

void PlayerServer::HandleAttacking()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//공격을 눌렀고, 다음 공격 가능 시간을 넘어갔다면
	if (mIsAttacking && TimeUtil::Get().GetFrameStartTime() > mTimeOfNextAttack)
	{
		//다음 공격 가능 시간 갱신해주고
		mTimeOfNextAttack = time + mTimeBetweenAttack;

		//TODO 공격 처리
	}

}
