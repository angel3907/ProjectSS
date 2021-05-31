#pragma once
#include "Scene.h"
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

private:
	InGameScene();
	virtual ~InGameScene();
	ReadyButton* mReadyButton;
};