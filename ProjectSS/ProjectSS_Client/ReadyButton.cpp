#include "ReadyButton.h"

void ReadyButton::Render()
{
	ImageSquareButton::Render();
}

void ReadyButton::ChangeState(ReadyButtonState InReadyButtonState)
{
	mReadyButtonState = InReadyButtonState;
	switch (mReadyButtonState)
	{
	case NONACTIVE:
		SetImageKey('RDNA');
	case ACTIVE:
		SetImageKey('RDAC');
		break;
	case READY:
		SetImageKey('WAIT');
		break;
	case STARTED:
		SetImageKey('STRT');
		break;
	default:
		break;
	}
}

void ReadyButton::ProcessClick()
{
	if (mReadyButtonState == ACTIVE)
	{
		//서버로 메시지를 보냄
		


	}

	/*
	비활성화 상탠데 클릭했다면 아무것도 아님
	활성화 상탠데 클릭했으면 서버로 메시지를 보냄
	준비 상탠데 다시 눌렀다면 일단은 처리 안 함
	시작 상탠데 둘렀다면 아무것도 아님
	*/
}
