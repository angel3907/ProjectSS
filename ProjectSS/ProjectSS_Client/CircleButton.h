#pragma once
#include "Button.h"

class CircleButton : public Button
{
public:
	CircleButton(Vector2 InPos, float InRadius):Button(InPos), Radius(InRadius){}
	~CircleButton() {}

	bool IsPressed(Vector2 InPos) override;
	void Render() override;

private:
	float Radius;
};