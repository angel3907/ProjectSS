#pragma once
class Scene
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;

	Scene(){};
	virtual ~Scene(){};
};