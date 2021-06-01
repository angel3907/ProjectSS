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

	//�������̸� ���ݿ� ���� �̵� ��ġ ����
	if (bAttacked)
	{
		UpdateAttacked();
	}

	//����Ʈ�� on�̸� �ð� ������Ʈ
	if (bAttackEffectOn)
	{
		UpdateAttackedEffectTime();
	}

	if (!Pos.IsEqual(OldPos))
	{
		//��� Ŭ���̾�Ʈ���� �̵������� ����
		ReplicatePlayer();
	}

	HandleAttacking();
}

void PlayerServer::ReplicatePlayer()
{
	ReplicationCommand RA;
	RA.NetworkId = GetNetworkId();
	RA.RA = ReplicationAction::RA_Update;

	//��� Ŭ���̾�Ʈ���� ���ø����̼� ���� ����
	NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RA);
}

void PlayerServer::HandleAttacking()
{
	float time = TimeUtil::Get().GetFrameStartTime();
	
	//������ ������, ���� ���� ���� �ð��� �Ѿ�ٸ�
	if (mIsAttacking && TimeUtil::Get().GetFrameStartTime() > mTimeOfNextAttack)
	{
		//���� ����Ʈ ��
		SetAttackEffect(true);

		//���� ���� ���� �ð� �������ְ�
		mTimeOfNextAttack = time + mTimeBetweenAttack;

		//�ֺ��� �÷��̾ �ִ��� Ȯ��
		for (auto PlayerValue : LinkingContext::Get().GetPlayerPtrSet())
		{
			//�� �÷��̾�� �н�
			if (PlayerValue == this)
			{
				continue;
			}

			//�̹� ���ݹް� �ִ� ����� ���� �� ����
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

		//���� ������Ʈ ��Ŷ ����
		ReplicatePlayer();
	}
}

void PlayerServer::UpdateAttacked()
{
	float CurrentTime = TimeUtil::Get().GetTimef();

	if (CurrentTime > AttackedTime + UpdateAttackedDuration)
	{
		AttackedTime = CurrentTime;
	
		//���ݴ��ϴ� �������� ��� ���Ѵ�
		//�̶� �ִ밪�� �����ϸ� �����
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
	//ƨ�ܳ��� ����
	bAttacked = true;
	AttackedTime = TimeUtil::Get().GetTimef();
	AttackedDir = Vector2::GetNormalized(Pos - InAttackingPlayerPos);

	//Ȥ�� ���� ��ġ�� ������ �׳� ���������� ƨ���
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
		//����Ʈ Ʈ�� �ð��� ������ ���� ����
		ReplicatePlayer();
	}
}
