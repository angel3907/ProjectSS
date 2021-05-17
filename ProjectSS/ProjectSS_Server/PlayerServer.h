#pragma once
#include "Player.h"

class PlayerServer : public Player
{
public:
	CLASS_IDENTIFICATION('PLYR', PlayerServer);

	PlayerServer();
	~PlayerServer(){}

	virtual void Update() override;

private:
	void ReplicatePlayer();

	//공격 처리
	void HandleAttacking();

	void UpdateAttacked();
	void SetAttacked(Vector2 InAttackingPlayerPos); //공격받았을 때

	void SetAttackEffect(bool InValue);
	void UpdateAttackedEffectTime();
	float AttackingTime = 0.f; //공격했던시간

	float mTimeOfNextAttack; //다음 공격가능한 시간
	float mTimeBetweenAttack; //공격 쿨타임

	Vector2 AttackedDir = Vector2(0,0);
	const float AttackSpeed = 200.0f;
};