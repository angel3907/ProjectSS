#include "Resultboard.h"
#include "stdafx.h"

struct ResultboardData
{
	uint32_t StarCount;
	std::string Name;
	PlayerColor Color;

	bool operator<(const ResultboardData& InResultBoardData)
	{
		return this->StarCount > InResultBoardData.StarCount;
	}

	bool operator==(const ResultboardData& InResultBoardData)
	{
		return this->StarCount == InResultBoardData.StarCount;
	}
};

Resultboard::Resultboard()
{
	QuitButton = new SquareButton(Vector2(WINDOW_WIDTH * 0.495f, WINDOW_HEIGHT * 0.916f), 423, 110); 
}

Resultboard::~Resultboard()
{
	delete QuitButton;
}

void Resultboard::Render()
{
	SDLRenderer::Get().DrawTexture(ResultboardTextureKey, Vector2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f));

	std::vector<ResultboardData> ResultboardDataArray;
	for (auto Player_ : LinkingContext::Get().GetPlayerPtrSet())
	{
		ResultboardDataArray.push_back({ Player_->GetStarCount(), Player_->GetName(), Player_->GetPlayerColor()});
	}

	sort(ResultboardDataArray.begin(), ResultboardDataArray.end());

	int PlayerNum = 0;
	int YOffset = 165;
	int BaseY = WINDOW_HEIGHT * 0.3f;

	for (auto ResultboardData_ : ResultboardDataArray)
	{
		SDLRenderer::Get().DrawStretchedTexture(ResultboardData_.Color, Vector2(WINDOW_WIDTH * 0.22f, BaseY + YOffset * PlayerNum + 25), Vector2(130, 130));

		SDLRenderer::Get().DrawFont(ETextSize::LARGE, BlackColor, 
			Vector2(WINDOW_WIDTH * 0.3f, BaseY + YOffset * PlayerNum), ResultboardData_.Name.c_str());

		std::string StarDesc = ("You got " + std::to_string(ResultboardData_.StarCount) + " Star!"); 

		SDLRenderer::Get().DrawFont(ETextSize::LARGE, BlackColor,
			Vector2(WINDOW_WIDTH * 0.55f, BaseY + YOffset * PlayerNum), StarDesc.c_str());

		PlayerNum++;
		if (PlayerNum >= DisplayPlayerScoreNum)
		{
			break;
		}
	}
}