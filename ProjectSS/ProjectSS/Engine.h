#pragma once
#include <memory>

class Engine
{

public:
	virtual ~Engine();
	static std::unique_ptr<Engine> sInstance;

	//실행 코드
	virtual int Run();
	void SetShouldKeepRunning(bool InShouldKeepRunning) {
		mShouldKeepRunning = InShouldKeepRunning;}
	//virtual void HandleEvent(SDL_Event* InEvent);

protected:
	//게임에 필요한 공통적인 것들 초기화
	Engine();

	//프레임마다 게임 루프 실행
	virtual void DoFrame();

private:
	int DoRunLoop();
	bool mShouldKeepRunning;
};