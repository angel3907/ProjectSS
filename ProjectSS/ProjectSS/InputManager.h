#pragma once
#include "MoveList.h"
#include "InputAction.h"

class InputManager
{
public:
	static InputManager& Get()
	{
		static InputManager sInstance;
		return sInstance;
	}

	//�Է� ó��
	void HandleInput (EInputAction InInputAction, int InKeyCode);

	const InputState& GetState() const {return mCurrentState;}
	MoveList& GetMoveList() { return mMoveList; }

	//������� �̵����� ������ �� �ʱ�ȭ
	const Move* GetAndClearPendingMove() {auto toRet = mPendingMove; mPendingMove = nullptr; return toRet;}

	void Update();

private:
	InputManager();
	virtual ~InputManager(){}

	//���� �Է� ����
	InputState mCurrentState;

	bool IsTimeToSampleInput(); //���� ��ǲ�� ���ø��� �ð�����
	const Move& SampleInputAsMove(); //��ǲ�� ����

	MoveList mMoveList;
	const Move* mPendingMove; //������� Move��
	float mNextTimeToSampleInput; //���� ��ǲ�� ���ø��� �ð�

	float mTimeBetweenInputSamples = 0.03f; //���ø� �ֱ�
};