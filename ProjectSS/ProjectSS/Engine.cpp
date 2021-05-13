#include "stdafx.h"
#include "Engine.h"
#include <time.h>
#include "SDL.h"
#include "SDLRenderer.h"

std::unique_ptr<Engine> Engine::sInstance;

Engine::Engine()
	:mShouldKeepRunning(true)
{
	//���� �ʱ�ȭ�� �ʿ��� �͵� �ʱ�ȭ.
	//���� ��ƿ, ��ü ������Ʈ��, ����, ���ھ� ����, SDL �ʱ�ȭ ��..
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
	//������ ����
	
	bool Quit = false;
	SDL_Event Event;
	memset(&Event, 0, sizeof(SDL_Event));

	while (!Quit && mShouldKeepRunning)
	{
		//SDL �̺�Ʈ �����ϴ� �κ�
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
			//�� �� ���� ����
			TimeUtil::Get().Update();
			DoFrame();
		}
	}

	return Event.type; //SDL �̺�Ʈ Ÿ�� ��ȯ�ϴ� �κ�
}

void Engine::HandleEvent(SDL_Event* InEvent)
{
	//â �ݱ� �� �̺�Ʈ ó��
}

void Engine::DoFrame()
{
	//���� ������Ʈ
}
