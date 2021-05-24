#pragma once
class Scene
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void HandleInput(SDL_Event* InEvent){}

	Scene(){};
	virtual ~Scene(){};
};