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
	//���� ó��
	void HandleAttacking();

	float mTimeOfNextAttack; //���� ���ݰ����� �ð�
	float mTimeBetweenAttack; //���� ��Ÿ��
};