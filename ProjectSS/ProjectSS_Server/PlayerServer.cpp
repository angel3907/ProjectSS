#include "stdafx.h"
#include "PlayerServer.h"
#include "ClientProxy.h"
#include "NetworkManagerServer.h"

PlayerServer::PlayerServer() :
	mTimeOfNextAttack(0.0f),
	mTimeBetweenAttack(5.0f)
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

	//공격중이면 공격에 의한 이동 위치 업뎃
	if (bAttacked)
	{
		UpdateAttacked();
	}

	//이펙트가 on이면 시간 업데이트
	if (bAttackEffectOn)
	{
		UpdateAttackedEffectTime();
	}

	if (!Pos.IsEqual(OldPos))
	{
		//모든 클라이언트에게 이동정보를 전송
		ReplicatePlayer();
	}

	HandleAttacking();
}

void PlayerServer::ReplicatePlayer()
{
	ReplicationCommand RA;
	RA.NetworkId = GetNetworkId();
	RA.RA = ReplicationAction::RA_Update;

	//모든 클라이언트에게 리플리케이션 정보 전송
	NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RA);
}

void PlayerServer::HandleAttacking()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//공격을 눌렀고, 다음 공격 가능 시간을 넘어갔다면
	if (mIsAttacking && TimeUtil::Get().GetFrameStartTime() > mTimeOfNextAttack)
	{
		//공격 이펙트 온
		SetAttackEffect(true);

		//다음 공격 가능 시간 갱신해주고
		mTimeOfNextAttack = time + mTimeBetweenAttack;

		//주변에 플레이어가 있는지 확인
		for (auto PlayerValue : LinkingContext::Get().GetPlayerPtrSet())
		{
			//내 플레이어면 패스
			if (PlayerValue == this)
			{
				continue;
			}

			//이미 공격받고 있던 사람은 영향 안 받음
			if (PlayerValue->IsAttacked())
			{
				continue;
			}

			if (IsCircleCollided(Pos, PlayerValue->GetPos(), AttackRadius, PlayerValue->GetPlayerRadius()))
			{
				PlayerServer* PlayerServerValue = static_cast<PlayerServer*>(PlayerValue);
				if (PlayerServerValue != nullptr)
				{
					PlayerServerValue->SetAttacked(Pos);
				}
			}
		}

		//정보 업데이트 패킷 보냄
		ReplicatePlayer();
	}
}

void PlayerServer::UpdateAttacked()
{
	float CurrentTime = TimeUtil::Get().GetTimef();

	if (CurrentTime > AttackedTime + UpdateAttackedDuration)
	{
		AttackedTime = CurrentTime;
	
		//공격당하는 방향으로 계속 더한다
		//이때 최대값에 도달하면 멈춘다
		Pos = Pos + (AttackedDir * AttackSpeed);
		
		if (ApplyPosLimit(Pos))
		{
			bAttacked = false;
			AttackedDir = Vector2(0, 0);
		}
	}
}

void PlayerServer::SetAttacked(Vector2 InAttackingPlayerPos)
{
	//튕겨나갈 방향
	bAttacked = true;
	AttackedTime = TimeUtil::Get().GetTimef();
	AttackedDir = Vector2::GetNormalized(Pos - InAttackingPlayerPos);

	//혹시 같은 위치에 있으면 그냥 오른쪽으로 튕기게
	if (AttackedDir.IsEqual(Vector2(0, 0)))
	{
		AttackedDir = Vector2(1, 0);
	}
}

void PlayerServer::SetAttackEffect(bool InValue)
{
	bAttackEffectOn = InValue;

	if (bAttackEffectOn)
	{
		AttackingTime = TimeUtil::Get().GetTimef();
	}
	else
	{
		AttackingTime = 0.f;
	}
}

void PlayerServer::UpdateAttackedEffectTime()
{
	float Time = TimeUtil::Get().GetTimef();

	if (Time > AttackingTime + AttackEffectDuration)
	{
		SetAttackEffect(false);
		//이펙트 트는 시간이 지나면 정보 업뎃
		ReplicatePlayer();
	}
}
