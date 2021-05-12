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

	ClientProxyPtr Client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());

	//ó�� �ȵ� �̵� ����Ʈ�� �����ͼ� ó����
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

	HandleAttacking();
	//���⿡ �̵����°� ����Ǿ����� �������ִ°� �ִµ� �ϴ� �н�
}

void PlayerServer::HandleAttacking()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//������ ������, ���� ���� ���� �ð��� �Ѿ�ٸ�
	if (mIsAttacking && TimeUtil::Get().GetFrameStartTime() > mTimeOfNextAttack)
	{
		//���� ���� ���� �ð� �������ְ�
		mTimeOfNextAttack = time + mTimeBetweenAttack;

		//TODO ���� ó��
	}

}
