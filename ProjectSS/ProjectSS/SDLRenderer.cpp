#include "stdafx.h"
#include "SDLRenderer.h"
#include "SDL.h"
#include "SDL_image.h"

void SDLRenderer::InitSDL()
{
	//SDL �ʱ�ȭ - ���� ���
	SDL_Init(SDL_INIT_VIDEO);

	//�̹��� �ʱ�ȭ
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	
	//������ ����
	//����
	//1. ������ â ���� (�ƽ�Ű ���� ���ڿ��� �ƴ϶�� UTF8 ���ڵ� �Ǿ��־�� ��)
	//2,3 â�� �ʱ���ġ
		// ����� ���� ��� �������� �ش� �ȼ���ŭ ������ ��ġ�� â�� ����
		//SDL_WINDOWPOS_UNDEFINED : ������ ��ġ�� �ڵ����� �����
	//4,5 â�� ũ��
		//���� ǥ����, 3��� �׵θ� ũ�⸦ ������ ���� Ŭ���̾�Ʈ ���� ũ��.
	//6. â�� �Ӽ�
		//�׵θ� ���� â�� ����ų�, ũ�⸦ ������ �� �ֵ��� �ϰų� ����� �Ӽ��� �� �� ����.
	Window = SDL_CreateWindow(
		"Hello SDL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //ȭ�� ���߾ӿ� ǥ��
		640, 480, 0
	);

	//������ ����
	//����
	//1. �������� ������ ������
	//2. �������� � ����̹��� ������� ����
		//PCȯ�濡���� DirectX / OpenGL / Software ���� �ϳ��� �����ؼ� ����� �� ����
		//-1�� ������ ���� ù��°�� ��� ������ ����̹��� �����
	//3. ������ �Ӽ�
	//SDL_RENDERER_ACCELERATED : �ϵ���� ����
	//SDL_RENDERER_PRESENTVSYNC : ���� ����ȭ ���
	//����� �ʿ� ������ 0
	Renderer = SDL_CreateRenderer(
		Window,
		-1,
		0
	);
}

SDL_Texture* SDLRenderer::LoadTexture(const char* File)
{
	SDL_Surface *Surface;
	SDL_Texture *Texture;

	//��Ʈ�� ���� �ε�
	//Surface = SDL_LoadBMP(File);
	Surface = IMG_Load(File); //bmp �̿��� �ٸ� �̹��� �ε�
	if (Surface == nullptr)
	{
		printf("%s ������ ���� �� �����ϴ�\n", File);
		printf("���� : %s\n", SDL_GetError());
	}

	//�����̽��κ��� �ؽ��ĸ� ����
	//���� 1)������ 2)�����̽�
	Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	if (Texture == nullptr)
	{
		printf("�ؽ��ĸ� ������ �� �����ϴ�.\n");
	}

	//�����̽��� �Ҵ��� �޸� ����
	SDL_FreeSurface(Surface);

	return Texture;
}

void SDLRenderer::LoadTextures()
{
	SDL_Texture* Texture;
	Texture = LoadTexture("../Resources/ppp.png");

	TestTexture = Texture;
	//Textures['TEST'] = Texture;
}

void SDLRenderer::DrawTexture(uint32_t InKey, Vector2 InPos)
{
	SDL_Texture* Texture = Textures[InKey];

	if (Texture == nullptr)
	{
		printf("Cannot Find Texture With Key %d\n", InKey);
		return;
	}
	
	//Src : �̹��� �� � �κ��� ��¿� ������
	//Dst : ȭ���� ��� ������ �̹����� �������

	//Rect�� x,y : �̹����� ���۵� ��ġ
	//		 w,h : ��, ���� 
	//�̹����� Ư�� ������ ����ϰų�, Ư�� �����ŭ �ø��ų� �ٿ��� ����� �� ����.
	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	//�̹��� ��ü�� ����� ���̹Ƿ�
	//src = (0,0,imageWidth, imageHeight)
	//dst = (x,y,imageWidth, imageHeight)
	
	//�ؽ����� ������ ������ �Լ�
	//����
	//1. ������ ���� �ؽ���, 2. �ؽ��� �ȼ� ����, 3.�ؽ�ó ���� ����, 4&5. �ؽ��� width, height
	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	Dst.x = InPos.PosX;
	Dst.y = InPos.PosY;
	Dst.w = Src.w;
	Dst.h = Src.h;

	//�̹����� Src �κ��� ȭ���� Dst�� ������ִ� �Լ�
	//����
	//1. ������ 2.�ؽ��� 3,4 src / dst ����
	SDL_RenderCopy(Renderer, Texture, &Src, &Dst);
}

void SDLRenderer::DrawTexture(SDL_Texture* InTexture, Vector2 InPos)
{
	SDL_Texture* Texture = InTexture;

	if (Texture == nullptr)
	{
		printf("Texture Is Null at SDLRenderer::DrawTexture\n");
		return;
	}

	//Src : �̹��� �� � �κ��� ��¿� ������
	//Dst : ȭ���� ��� ������ �̹����� �������

	//Rect�� x,y : �̹����� ���۵� ��ġ
	//		 w,h : ��, ���� 
	//�̹����� Ư�� ������ ����ϰų�, Ư�� �����ŭ �ø��ų� �ٿ��� ����� �� ����.
	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	//�̹��� ��ü�� ����� ���̹Ƿ�
	//src = (0,0,imageWidth, imageHeight)
	//dst = (x,y,imageWidth, imageHeight)

	//�ؽ����� ������ ������ �Լ�
	//����
	//1. ������ ���� �ؽ���, 2. �ؽ��� �ȼ� ����, 3.�ؽ�ó ���� ����, 4&5. �ؽ��� width, height
	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	Dst.x = InPos.PosX;
	Dst.y = InPos.PosY;
	Dst.w = Src.w;
	Dst.h = Src.h;

	//�̹����� Src �κ��� ȭ���� Dst�� ������ִ� �Լ�
	//����
	//1. ������ 2.�ؽ��� 3,4 src / dst ����
	SDL_RenderCopy(Renderer, Texture, &Src, &Dst);
}

void SDLRenderer::DrawTest()
{
	DrawTexture(TestTexture, Vector2(0,0));
}

void SDLRenderer::Present()
{
	SDL_RenderPresent(Renderer);
}

void SDLRenderer::Clear()
{
	SDL_RenderClear(Renderer);
}

void SDLRenderer::QuitSDL()
{
	SDL_DestroyTexture(TestTexture);

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);

	//SDL ����
	SDL_Quit();
	IMG_Quit();
}
