#pragma once
#include "Scene.h"

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

private:
	InGameScene();
	virtual ~InGameScene();
};