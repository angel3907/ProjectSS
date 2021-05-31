#pragma once
#include "SDLRenderer.h"
#include "SquareButton.h"
class ImageSquareButton : public SquareButton
{
public:
	ImageSquareButton(Vector2 InPos, int InWidth, int InHeight, uint32_t InImageKey) : SquareButton(InPos, InWidth, InHeight)
	{
		ImageKey = InImageKey;
	}
	
	virtual ~ImageSquareButton(){}

	void SetImageKey(uint32_t InImageKey){ImageKey = InImageKey;}
	virtual void Render() override;

private:
	uint32_t ImageKey;
};