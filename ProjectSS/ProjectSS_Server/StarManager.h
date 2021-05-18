#pragma once
#include "StarServer.h"
#include <vector>

/*1. �� ��ü �̸� �����صα� (20�� ���ͷ�)
2. ���� ������ ��ġ�� ũ�� ���ϱ�
3. ���� Ȱ��ȭ�ؼ� �ֱ������� �α� (1~3�� ���̷�)
4. ���������� ���� ���� �÷��̾�� �浹�ߴ��� Ȯ���ϱ�
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

	//�� ���� ��ġ / ũ�� ���ϱ�
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