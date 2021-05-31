#include "stdafx.h"
#include "InGameScene.h"
#include "SDLRenderer.h"
#include "ScoreboardManager.h"
#include "NetworkManagerClient.h"
#include "ReadyButton.h"

InGameScene::InGameScene()
{
	mReadyButton = new ReadyButton(Vector2(WINDOW_WIDTH * 0.858f, WINDOW_HEIGHT * 0.91f), 334, 132, 'WAIT');
}

InGameScene::~InGameScene()
{
	delete mReadyButton;
}

void InGameScene::Update()
{
	ScoreboardManager::Get().UpdateScoreboard();
	InputManager::Get().Update();

	//들어오는 패킷 처리
	NetworkManagerClient::sInstance->ProcessInComingPacket();

	//나가는 패킷 처리
	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void InGameScene::Render()
{
	SDLRenderer::Get().DrawBackground();
	ScoreboardManager::Get().RenderScoreborad();

	mReadyButton->Render();

	//모든 오브젝트 드로우
 	for (auto GO : LinkingContext::Get().GetGameObjectSet())
 	{
 		GO->Render();
 	}
}

void InGameScene::HandleInput(SDL_Event* InEvent)
{
	switch (InEvent->type)
	{
	case SDL_KEYDOWN:
		InputManager::Get().HandleInput(EIA_Pressed, InEvent->key.keysym.sym);
		break;
	case SDL_KEYUP:
		InputManager::Get().HandleInput(EIA_Released, InEvent->key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (InEvent->button.button == SDL_BUTTON_LEFT)
		{
			CheckButtonsPressed(Vector2(InEvent->button.x, InEvent->button.y));
		}
		break;
	}
}

void InGameScene::CheckButtonsPressed(Vector2 InPos)
{
	if (mReadyButton->IsPressed(InPos))
	{
		mReadyButton->ProcessClick();
	}
}

void InGameScene::NotifyReadyPacket(ReadyPacketType InReadyPacketType)
{
	switch (InReadyPacketType)
	{
	case READY_ACTIVE:
		mReadyButton->ChangeState(ACTIVE);
		break;
	case READY_NONACTIVE:
		mReadyButton->ChangeState(NONACTIVE);
		break;
	case READY_ACK:
		mReadyButton->ChangeState(READY);
		break;
	case START:
		mReadyButton->ChangeState(STARTED);
		break;
	case END:
		ShowResultUI();
		break;
	case READY_SEND:
		printf("Server Send Ready Packet Type : READY, Please Check the Server Send right Packet\n");
		break;
	default:
		break;
	}
}

void InGameScene::ShowResultUI()
{
	printf("Game is finished!\n");
}
