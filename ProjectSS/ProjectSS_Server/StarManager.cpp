#include "StarManager.h"
#include "stdafx.h"
#include "NetworkManagerServer.h"
#include "PlayerServer.h"

StarManager::~StarManager()
{
	for (int i = 0; i < MaxStarCnt; i++)
	{
		delete Stars[i];
	}

	Stars.clear();
}

void StarManager::Update()
{
	//������ ���۵Ǹ� ������Ʈ�� ������ ��
	CheckStarGeneratingTime();
	CheckStarCollidedToPlayer();
}

void StarManager::CheckStarGeneratingTime()
{
	float Time = TimeUtil::Get().GetTimef();

	if (Time > mLastTimeStarGenerated + StarGeneratingDuration)
	{
		for (auto StarValue : Stars)
		{
			//������ ���� ã��
			if (StarValue->GetStarStatus().bHidden == true)
			{
				StarValue->GetStarStatus().bHidden = false;
				SetRandomStatus(StarValue);
				
				//���ø����̼� ��Ű�� (������Ʈ)
				ReplicationCommand RC;
				RC.NetworkId = StarValue->GetNetworkId();
				RC.RA = ReplicationAction::RA_Update;
				NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RC);
				break;
			}
		}
		//������ �н�

		//�� ��ȯ �ֱ� ��������
		StarGeneratingDuration = GetRandomInt(MinStarGeneratingDuration * 10, MaxStarGeneratingDuration * 10) * 0.1f;
		mLastTimeStarGenerated = Time;
	}
}

void StarManager::CheckStarCollidedToPlayer()
{
	for (auto StarValue : Stars)
	{
		//���̴� �� ��
		if (StarValue->GetStarStatus().bHidden == false)
		{
			for (auto PlayerPtr_ :  LinkingContext::Get().GetPlayerPtrSet())
			{
				//��� �÷��̾ ���鼭 �浹�� �� ������
				if (IsCircleCollided(StarValue->GetPos(), PlayerPtr_->GetPos(), StarValue->GetStarStatus().StarRadius, PlayerPtr_->GetPlayerRadius()))
				{
					//�Ⱥ��̰� �ϰ�
					StarValue->GetStarStatus().bHidden = true;
					
					PlayerServer* PlayerServer_ = static_cast<PlayerServer*>(PlayerPtr_);
					if (PlayerServer_ != nullptr)
					{
						//�׸�ŭ �� ���� �ø���
						PlayerServer_->AddStarCount(StarValue->GetStarStatus().Value);
						PlayerServer_->ReplicatePlayer(); //�÷��̾� ���ø�����Ʈ

						//���ø����̼� ��Ű�� (�� ������Ʈ)
						ReplicationCommand RC;
						RC.NetworkId = StarValue->GetNetworkId();
						RC.RA = ReplicationAction::RA_Update;
						NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RC);
					}
				}
			}
		}
	}
}

StarManager::StarManager()
{
	for (int i = 0; i < MaxStarCnt; i++)
	{
		StarServer* S = new StarServer(StarStatus());
		Stars.push_back(S);

		int NetworkId = LinkingContext::Get().GetNetworkId(S, true);

		//���ø����̼� ��Ű�� (����)
		ReplicationCommand RC;
		RC.NetworkId = NetworkId;
		RC.RA = ReplicationAction::RA_Create;
		NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RC);
	}
}

void StarManager::SetRandomStatus(StarServer* InStar)
{
	SetRandomPos(InStar);
	SetRandomSize(InStar);
}

void StarManager::SetRandomPos(StarServer* InStar)
{
	//StarRadius ~ WORLD_WIDTH-StarRadius ������ ���� �����ؾ� ��
	uint8_t StarRadius = InStar->GetStarStatus().StarRadius;
	int PosX = GetRandomInt(StarRadius, WORLD_WIDTH - StarRadius);
	int PosY = GetRandomInt(StarRadius, WORLD_HEIGHT - StarRadius);

	InStar->SetPos(Vector2(PosX, PosY));
}

void StarManager::SetRandomSize(StarServer* InStar)
{
	//80% Ȯ���� �׳� ��, 20% Ȯ���� ū ���� ������ ��
	//0~9���� ���� ��������
	int Res = GetRandomInt(0, 10);
	if (Res <= 7)
	{
		InStar->GetStarStatus().SetValue(1);
	}
	else
	{
		InStar->GetStarStatus().SetValue(3);
	}
}
