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

	//�׽�Ʈ�� �Լ�
	void DrawBackground();


	//�׸� �� ȭ�鿡 ǥ��
	void Present();
	//ȭ�� ����
	void Clear();

	void QuitSDL();

	void CloseFonts();
	void CloseTextures();

	int GetTextSize(ETextSize InTextSize) { return TextSizeEnumToSizeValue[InTextSize]; } 

private:
	//SDL�� ���� â�� ��� �� �ְ�, �� â�� ���� �������� ����.
	//â�� ��������� �ش� â�� �ڵ��� ������ �����صΰ�, �ڵ��� ���� �۾��� �ؾ� ��
	//������ ��ü�� �ƹ� ������ ���� �ʰ�, �ܼ��� ������ �������� �ڵ�.
	//â �̸� ��ġ ũ�� �ٲٴµ� �������� �ڵ��� �ʿ�
	SDL_Window* Window;

	//������ - â�� ���� �������� �� �ֵ��� �ϴ� �ڵ�
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