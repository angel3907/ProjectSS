#pragma once
#include "ImageSquareButton.h"

enum ReadyButtonState
{
	NONACTIVE,
	ACTIVE,
	READY,
	STARTED
};

class ReadyButton : public ImageSquareButton
{
public:
	ReadyButton(Vector2 InPos, int InWidth, int InHeight, uint32_t InImageKey) : ImageSquareButton(InPos, InWidth, InHeight, InImageKey)
	{
		mReadyButtonState = NONACTIVE;
	}

	virtual ~ReadyButton() {}
	virtual void Render() override;
	void ChangeState(ReadyButtonState InReadyButtonState);
	void ProcessClick();

private:
	ReadyButtonState mReadyButtonState;

	float ReadyPacketSendDuration = 1.0f;
	float ReadyPacketSendTime = 0.f;
};