#pragma once
struct SDL_Renderer;
struct SDL_Window;

#include "SDL_ttf.h"
#include <unordered_map>
#include "MathUtils.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define  WINDOW_HALF_WIDTH 500
#define  WINDOW_HALF_HEIGHT 500

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
	void DrawStretchedTextureEx(SDL_Texture* InTexture, Vector2 InPos, Vector2 InWidthHeight, float InAngle, SDL_RendererFlip InFlip = SDL_FLIP_NONE);

	void DrawFont(TTF_Font* InFont, SDL_Color InColor, Vector2 InPos, const char* InText);

	//�׽�Ʈ�� �Լ�
	void DrawBackground();


	//�׸� �� ȭ�鿡 ǥ��
	void Present();
	//ȭ�� ����
	void Clear();

	void QuitSDL();

	void CloseFonts();
	void CloseTextures();

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

	std::unordered_map<uint32_t, SDL_Texture*> Textures;

	TTF_Font* MainFont;
};