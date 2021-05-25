#pragma once
struct SDL_Renderer;
struct SDL_Window;

#include "SDL_ttf.h"
#include <unordered_map>
#include "MathUtils.h"

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 1000

enum ETextSize
{
	SMALL = 'SMAL',
	MID = 'MIDD',
	LARGE = 'LARG',
	EXTREME = 'EXTR'
};

class SDLRenderer
{
public:
	static SDLRenderer& Get()
	{
		static SDLRenderer sInstance;
		return sInstance;
	}

	void InitSDL();

	SDL_Texture* LoadTexture(const char* File);
	TTF_Font* LoadFont(const char* File, int FontSize);

	void LoadFonts();
	void LoadTextures();

	void SetTextureColorMod(SDL_Texture* InTexture, SDL_Color InColor);
	void SetTextureBlendMod(SDL_Texture* InTexture, SDL_BlendMode InBlendMode);

	void DrawTexture(uint32_t InKey, Vector2 InPos);
	void DrawTexture(SDL_Texture* InTexture, Vector2 InPos);
	void DrawStretchedTexture(SDL_Texture* InTexture, Vector2 InPos, Vector2 InWidthHeight);
	void DrawStretchedTexture(uint32_t InKey, Vector2 InPos, Vector2 InWidthHeight);
	void DrawStretchedTextureEx(SDL_Texture* InTexture, Vector2 InPos, Vector2 InWidthHeight, float InAngle, SDL_RendererFlip InFlip = SDL_FLIP_NONE);

	void DrawFont(TTF_Font* InFont, SDL_Color InColor, Vector2 InPos, const char* InText);
	void DrawFont(uint32_t InKey, SDL_Color InColor, Vector2 InPos, const char* InText);

	void DrawSqure(Vector2 InPos, int InWidth, int inHeight);
	void DrawCircle(Vector2 InPos, float InRadius);

	void DrawLine(Vector2 InFirstPos, Vector2 InSecondPos);

	//테스트용 함수
	void DrawBackground();


	//그린 걸 화면에 표시
	void Present();
	//화면 비우기
	void Clear();

	void QuitSDL();

	void CloseFonts();
	void CloseTextures();

	int GetTextSize(ETextSize InTextSize) { return TextSizeEnumToSizeValue[InTextSize]; } 

private:
	//SDL은 여러 창을 띄울 수 있고, 각 창에 대한 렌더링을 지원.
	//창을 만들었으면 해당 창의 핸들을 변수에 저장해두고, 핸들을 갖고 작업을 해야 함
	//윈도우 자체는 아무 정보를 담지 않고, 단순히 생성된 윈도우의 핸들.
	//창 이름 위치 크기 바꾸는데 윈도우의 핸들이 필요
	SDL_Window* Window;

	//렌더러 - 창에 대해 렌더링할 수 있도록 하는 핸들
	SDL_Renderer* Renderer;

	SDL_Texture* PlayerTexture;
	SDL_Texture* PlayerAttackEffectTexture;
	SDL_Texture* BackgroundTexture;

	SDL_Texture* Star_S_Texture;
	SDL_Texture* Star_L_Texture;

	SDL_Texture* ScoreboardTexture;

	SDL_Texture* EntrySceneTexture;
	SDL_Texture* PlayerColorTextures[6];
	const uint32_t PlayerColorNum = 6;
	
	SDL_Texture* CheckMarkTexture;


	std::unordered_map<uint32_t, SDL_Texture*> Textures;
	std::unordered_map<uint32_t, TTF_Font*> Fonts;

	TTF_Font* ExtremeFont;
	TTF_Font* LargeFont;
	TTF_Font* MidFont;
	TTF_Font* SmallFont;

	std::unordered_map<uint32_t, int> TextSizeEnumToSizeValue;
};