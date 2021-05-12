#pragma once
#include "Player.h"

class PlayerServer : public Player
{
public:
	CLASS_IDENTIFICATION('PLSV', PlayerServer);

	PlayerServer();
	~PlayerServer(){}

	virtual void Update() override;

private:
	//공격 처리
	void HandleAttacking();

	float mTimeOfNextAttack; //다음 공격가능한 시간
	float mTimeBetweenAttack; //공격 쿨타임
};