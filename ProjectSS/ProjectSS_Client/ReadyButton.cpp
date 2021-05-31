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
		//������ �޽����� ����
		


	}

	/*
	��Ȱ��ȭ ���ĵ� Ŭ���ߴٸ� �ƹ��͵� �ƴ�
	Ȱ��ȭ ���ĵ� Ŭ�������� ������ �޽����� ����
	�غ� ���ĵ� �ٽ� �����ٸ� �ϴ��� ó�� �� ��
	���� ���ĵ� �ѷ��ٸ� �ƹ��͵� �ƴ�
	*/
}
