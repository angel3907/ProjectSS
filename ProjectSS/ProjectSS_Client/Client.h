#pragma once
#include "Engine.h"
#include "Scene.h"
#include "InGameScene.h"
#include "EntryScene.h"
#include "NetworkManager.h"

class Client : public Engine
{
public:
	static bool StaticInit();
	void EnterInGameScene() { CurrentScene = &InGameScene::Get(); }
	void NotifyNoAdmittanceToEntryScene(NoAdmittanceReason InReason);
	
protected:
	Client();

	//매 프레임 진행
	virtual void DoFrame() override;
	void Render();
	virtual void HandleEvent(SDL_Event* InEvent) override;

private:
	void InitScene();
	Scene* CurrentScene;
};