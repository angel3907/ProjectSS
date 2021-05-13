#include "stdafx.h"
#include "Engine.h"
#include <time.h>
#include "SDL.h"
#include "SDLRenderer.h"

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
	SDLRenderer::Get().QuitSDL();
}

int Engine::Run()
{
	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	//프레임 루프
	
	bool Quit = false;
	SDL_Event Event;
	memset(&Event, 0, sizeof(SDL_Event));

	while (!Quit && mShouldKeepRunning)
	{
		//SDL 이벤트 폴링하는 부분
		if (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
			{
				Quit = true;
			}
			else
			{
				HandleEvent(&Event);
			}
		}
		else
		{ 
			//그 외 게임 루프
			TimeUtil::Get().Update();
			DoFrame();
		}
	}

	return Event.type; //SDL 이벤트 타입 반환하는 부분
}

void Engine::HandleEvent(SDL_Event* InEvent)
{
	//창 닫기 외 이벤트 처리
}

void Engine::DoFrame()
{
	//월드 업데이트
}
