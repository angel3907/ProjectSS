#include "stdafx.h"
#include "SDLRenderer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

void SDLRenderer::InitSDL()
{
	//SDL 초기화 - 비디오 기능
	SDL_Init(SDL_INIT_VIDEO);

	//이미지 초기화
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	//텍스트 초기화
	TTF_Init();
	
	//윈도우 생성
	//인자
	//1. 생성할 창 제목 (아스키 범위 문자열이 아니라면 UTF8 인코딩 되어있어야 함)
	//2,3 창의 초기위치
		// 모니터 좌측 상단 기준으로 해당 픽셀만큼 떨어진 위치에 창이 생성
		//SDL_WINDOWPOS_UNDEFINED : 적당한 위치에 자동으로 띄워줌
	//4,5 창의 크기
		//제목 표시줄, 3방면 테두리 크기를 제외한 순수 클라이언트 영역 크기.
	//6. 창의 속성
		//테두리 없는 창을 만들거나, 크기를 조정할 수 있도록 하거나 등등의 속성을 줄 수 있음.
	Window = SDL_CreateWindow(
		"Hello SDL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //화면 정중앙에 표시
		WINDOW_WIDTH, WINDOW_HEIGHT, 0
	);

	//렌더러 생성
	//인자
	//1. 렌더러를 생성할 윈도우
	//2. 렌더링에 어떤 드라이버를 사용할지 설정
		//PC환경에서는 DirectX / OpenGL / Software 셋중 하나를 선택해서 사용할 수 있음
		//-1을 넣으면 가장 첫번째로 사용 가능한 드라이버를 사용함
	//3. 렌더러 속성
	//SDL_RENDERER_ACCELERATED : 하드웨어 가속
	//SDL_RENDERER_PRESENTVSYNC : 수직 동기화 사용
	//사용할 필요 없으니 0
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

	//비트맵 파일 로드
	//Surface = SDL_LoadBMP(File);
	Surface = IMG_Load(File); //bmp 이외의 다른 이미지 로드
	if (Surface == nullptr)
	{
		printf("%s 경로의 파일을 읽을 수 없습니다.\n", File);
		printf("에러 : %s\n", SDL_GetError());
	}

	//서페이스로부터 텍스쳐를 생성
	//인자 1)렌더러 2)서페이스
	Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	if (Texture == nullptr)
	{
		printf("텍스쳐를 생성할 수 없습니다.\n");
	}

	//서페이스에 할당한 메모리 해제
	SDL_FreeSurface(Surface);

	return Texture;
}

TTF_Font* SDLRenderer::LoadFont(const char* File, int FontSize)
{
	TTF_Font* Font = TTF_OpenFont(File, FontSize);

	if (Font == nullptr)
	{
		printf("폰트를 불러오는 데 실패했습니다\n");
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
		Textures[i] = PlayerColorTextures[i]; //맵 키 예외 0~PlayerColorNum
	}

	//텍스쳐 맵에 등록
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
	
	//Src : 이미지 중 어떤 부분이 출력에 사용될지
	//Dst : 화면의 어느 영역에 이미지를 출력할지

	//Rect의 x,y : 이미지가 시작될 위치
	//		 w,h : 폭, 높이 
	//이미지의 특정 범위만 출력하거나, 특정 사이즈만큼 늘리거나 줄여서 출력할 수 있음.
	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	//이미지 전체를 출력할 것이므로
	//src = (0,0,imageWidth, imageHeight)
	//dst = (x,y,imageWidth, imageHeight)
	
	//텍스쳐의 정보를 얻어오는 함수
	//인자
	//1. 정보를 얻어올 텍스쳐, 2. 텍스쳐 픽셀 포맷, 3.텍스처 접근 정보, 4&5. 텍스쳐 width, height
	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	//인자로 받아온 위치가 그림의 중심이 되도록..
	Dst.x = InPos.PosX - Src.w * 0.5f;
	Dst.y = InPos.PosY - Src.h * 0.5f;
	Dst.w = Src.w;
	Dst.h = Src.h;

	//이미지의 Src 부분을 화면의 Dst에 출력해주는 함수
	//인자
	//1. 렌더러 2.텍스쳐 3,4 src / dst 범위
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

	//Src : 이미지 중 어떤 부분이 출력에 사용될지
	//Dst : 화면의 어느 영역에 이미지를 출력할지

	//Rect의 x,y : 이미지가 시작될 위치
	//		 w,h : 폭, 높이 
	//이미지의 특정 범위만 출력하거나, 특정 사이즈만큼 늘리거나 줄여서 출력할 수 있음.
	SDL_Rect Src, Dst;
	Src.x = Src.y = 0;

	//이미지 전체를 출력할 것이므로
	//src = (0,0,imageWidth, imageHeight)
	//dst = (x,y,imageWidth, imageHeight)

	//텍스쳐의 정보를 얻어오는 함수
	//인자
	//1. 정보를 얻어올 텍스쳐, 2. 텍스쳐 픽셀 포맷, 3.텍스처 접근 정보, 4&5. 텍스쳐 width, height
	SDL_QueryTexture(Texture, nullptr, nullptr, &Src.w, &Src.h);

	Dst.x = InPos.PosX;
	Dst.y = InPos.PosY;
	Dst.w = Src.w;
	Dst.h = Src.h;

	//이미지의 Src 부분을 화면의 Dst에 출력해주는 함수
	//인자
	//1. 렌더러 2.텍스쳐 3,4 src / dst 범위
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

	//Angle : 회전각
	//Flip : 뒤집기 옵션
	//	- SDL_FLIP_NONE	      : 안 뒤집음
	//	- SDL_FLIP_HORIZONTAL : 수평 뒤집기
	//	- SDL_FLIP_VERTICAL   : 수직 뒤집기
	//Center : 회전 / 뒤집기 중심
	SDL_RenderCopyEx(Renderer, Texture, &Src, &Dst, InAngle, &Center, InFlip);
}

void SDLRenderer::DrawFont(TTF_Font* InFont, SDL_Color InColor, Vector2 InPos, const char* InText)
{
	SDL_Surface* Surface = TTF_RenderText_Solid(InFont, InText, InColor);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
	
	if (Surface == nullptr || Texture == nullptr)
	{
		printf("폰트 로드에 실패했습니다\n");
		printf("에러 : %s\n", SDL_GetError());
		return;
	}
	
	//폰트 그리기
	DrawTexture(Texture, InPos);
	
	//서페이스에 할당한 메모리 해제
	SDL_FreeSurface(Surface);

	//텍스쳐에 할당한 메모리 해제
	SDL_DestroyTexture(Texture);
}

void SDLRenderer::DrawFont(uint32_t InKey, SDL_Color InColor, Vector2 InPos, const char* InText)
{
	if (Fonts.find(InKey) == Fonts.end())
	{
		printf("잘못된 폰트 이름일니다\n");
		return;
	}

	SDL_Surface* Surface = TTF_RenderText_Solid(Fonts[InKey], InText, InColor);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);

	if (Surface == nullptr || Texture == nullptr)
	{
		printf("폰트 로드에 실패했습니다\n");
		printf("에러 : %s\n", SDL_GetError());
		return;
	}

	//폰트 그리기
	DrawTexture(Texture, InPos);

	//서페이스에 할당한 메모리 해제
	SDL_FreeSurface(Surface);

	//텍스쳐에 할당한 메모리 해제
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
	//TODO 구현
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

	//SDL 정리
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
