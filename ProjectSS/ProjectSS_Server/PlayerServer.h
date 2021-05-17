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

	//���� ó��
	void HandleAttacking();

	void UpdateAttacked();
	void SetAttacked(Vector2 InAttackingPlayerPos); //���ݹ޾��� ��

	void SetAttackEffect(bool InValue);
	void UpdateAttackedEffectTime();
	float AttackingTime = 0.f; //�����ߴ��ð�

	float mTimeOfNextAttack; //���� ���ݰ����� �ð�
	float mTimeBetweenAttack; //���� ��Ÿ��

	Vector2 AttackedDir = Vector2(0,0);
	const float AttackSpeed = 200.0f;
};