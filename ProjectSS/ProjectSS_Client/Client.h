#pragma once
#include "Engine.h"

class Client : public Engine
{
public:
	static bool StaticInit();

protected:
	Client();

	//�� ������ ����
	virtual void DoFrame() override;
	//virtual void HandleEvent(SDL_Event* InEvent) override;
};