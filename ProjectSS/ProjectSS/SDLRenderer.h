#pragma once
struct SDL_Renderer;
struct SDL_Window;
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

	void LoadTextures();
	void DrawTexture(uint32_t InKey, Vector2 InPos);
	void Present();
	void Clear();

	void QuitSDL();

private:
	//SDL�� ���� â�� ��� �� �ְ�, �� â�� ���� �������� ����.
	//â�� ��������� �ش� â�� �ڵ��� ������ �����صΰ�, �ڵ��� ���� �۾��� �ؾ� ��
	//������ ��ü�� �ƹ� ������ ���� �ʰ�, �ܼ��� ������ �������� �ڵ�.
	//â �̸� ��ġ ũ�� �ٲٴµ� �������� �ڵ��� �ʿ�
	SDL_Window* Window;

	//������ - â�� ���� �������� �� �ֵ��� �ϴ� �ڵ�
	SDL_Renderer* Renderer;

	unordered_map<uint32_t, SDL_Texture*> Textures;
};