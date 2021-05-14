#pragma once
#include <memory>
#include "SDL.h"

class Engine
{

public:
	virtual ~Engine();
	static std::unique_ptr<Engine> sInstance;

	//���� �ڵ�
	virtual int Run();
	void SetShouldKeepRunning(bool InShouldKeepRunning) {
		mShouldKeepRunning = InShouldKeepRunning;}
	//virtual void HandleEvent(SDL_Event* InEvent);

protected:
	//���ӿ� �ʿ��� �������� �͵� �ʱ�ȭ
	Engine();

	//�����Ӹ��� ���� ���� ����
	virtual void DoFrame();

private:
	int DoRunLoop();
	virtual void HandleEvent(SDL_Event* InEvent);

	bool mShouldKeepRunning;
};