#include "StarManager.h"
#include "stdafx.h"
#include "NetworkManagerServer.h"

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
	//게임이 시작되면 업데이트를 시작할 것
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
			//숨겨진 별을 찾음
			if (StarValue->GetStarStatus().bHidden == true)
			{
				StarValue->GetStarStatus().bHidden = false;
				SetRandomStatus(StarValue);
				
				//리플리케이션 시키기 (업데이트)
				ReplicationCommand RC;
				RC.NetworkId = StarValue->GetNetworkId();
				RC.RA = ReplicationAction::RA_Update;
				NetworkManagerServer::sInstance->AddUnprocessedRAToAllClients(RC);
				break;
			}
		}
		//없으면 패스

		//별 소환 주기 랜덤돌림
		StarGeneratingDuration = GetRandomInt(MinStarGeneratingDuration * 10, MaxStarGeneratingDuration * 10) * 0.1f;
		mLastTimeStarGenerated = Time;
	}
}

void StarManager::CheckStarCollidedToPlayer()
{
}

StarManager::StarManager()
{
	for (int i = 0; i < MaxStarCnt; i++)
	{
		StarServer* S = new StarServer(StarStatus());
		Stars.push_back(S);

		int NetworkId = LinkingContext::Get().GetNetworkId(S, true);

		//리플리케이션 시키기 (생성)
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
	//StarRadius ~ WORLD_WIDTH-StarRadius 사이의 값을 리턴해야 함
	uint8_t StarRadius = InStar->GetStarStatus().StarRadius;
	int PosX = GetRandomInt(StarRadius, WORLD_WIDTH - StarRadius);
	int PosY = GetRandomInt(StarRadius, WORLD_HEIGHT - StarRadius);

	InStar->SetPos(Vector2(PosX, PosY));
}

void StarManager::SetRandomSize(StarServer* InStar)
{
	//80% 확률로 그냥 별, 20% 확률로 큰 별이 나오게 함
	//0~9까지 값이 나오도록
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
