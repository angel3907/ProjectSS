#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Player.h"
class SquareButton;
class CircleButton;
class TextInputBox;
class TextBox;

enum ServerState
{
	NO_SERVER,
	FULL_PLAYER,
	GAME_STARTED
};

class EntryScene : public Scene
{
public:
	static EntryScene& Get()
	{
		static EntryScene sInstance;
		return sInstance;
	}

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(SDL_Event* InEvent) override;

	void CheckButtonsPressed(Vector2 InPos);
	void ProcessInput(SDL_Event* InEvent);

	void InitServerStateToStringMap();

	bool IsAllTextBoxFilled();
	void NotifyEmptyTextBox();

	void NotifyNoAdmittance(NoAdmittanceReason InReason);

	void TryEnterServer();
	void SetServerAddress();
	void SetPlayerState();
	void SetTryEnterServer(bool bInTryEnterServer) { bTryEnterServer = bInTryEnterServer; }

	void CheckTryEnterServerTimeLimit();

private:
	EntryScene();
	virtual ~EntryScene();

	bool bTryEnterServer;
	float TryEnterServerTime;
	const float TryEnterServerTimeLimit = 10.0f;

	const int NameLimit = 8;
	const int IPLimit = 15;
	const int PortLimit = 5;

	PlayerColor SelectedPlayerColor = RED;

	Vector2 CheckMarkPos;
	Vector2 CheckMarkOffsetX;
	Vector2 PlayerColorPos;

	Vector2 ColorButtonPos;

	SquareButton* EntryButton;
	CircleButton* ColorButton[6];

	TextInputBox* NameInputBox;

	TextInputBox* IPInputBox;
	TextInputBox* PortInputBox;

	TextBox* EntryStatusTextBox;

	std::unordered_map<ServerState, std::string> ServerStateToStringMap; 
};