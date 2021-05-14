#pragma once
struct SDL_Renderer;
struct SDL_Window;

#include "SDL_ttf.h"
#include <unordered_map>
#include "MathUtils.h"

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
	void DrawTexture(uint32_t InKey, Vector2 InPos);
	void DrawTexture(SDL_Texture* InTexture, Vector2 InPos);

	void DrawFont(TTF_Font* InFont, SDL_Color InColor, Vector2 InPos, const char* InText);

	//�׽�Ʈ�� �Լ�
	void DrawTest();


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

	SDL_Texture* TestTexture;

	unordered_map<uint32_t, SDL_Texture*> Textures;

	TTF_Font* MainFont;
};