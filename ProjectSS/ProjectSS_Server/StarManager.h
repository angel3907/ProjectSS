#pragma once
#include "StarServer.h"
#include <vector>

/*1. 별 객체 미리 생성해두기 (20개 벡터로)
2. 별의 랜덤한 위치와 크기 정하기
3. 상태 활성화해서 주기적으로 두기 (1~3초 사이로)
4. 숨겨져있지 않은 별이 플레이어와 충돌했는지 확인하기
*/

class StarManager
{
public:
	static StarManager& Get()
	{
		static StarManager sInstance;
		return sInstance;
	}

	~StarManager();

	void Update();
	void CheckStarGeneratingTime();
	void CheckStarCollidedToPlayer();

private:
	StarManager();

	//별 랜덤 위치 / 크기 정하기
	void SetRandomStatus(StarServer* InStar);
	void SetRandomPos(StarServer* InStar);
	void SetRandomSize(StarServer* InStar);

	std::vector<StarServer*> Stars;
	const int MaxStarCnt = 20;
	float mLastTimeStarGenerated = 0.f;

	float StarGeneratingDuration = MinStarGeneratingDuration;

	const float MinStarGeneratingDuration = 0.5f;
	const float MaxStarGeneratingDuration = 2.0f;
};