#pragma once
#include "Engine.h"

class Client : public Engine
{
public:
	static bool StaticInit();

protected:
	Client();

	//매 프레임 진행
	virtual void DoFrame() override;
	void Render();
	virtual void HandleEvent(SDL_Event* InEvent) override;
};