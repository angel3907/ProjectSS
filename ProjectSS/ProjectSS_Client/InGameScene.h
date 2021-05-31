#pragma once
#include "Scene.h"
class Resultboard;
class ReadyButton;

class InGameScene : public Scene
{
public:
	static InGameScene& Get()
	{
		static InGameScene sInstance;
		return sInstance;
	}

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(SDL_Event* InEvent) override;
	void CheckButtonsPressed(Vector2 InPos);
	void NotifyReadyPacket(ReadyPacketType InReadyPacketType);
	void ShowResultUI();

private:
	InGameScene();
	virtual ~InGameScene();
	ReadyButton* mReadyButton;
	Resultboard* mResultboard;

	bool bShowResult = false;
};