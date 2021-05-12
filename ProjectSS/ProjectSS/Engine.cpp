#include "stdafx.h"
#include "Engine.h"
#include <time.h>

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
}

int Engine::Run()
{
	return DoRunLoop();
}

int Engine::DoRunLoop()
{
	//������ ����
	
	bool quit = false;
	
	while (!quit && mShouldKeepRunning)
	{
		//SDL �̺�Ʈ �����ϴ� �κ�
		//�� �� ���� ����
		TimeUtil::Get().Update();
		DoFrame();
	}

	return true; //SDL �̺�Ʈ Ÿ�� ��ȯ�ϴ� �κ�
}

void Engine::DoFrame()
{
	//���� ������Ʈ
}
