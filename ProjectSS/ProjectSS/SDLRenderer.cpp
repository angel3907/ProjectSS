#include "stdafx.h"
#include "SDLRenderer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

void SDLRenderer::InitSDL()
{
	//SDL �ʱ�ȭ - ���� ���
	SDL_Init(SDL_INIT_VIDEO);

	//�̹��� �ʱ�ȭ
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	//�ؽ�Ʈ �ʱ�ȭ
	TTF_Init();
	
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
		WINDOW_WIDTH, WINDOW_HEIGHT, 0
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

void SDLRenderer::StartTextInput()
{
	SDL_StartTextInput();
}

void SDLRenderer::EndTextInput()
{
	SDL_StopTextInput();
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
		printf("%s ����� ������ ���� �� �����ϴ�.\n", File);
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

TTF_Font* SDLRenderer::LoadFont(const char* File, int FontSize)
{
	TTF_Font* Font = TTF_OpenFont(File, FontSize);

	if (Font == nullptr)
	{
		printf("��Ʈ�� �ҷ����� �� �����߽��ϴ�\n");
		return nullptr;
	}

	return Font;
}

void SDLRenderer::LoadFonts()
{
	TextSizeEnumToSizeValue[ETextSize::SMALL] = 30;
	TextSizeEnumToSizeValue[ETextSize::MID] = 40;
	TextSizeEnumToSizeValue[ETextSize::LARGE] = 50;
	TextSizeEnumToSizeValue[ETextSize::EXTREME] = 60;

	SmallFont = LoadFont("../Resources/Fonts/D2Coding.ttf", TextSizeEnumToSizeValue[SMALL]);
	MidFont = LoadFont("../Resources/Fonts/D2Coding.ttf", TextSizeEnumToSizeValue[MID]);
	LargeFont = LoadFont("../Resources/Fonts/D2Coding.ttf", TextSizeEnumToSizeValue[LARGE]);
	ExtremeFont = LoadFont("../Resources/Fonts/D2Coding.ttf", TextSizeEnumToSizeValue[EXTREME]);
	
	Fonts[SMALL] = SmallFont;
	Fonts[MID] = MidFont;
	Fonts[LARGE] = LargeFont;
	Fonts[EXTREME] = ExtremeFont;
}

void SDLRenderer::LoadTextures()
{
	PlayerTexture = LoadTexture("../Resources/Images/Player.png");
	BackgroundTexture = LoadTexture("../Resources/Images/Background.png");
	PlayerAttackEffectTexture = LoadTexture("../Resources/Images/Effect.png");

	Star_S_Texture = LoadTexture("../Resources/Images/Star_S.png");
	Star_L_Texture = LoadTexture("../Resources/Images/Star_L.png");

	ScoreboardTexture = LoadTexture("../Resources/Images/Scoreboard.png");

	EntrySceneTexture = LoadTexture("../Resources/Images/EntryScene.png");
	CheckMarkTexture = LoadTexture("../Resources/Images/check.png");

	for (int i = 0; i < PlayerColorNum; i++)
	{
		std::string TexturePath = "../Resources/Images/PlayerColor" + std::to_string(i) + ".png";
		PlayerColorTextures[i] = LoadTexture(TexturePath.c_str());
		Textures[i] = PlayerColorTextures[i]; //�� Ű ���� 0~PlayerColorNum
	}

	//�ؽ��� �ʿ� ���
	Textures['PLYR'] = PlayerTexture;
	Textures['PLAE'] = PlayerAttackEffectTexture;

	Textures['STAS'] = Star_S_Texture;
	Textures['STAL'] = Star_L_Texture;

	Textures['SCOR'] = ScoreboardTexture;

	Textures['ENTR'] = EntrySceneTexture;
	Textures['CHEK'] = CheckMarkTexture;
}

void SDLRenderer::SetTextureColorMod(SDL_Texture* InTexture, SDL_Color InColor)
{
	SDL_SetTextureColorMod(InTexture, InColor.r, InColor.g, InColor.b);
}

void SDLRenderer::SetTextureBlendMod(SDL_Texture* InTexture, SDL_BlendMode InBlendMode)
{
	SDL_SetTextureBlendMode(InTexture, InBlendMode);
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

	//���ڷ� �޾ƿ� ��ġ�� �׸��� �߽��� �ǵ���..
	Dst.x = InPos.PosX - Src.w * 0.5f;
	Dst.y = InPos.PosY - Src.h * 0.5f;
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

void SDLRenderer::DrawStretchedTexture(SDL_Texture* InTexture, Vector2 InPos, Vector2 InWidthHeight)
{
	SDL_Texture* Texture = InTexture;

	if (Texture == nullptr)
	{
		printf("Texture Is Null at SDLRenderer::DrawStretchedTexture\n");
		return;
	}

	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	Dst.x = InPos.PosX;
	Dst.y = InPos.PosY;
	Dst.w = InWidthHeight.PosX;
	Dst.h = InWidthHeight.PosY;

	SDL_RenderCopy(Renderer, Texture, &Src, &Dst);
}

void SDLRenderer::DrawStretchedTextureEx(SDL_Texture* InTexture, Vector2 InPos, Vector2 InWidthHeight, float InAngle, SDL_RendererFlip InFlip /*= SDL_FLIP_NONE*/)
{
	SDL_Texture* Texture = InTexture;

	if (Texture == nullptr)
	{
		printf("Texture Is Null at SDLRenderer::DrawStretchedTextureEx\n");
		return;
	}

	SDL_Point Center;
	Center.x = InWidthHeight.PosX * 0.5f;
	Center.y = InWidthHeight.PosX * 0.5f;
	
	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	Dst.x = InPos.PosX;
	Dst.y = InPos.PosY;
	Dst.w = InWidthHeight.PosX;
	Dst.h = InWidthHeight.PosY;

	//Angle : ȸ����
	//Flip : ������ �ɼ�
	//	- SDL_FLIP_NONE	      : �� ������
	//	- SDL_FLIP_HORIZONTAL : ���� ������
	//	- SDL_FLIP_VERTICAL   : ���� ������
	//Center : ȸ�� / ������ �߽�
	SDL_RenderCopyEx(Renderer, Texture, &Src, &Dst, InAngle, &Center, InFlip);
}

void SDLRenderer::DrawFont(TTF_Font* InFont, SDL_Color InColor, Vector2 InPos, const char* InText)
{
	SDL_Surface* Surface = TTF_RenderText_Solid(InFont, InText, InColor);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	
	if (Surface == nullptr || Texture == nullptr)
	{
		printf("��Ʈ �ε忡 �����߽��ϴ�\n");
		printf("���� : %s\n", SDL_GetError());
		return;
	}
	
	//��Ʈ �׸���
	DrawTexture(Texture, InPos);
	
	//�����̽��� �Ҵ��� �޸� ����
	SDL_FreeSurface(Surface);

	//�ؽ��Ŀ� �Ҵ��� �޸� ����
	SDL_DestroyTexture(Texture);
}

void SDLRenderer::DrawFont(uint32_t InKey, SDL_Color InColor, Vector2 InPos, const char* InText)
{
	if (Fonts.find(InKey) == Fonts.end())
	{
		printf("�߸��� ��Ʈ �̸��ϴϴ�\n");
		return;
	}

	SDL_Surface* Surface = TTF_RenderText_Solid(Fonts[InKey], InText, InColor);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);

	if (Surface == nullptr || Texture == nullptr)
	{
		printf("��Ʈ �ε忡 �����߽��ϴ�\n");
		printf("���� : %s\n", SDL_GetError());
		return;
	}

	//��Ʈ �׸���
	DrawTexture(Texture, InPos);

	//�����̽��� �Ҵ��� �޸� ����
	SDL_FreeSurface(Surface);

	//�ؽ��Ŀ� �Ҵ��� �޸� ����
	SDL_DestroyTexture(Texture);
}

void SDLRenderer::DrawSqure(Vector2 InPos, int InWidth, int InHeight)
{
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 50);


	SDL_Rect Rect = {InPos.PosX - InWidth * 0.5f, InPos.PosY - InHeight * 0.5f,
					 InWidth, InHeight};
					 

	SDL_RenderFillRect(Renderer, &Rect);
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
}

void SDLRenderer::DrawCircle(Vector2 InPos, float InRadius)
{
	//TODO ����
}

void SDLRenderer::DrawLine(Vector2 InFirstPos, Vector2 InSecondPos)
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 50);
	SDL_RenderDrawLine(Renderer, InFirstPos.PosX, InFirstPos.PosY, InSecondPos.PosX, InSecondPos.PosY);
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
}

void SDLRenderer::DrawBackground()
{
	DrawStretchedTexture(BackgroundTexture, Vector2(0,0), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
//	DrawStretchedTexture(PlayerTexture, Vector2(0, 0), Vector2(100, 100));
//DrawFont(MainFont,  Color, Vector2(0,0), "Hi~My Name is..");
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
	CloseTextures();
	CloseFonts();
	SDL_StopTextInput();

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);

	//SDL ����
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

void SDLRenderer::CloseFonts()
{
	TTF_CloseFont(LargeFont);
	TTF_CloseFont(MidFont);
	TTF_CloseFont(SmallFont);
	TTF_CloseFont(ExtremeFont);
}

void SDLRenderer::CloseTextures()
{
	SDL_DestroyTexture(PlayerTexture);
	SDL_DestroyTexture(BackgroundTexture);
	SDL_DestroyTexture(PlayerAttackEffectTexture);
	SDL_DestroyTexture(Star_S_Texture);
	SDL_DestroyTexture(Star_L_Texture);
	SDL_DestroyTexture(ScoreboardTexture);

	SDL_DestroyTexture(EntrySceneTexture);

	for (int i = 0; i < PlayerColorNum; i++)
	{
		SDL_DestroyTexture(PlayerColorTextures[i]);
	}

	SDL_DestroyTexture(CheckMarkTexture);
}
