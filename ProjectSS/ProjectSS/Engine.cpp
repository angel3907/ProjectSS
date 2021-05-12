#include "stdafx.h"
#include "Engine.h"
#include <time.h>

std::unique_ptr<Engine> Engine::sInstance;

Engine::Engine()
	:mShouldKeepRunning(true)
{
	//최초 초기화가 필요한 것들 초기화.
	//소켓 유틸, 객체 레지스트리, 월드, 스코어 보드, SDL 초기화 등..
	SocketUtil::StartUsingSocket();

	srand(static_cast<uint32_t>(time(nullptr)));
}

Engine::~Engine()
{
	SocketUtil::EndUsingSocket();	
}

int Engine::Run()
{
	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	//프레임 루프
	
	bool quit = false;
	
	while (!quit && mShouldKeepRunning)
	{
		//SDL 이벤트 폴링하는 부분
		//그 외 게임 루프
		TimeUtil::Get().Update();
		DoFrame();
	}

	return true; //SDL 이벤트 타입 반환하는 부분
}

void Engine::DoFrame()
{
	//월드 업데이트
}
