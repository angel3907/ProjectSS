#pragma once
#include <memory>

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
	bool mShouldKeepRunning;
};