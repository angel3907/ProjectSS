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

	//입력 처리
	void HandleInput (EInputAction InInputAction, int InKeyCode);

	const InputState& GetState() const {return mCurrentState;}
	MoveList& GetMoveList() { return mMoveList; }

	//대기중인 이동값을 가져온 후 초기화
	const Move* GetAndClearPendingMove() {auto toRet = mPendingMove; mPendingMove = nullptr; return toRet;}

	void Update();

private:
	InputManager();
	virtual ~InputManager(){}

	//현재 입력 상태
	InputState mCurrentState;

	bool IsTimeToSampleInput(); //다음 인풋을 샘플링할 시간인지
	const Move& SampleInputAsMove(); //인풋을 샘플

	MoveList mMoveList;
	const Move* mPendingMove; //대기중인 Move값
	float mNextTimeToSampleInput; //다음 인풋을 샘플링할 시간

	float mTimeBetweenInputSamples = 0.03f; //샘플링 주기
};