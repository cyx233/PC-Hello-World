﻿// JumpingGame.cpp : 定义应用程序的入口点。
//

#include "JumpingGame.h"

using namespace std;



#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

//图像资源

HBITMAP bmp_Background; //背景图像
HBITMAP bmp_Title;		//标题背景
HBITMAP bmp_GameBg;		//游戏背景
HBITMAP bmp_GameBg2;
HBITMAP bmp_GameBg3;
HBITMAP bmp_GameBg4;
HBITMAP bmp_GameBg5;
HBITMAP bmp_EndBg;
HBITMAP bmp_EndBg2;
HBITMAP bmp_EndBg3;
HBITMAP bmp_StartBg;
HBITMAP bmp_Select;

HBITMAP bmp_StartButton;	//开始按钮图像
HBITMAP bmp_HelpButton;		//帮助按钮图像
HBITMAP bmp_MenuButton;		//菜单按钮图像
HBITMAP bmp_BackButton;		//返回按钮图像
HBITMAP bmp_ContinueButton; //继续按钮图像
HBITMAP bmp_RetryButton;	//重生按钮图像
HBITMAP bmp_SelectButton;	//选关按钮图像
HBITMAP bmp_PauseButton;	//继续按钮图像
HBITMAP bmp_NextButton;	//继续按钮图像
HBITMAP bmp_SoundButton;//bgm按钮
HBITMAP bmp_SilentButton;//静音


HBITMAP bmp_STAGE1;
HBITMAP bmp_STAGE2;
HBITMAP bmp_STAGE3;
HBITMAP bmp_STAGE4;
HBITMAP bmp_STAGE5;
HBITMAP bmp_HELP_STAGE1;
HBITMAP bmp_HELP_STAGE2;
HBITMAP bmp_HELP_STAGE3;
HBITMAP bmp_HELP_STAGE4;
HBITMAP bmp_TitleBg;

HBITMAP bmp_Name;			//姓名栏图像

HBITMAP bmp_Hero; //主角图像

HBITMAP bmp_BlockDirt;  //泥土图像
HBITMAP bmp_BlockGrass; //草地图像
HBITMAP bmp_BlockSave;	//存档点图像
HBITMAP bmp_BlockPedal;	//踏板图像
HBITMAP bmp_BlockOnoff;	//开关图像
HBITMAP bmp_BlockPedalon;//触发图像
HBITMAP bmp_BlockApple;	//终点图像

HBITMAP bmp_BlockThorn;		//尖刺图像
HBITMAP bmp_BlockFire;		//火焰图像
HBITMAP bmp_BlockIce;		//冰焰图像

HBITMAP bmp_BloodBody;	//带血尸体图像
HBITMAP bmp_BurnedBody; //燃烧尸体图像
HBITMAP bmp_FreezedBody;//冰冻尸体图像

Stage* currentStage = NULL; //当前场景状态
Block* CurrentSave = NULL;	//当前存档点
Hero* theHero = NULL; //主角状态
vector<Button*> buttons; //按钮
Name*theName;			//姓名栏
vector<Block*>blocks; //方块         
vector<int>namelist;//名单
int lucky;//最后的人
bool nameflag = true;//滚动打印名单
bool silent;//静音


int mouseX = 0; //鼠标横坐标
int mouseY = 0; //鼠标纵坐标
bool mouseDown = false; //鼠标左键
bool keyUpDown = false; //上键
bool keyDownDown = false; //下键
bool keyLeftDown = false; //左键
bool keyRightDown = false; //右键
bool keySpaceDown = false; //空格键
bool keyEscDown = false;//Esc键


double const PI = acos(double(-1));
#pragma endregion



#pragma region Win32程序框架



// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_JUMPINGGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPINGGAME));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}




//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JUMPINGGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			// 初始化游戏窗体
			InitGame(hWnd, wParam, lParam);
			break;
		case WM_KEYDOWN:
			// 键盘按下事件
			KeyDown(hWnd, wParam, lParam);
			break;
		case WM_KEYUP:
			// 键盘松开事件
			KeyUp(hWnd, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			// 鼠标移动事件
			MouseMove(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			// 鼠标左键按下事件
			LButtonDown(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			// 鼠标左键松开事件
			LButtonUp(hWnd, wParam, lParam);
			break;
		case WM_TIMER:
			// 定时器事件
			if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
			break;
		case MM_MCINOTIFY:
			//音效
		{
			if (silent)
			{
				break;
			}

			if (lParam == 1)
				mciSendString(TEXT("play BGM from 0 notify"), NULL, 0, hWnd);

			if (lParam == 3)
				mciSendString(TEXT("play END from 0 notify"), NULL, 0, hWnd);

			break;
		}
		case WM_PAINT:
			// 绘图
			Paint(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion



#pragma region 加载图像资源 添加按钮


void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载声音资源
	mciSendString(
		TEXT("open \"res\\sounds\\bgm.wav\" type waveaudio alias BGM"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\finish.wav\" type waveaudio alias FINISH"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\end.wav\" type waveaudio alias END"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\button.wav\" type waveaudio alias BUTTON"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\fire.wav\" type waveaudio alias FIRE"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\bleed.wav\" type waveaudio alias BLEED"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\ice.wav\" type waveaudio alias ICE"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\onoff.wav\" type waveaudio alias ONOFF"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\save.wav\" type waveaudio alias SAVE"),
		NULL, 0, NULL);
	mciSendString(
		TEXT("open \"res\\sounds\\jump.wav\" type waveaudio alias JUMP"),
		NULL, 0, NULL);

	mciSendString(TEXT("play BGM from 0 notify"), NULL, 0, hWnd);
	mciSendString(TEXT("play END notify"), NULL, 0, hWnd);


	//加载图像资源
	bmp_Title = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_TITLE));
	bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	bmp_GameBg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GAMEBG));
	bmp_GameBg2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GAMEBG2));
	bmp_GameBg3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GAMEBG3));
	bmp_GameBg4 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GAMEBG4));
	bmp_GameBg5 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GAMEBG5));
	bmp_EndBg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ENDBG));
	bmp_EndBg2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ENDBG2));
	bmp_EndBg3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ENDBG3));
	bmp_StartBg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STARTBG));
	bmp_Select = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SELECT));
	bmp_TitleBg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_TITLEBG));

	bmp_Name = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_NAME));

	bmp_Hero = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HERO));

	bmp_BlockDirt = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_DIRT));
	bmp_BlockGrass = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	bmp_BlockThorn = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_THORN));
	bmp_BlockIce = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ICE));
	bmp_BlockSave = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SAVE));
	bmp_BlockFire = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FIRE));
	bmp_BlockPedal = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_PEDAL));
	bmp_BlockPedalon = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_PEDALON));
	bmp_BlockApple = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_APPLE));
	bmp_BlockOnoff = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_ONOFF));

	bmp_BloodBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLOODBODY));
	bmp_BurnedBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BURNEDBODY));
	bmp_FreezedBody = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FREEZED));


	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_HelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP));
	bmp_MenuButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_MENU));
	bmp_BackButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BACK));
	bmp_RetryButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RETRY));
	bmp_PauseButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_PAUSE));
	bmp_ContinueButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CONTINUE));
	bmp_NextButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_NEXT));
	bmp_SoundButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SOUND));
	bmp_SilentButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_SILENT));

	bmp_STAGE1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE1));
	bmp_STAGE2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE2));
	bmp_STAGE3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE3));
	bmp_STAGE4 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE4));
	bmp_STAGE5 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE5));
	bmp_HELP_STAGE1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP1));
	bmp_HELP_STAGE2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP2));
	bmp_HELP_STAGE3 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP3));
	bmp_HELP_STAGE4 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_HELP4));



	//添加按钮
	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 567, 400);
	buttons.push_back(startButton);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 567, 500);
	buttons.push_back(helpButton);
	for (int i = 3; i <= 11; i++)
	{
		Button* pauseButton = CreateButton(i * 1000 + BUTTON_PAUSE, bmp_PauseButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1232, 0);
		buttons.push_back(pauseButton);
	}
	Button* menuButton = CreateButton(100 * 1000 + BUTTON_MENU, bmp_MenuButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1232, BLOCK_SIZE_Y + 5);
	buttons.push_back(menuButton);
	menuButton = CreateButton(STAGE_ENDSTORY * 1000 + BUTTON_MENU, bmp_MenuButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1250 - BLOCK_SIZE_X - 10, 700 - BLOCK_SIZE_Y - 10);
	buttons.push_back(menuButton);

	Button* retryButton = CreateButton(100 * 1000 + BUTTON_RETRY, bmp_RetryButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1232, BLOCK_SIZE_Y * 2 + 10);
	buttons.push_back(retryButton);

	Button* backButton = CreateButton(100 * 1000 + BUTTON_BACK, bmp_BackButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1232, BLOCK_SIZE_Y * 3 + 15);
	buttons.push_back(backButton);
	backButton = CreateButton(STAGE_SELECT * 1000 + BUTTON_BACK, bmp_BackButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 20 * BLOCK_SIZE_X - BLOCK_SIZE_X, BLOCK_SIZE_Y * 20);
	buttons.push_back(backButton);

	Button* soundButton = CreateButton(100 * 1000 + BUTTON_SOUND, bmp_SoundButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1222 - BLOCK_SIZE_X, 0);
	buttons.push_back(soundButton);

	Button* title = CreateButton(BUTTON_LABEL, bmp_Title, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
	buttons.push_back(title);

	Button* nextbutton = CreateButton(STAGE_ENDSTORY3 * 1000 + BUTTON_NEXT, bmp_NextButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1250 - BLOCK_SIZE_X - 10, 700 - BLOCK_SIZE_Y - 10);
	buttons.push_back(nextbutton);

	nextbutton = CreateButton(STAGE_ENDSTORY2 * 1000 + BUTTON_NEXT, bmp_NextButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1250 - BLOCK_SIZE_X - 10, 700 - BLOCK_SIZE_Y - 10);
	buttons.push_back(nextbutton);

	nextbutton = CreateButton(STAGE_STARTSTORY * 1000 + BUTTON_NEXT, bmp_NextButton, BLOCK_SIZE_X, BLOCK_SIZE_Y, 1250 - BLOCK_SIZE_X - 10, 700 - BLOCK_SIZE_Y - 10);
	buttons.push_back(nextbutton);





	Button* label = CreateButton(STAGE_SELECT * 1000 + BUTTON_LABEL, bmp_STAGE1, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT, 100, (BUTTON_LABEL_HEIGHT + 20));
	buttons.push_back(label);
	label = CreateButton(STAGE_SELECT * 1000 + BUTTON_LABEL, bmp_STAGE2, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT, 100, 2 * (BUTTON_LABEL_HEIGHT + 20));
	buttons.push_back(label);
	label = CreateButton(STAGE_SELECT * 1000 + BUTTON_LABEL, bmp_STAGE3, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT, 100, 3 * (BUTTON_LABEL_HEIGHT + 20));
	buttons.push_back(label);
	label = CreateButton(STAGE_SELECT * 1000 + BUTTON_LABEL, bmp_STAGE4, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT, 100, 4 * (BUTTON_LABEL_HEIGHT + 20));
	buttons.push_back(label);
	label = CreateButton(STAGE_SELECT * 1000 + BUTTON_LABEL, bmp_STAGE5, BUTTON_LABEL_WIDTH, BUTTON_LABEL_HEIGHT, 100, 5 * (BUTTON_LABEL_HEIGHT + 20));
	buttons.push_back(label);


	//初始化姓名栏
	theName = CreateName(bmp_Name, 567, 700);

	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);



	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}


#pragma endregion



#pragma region 事件处理函数
// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_UP:
			keyUpDown = true;
			break;
		case VK_DOWN:
			keyDownDown = true;
			break;
		case VK_LEFT:
			keyLeftDown = true;
			break;
		case VK_RIGHT:
			keyRightDown = true;
			break;
		case VK_SPACE:
			keySpaceDown = true;
			break;
		case VK_ESCAPE:
		{
			if (!keyEscDown)
			{
				if (currentStage->timerOn)
					for (int i = 0; i < buttons.size(); i++)
					{
						Button* button = buttons[i];
						if (button->visible&&
							button->buttonID == currentStage->stageID * 1000 + BUTTON_PAUSE)
						{
							button->buttonID = button->buttonID - BUTTON_PAUSE + BUTTON_CONTINUE;
							button->img = bmp_ContinueButton;
						}

						if ((buttons[i]->buttonID == 100 * 1000 + BUTTON_MENU)
							|| (buttons[i]->buttonID == 100 * 1000 + BUTTON_RETRY))
							buttons[i]->visible = true;
						InvalidateRect(hWnd, NULL, FALSE);
						currentStage->timerOn = false;
					}
				else
				{
					for (int i = 0; i < buttons.size(); i++)
					{
						Button* button = buttons[i];
						if (button->visible&&
							button->buttonID == currentStage->stageID * 1000 + BUTTON_CONTINUE)
						{
							button->buttonID = button->buttonID - BUTTON_CONTINUE + BUTTON_PAUSE;
							button->img = bmp_PauseButton;
						}

						if ((buttons[i]->buttonID == 100 * 1000 + BUTTON_MENU)
							|| (buttons[i]->buttonID == 100 * 1000 + BUTTON_RETRY))
							buttons[i]->visible = false;
						InvalidateRect(hWnd, NULL, FALSE);
						currentStage->timerOn = true;
					}
				}
			}
			keyEscDown = true;
			break;
		}
		default:
			break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case VK_UP:
			keyUpDown = false;
			break;
		case VK_DOWN:
			keyDownDown = false;
			break;
		case VK_LEFT:
			keyLeftDown = false;
			break;
		case VK_RIGHT:
			keyRightDown = false;
			break;
		case VK_SPACE:
			keySpaceDown = false;
			break;
		case VK_ESCAPE:
			keyEscDown = false;
			break;
		default:
			break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;
	//鼠标与按钮交互
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible && button->buttonID!=BUTTON_LABEL)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				if (!silent)
					mciSendString(TEXT("play BUTTON from 0"), NULL, 0, hWnd);
				switch (button->buttonID % 100) {
					case BUTTON_STARTGAME:
					{
						InitStage(hWnd, STAGE_STARTSTORY);
						return;
					}

					case BUTTON_HELP:
					{
						InitStage(hWnd, STAGE_HELP_1);
						return;
					}

					case BUTTON_MENU:
					{
						InitStage(hWnd, STAGE_STARTMENU);
						return;
					}

					case BUTTON_BACK:
					{
						if (currentStage->stageID >= STAGE_1 &&
							currentStage->stageID <= STAGE_5)
							InitStage(hWnd, STAGE_SELECT);
						else if (currentStage->stageID == STAGE_SELECT||
							currentStage->stageID == STAGE_HELP_1)
							InitStage(hWnd, STAGE_STARTMENU);
						else if (currentStage->stageID > STAGE_HELP_1 &&
							currentStage->stageID <= STAGE_HELP_4)
							InitStage(hWnd, currentStage->stageID - 1);
						return;
					}

					case BUTTON_CONTINUE:
					{
						currentStage->timerOn = true;
						button->buttonID = button->buttonID + BUTTON_PAUSE - BUTTON_CONTINUE;
						button->img = bmp_PauseButton;
						for (int j = 0; j < buttons.size(); j++)
						{
							if ((buttons[j]->buttonID == 100 * 1000 + BUTTON_MENU)
								|| (buttons[j]->buttonID == 100 * 1000 + BUTTON_RETRY)
								|| (buttons[j]->buttonID == 100 * 1000 + BUTTON_BACK))
								buttons[j]->visible = false;
						}
						return;
					}

					case BUTTON_RETRY:
					{
						InitStage(hWnd, currentStage->stageID);
						return;
					}

					case BUTTON_PAUSE:
					{
						button->buttonID = button->buttonID - BUTTON_PAUSE + BUTTON_CONTINUE;
						button->img = bmp_ContinueButton;
						for (int j = 0; j < buttons.size(); j++)
						{
							if ((buttons[j]->buttonID == 100 * 1000 + BUTTON_MENU)
								|| (buttons[j]->buttonID == 100 * 1000 + BUTTON_RETRY)
								|| (buttons[j]->buttonID == 100 * 1000 + BUTTON_BACK))
								buttons[j]->visible = true;
						}
						InvalidateRect(hWnd, NULL, FALSE);
						currentStage->timerOn = false;
						return;
					}

					case BUTTON_NEXT:
					{
						switch (currentStage->stageID)
						{
							case STAGE_ENDSTORY3:
								InitStage(hWnd, STAGE_ENDSTORY);
								break;
							default:
								InitStage(hWnd, currentStage->stageID + 1);
								break;
						}
						return;
					}

					case BUTTON_SILENT:
					{
						button->buttonID = button->buttonID + BUTTON_SOUND - BUTTON_SILENT;
						button->img = bmp_SoundButton;
						silent = false;
						switch (currentStage->stageID)
						{
							case STAGE_ENDSTORY:
							{
								mciSendString(TEXT("resume END"), NULL, 0, hWnd);
								break;
							}

							case STAGE_ENDSTORY3:
							{
								mciSendString(TEXT("resume END"), NULL, 0, hWnd);
								break;
							}

							case STAGE_ENDSTORY2:
								break;

							default:
							{
								mciSendString(TEXT("resume BGM"), NULL, 0, hWnd);
								break;
							}
						}
						InvalidateRect(hWnd, NULL, FALSE);
						return;
					}
					
					
					case BUTTON_SOUND:
					{
						button->buttonID = button->buttonID - BUTTON_SOUND + BUTTON_SILENT;
						button->img = bmp_SilentButton;
						silent = true;
						switch (currentStage->stageID)
						{
							case STAGE_ENDSTORY:
							{
								mciSendString(TEXT("pause END"), NULL, 0, hWnd);
								break;
							}

							case STAGE_ENDSTORY3:
							{
								mciSendString(TEXT("pause END"), NULL, 0, hWnd);
								break;
							}

							default:
							{
								mciSendString(TEXT("pause BGM"), NULL, 0, hWnd);
								break;
							}
						}
						InvalidateRect(hWnd, NULL, FALSE);
						return;
					}

					case BUTTON_LABEL:
					{
						if (button->img == bmp_STAGE1)
						{
							InitStage(hWnd, STAGE_1);
							return;
						}
						if (button->img == bmp_STAGE2)
						{
							InitStage(hWnd, STAGE_2);
							return;
						}
						if (button->img == bmp_STAGE3)
						{
							InitStage(hWnd, STAGE_3);
							return;
						}
						if (button->img == bmp_STAGE4)
						{
							InitStage(hWnd, STAGE_4);
							return;
						}
						if (button->img == bmp_STAGE5)
						{
							InitStage(hWnd, STAGE_5);
							return;
						}
						return;
					}

					default:
						return;
				}
			}
		}
	}
}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateHero(hWnd); //刷新主角
	UpdateSurround(hWnd); //

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion



#pragma region 新建结构体函数

// 添加姓名栏函数
Name* CreateName(HBITMAP img, int x, int y)
{
	Name* name = new Name();
	name->visible = true;
	name->img = img;
	name->x = x;
	name->y = y;
	name->frame = 0;
	return name;
}


// 添加主角函数
Hero* CreateHero(HBITMAP img, int x, int y)
{
	Hero* hero = new Hero();
	hero->img = img;
	hero->x = x;
	hero->y = y;
	hero->frame = 0;
	hero->vx = 0;
	hero->vy = 0;
	hero->basevx = 0;
	UpdateName();
	return hero;
}


// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;
	button->visible = false;
	return button;
}


//添加方块函数
Block* CreateBlock(int blockID, HBITMAP img, int width, int height, int x, int y)
{
	Block*block = new Block();
	block->blockID = blockID;
	block->img = img;
	block->width = width;
	block->height = height;
	block->x = x;
	block->y = y;
	block->visible = false;
	block->frame = 0;
	block->vx = 0;
	block->prevx = 0;
	block->prevy = 0;
	block->m = 0;
	block->n = 0;
	block->turnon = false;
	block->link = 0;
	return block;
}


#pragma endregion



#pragma region 加载数据资源函数


// 初始化按钮函数(stageID+1)*1000+i=第i个buttonID
void InitButton(HWND hWnd, int stageID)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->buttonID / 1000  == stageID)
		{
			button->visible = true;
		}
		else
		{
			button->visible = false;
		}

		if (button->buttonID % 100 == BUTTON_CONTINUE)
		{
			button->buttonID = button->buttonID - BUTTON_CONTINUE + BUTTON_PAUSE;
			button->img = bmp_PauseButton;
		}

		if (button->buttonID % 100 == BUTTON_SOUND || button->buttonID % 100 == BUTTON_SILENT)
			button->visible = true;
	}
}


// 初始化方块函数(stageID+1)*1000+i=第i个blockID
void InitBlock(HWND hWnd, int stageID)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];

		if (block->blockID / 1000 == stageID)
		{
			switch (block->blockID % 100)
			{
				case BLOCK_FIRE:
					block->visible = !((block->blockID % 1000) / 100);
					break;
				case BLOCK_ICE:
					block->visible = !((block->blockID % 1000) / 100);
					break;
				case BLOCK_MOVETHORN:
				{
					block->visible = true;
					block->turnon = !((block->blockID % 1000) / 100);
					break;
				}
				default:
					block->visible = true;
			}
		}
		else
			block->visible = false;
	}
}


// 初始化地图函数
void InitMap(HWND hWnd, int stageID)
{

	//释放缓存
	for (int i = 0; i < blocks.size(); i++)
	{
		delete blocks[i];
		blocks[i] = NULL;
	}
	vector<Block*>().swap(blocks);



	//添加方块
	Block* normal, *thorn, *savepoint, *fire, *ice, *pedal, *onoff, *apple;


	switch (stageID)
	{
		case STAGE_STARTMENU:
		{
			for (int i = 0; i < 40; i++)
			{
				thorn = CreateBlock(STAGE_STARTMENU * 1000 + BLOCK_THORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600);
				blocks.push_back(thorn);

				savepoint = CreateBlock(STAGE_STARTMENU * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 530);
				CurrentSave = savepoint;
			}
			break;
		}
		case STAGE_1:
		{
			onoff = CreateBlock(STAGE_1 * 1000 + BLOCK_ONOFF, bmp_BlockOnoff, BLOCK_SIZE_X, BLOCK_SIZE_Y, 36 * BLOCK_SIZE_X, 5 * BLOCK_SIZE_Y);
			onoff->link = 1;
			blocks.push_back(onoff);

			for (int i = 0; i < 40; i++)
			{
				for (int j = 0; j <= 5; j++)
				{
					if (i == 36 && j == 5)
						continue;
					else if(i>30)
					{
						normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
				}

				if (i <= 5 || (i >= 13 && i <= 16))
				{
					normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}
				else if (i <= 12)
				{
					thorn = CreateBlock(STAGE_1 * 1000 + BLOCK_THORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
					blocks.push_back(thorn);
				}

				if (i >= 19 && i <= 26)
				{
					normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
					normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

				if (i <= 26 || i >= 31)
					for (int j = 15; j <= 24; j++)
					{
						if (i >= 17 && j <= 18 && i <= 24)
							continue;
						else
						{
							normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
						if (i == 25 || i == 26)
						{
							normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}

					}

				if (i >= 31)
					for (int j = 9; j <= 14; j++)
					{

						normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
						blocks.push_back(normal);

						normal = CreateBlock(STAGE_1 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 9 * BLOCK_SIZE_Y);
						blocks.push_back(normal);

					}

			}

			thorn = CreateBlock(STAGE_1 * 1000 + BLOCK_THORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, 30 * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
			blocks.push_back(thorn);

			for (int i = 14; i <= 18; i++)
			{
				fire = CreateBlock(STAGE_1 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 19 * BLOCK_SIZE_X, i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}

			for (int i = 0; i <= 11; i++)
			{
				fire = CreateBlock(STAGE_1 * 1000 + BLOCK_FIRE + 100, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 19 * BLOCK_SIZE_X, i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}

			savepoint = CreateBlock(STAGE_1 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 14 * BLOCK_SIZE_Y - BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_1 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 22 * BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
			blocks.push_back(apple);
			break;
		}

		case STAGE_2:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i <= 9)
				{
					if (i == 9)
					{
						pedal = CreateBlock(STAGE_2 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, i* BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
						pedal->link = 1;
						blocks.push_back(pedal);
					}
					else
					{
						normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
				}

				for (int j = 19; j < 24; j++)
				{
					normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

				if (i > 9)
				{
					for (int j = 14; j <= 18; j++)
					{
						if (j == 14)
						{
							if (i != 19)
							{
								normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
								blocks.push_back(normal);
							}
							else
							{
								pedal = CreateBlock(STAGE_2 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, i* BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
								pedal->link = 2;
								blocks.push_back(pedal);
							}

						}
						else
						{
							normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
					}
				}

				if (i > 19)
				{
					for (int j = 11; j <= 14; j++)
					{
						if (j == 11 && i <= 30)
						{
							normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
						else
						{
							normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
					}
				}


				if (i == 20)
				{
					for (int j = 6; j <= 10; j++)
					{
						fire = CreateBlock(STAGE_2 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i* BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
						fire->link = 1;
						blocks.push_back(fire);
					}
				}

				if (i > 30)
				{
					for (int j = 9; j <= 10; j++)
					{
						if (j == 9)
						{
							normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
						else
						{
							normal = CreateBlock(STAGE_2 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
							blocks.push_back(normal);
						}
					}
				}

			}



			ice = CreateBlock(STAGE_2 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 5 * BLOCK_SIZE_X, 17 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			ice = CreateBlock(STAGE_2 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 10 * BLOCK_SIZE_X, 10 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			fire = CreateBlock(STAGE_2 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 8.5* BLOCK_SIZE_X, 17 * BLOCK_SIZE_Y);
			fire->link = 2;
			blocks.push_back(fire);

			fire = CreateBlock(STAGE_2 * 1000 + BLOCK_FIRE + 100, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 6 * BLOCK_SIZE_X, 17 * BLOCK_SIZE_Y);
			fire->link = 2;
			blocks.push_back(fire);



			savepoint = CreateBlock(STAGE_2 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 17 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_2 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 35 * BLOCK_SIZE_X, 8 * BLOCK_SIZE_Y);
			blocks.push_back(apple);
			break;
		}

		case STAGE_3:
		{
			for (int i = 0; i < 40; i++)
			{
				for (int j = 15; j < 24; j++)
				{
					normal = CreateBlock(STAGE_3 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

				if (i < 8 || (i >= 16 && i < 24) || i >= 32)
				{
					if (i != 4)
					{
						normal = CreateBlock(STAGE_3 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
				}
				else
				{
					thorn = CreateBlock(STAGE_3 * 1000 + BLOCK_THORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
					blocks.push_back(thorn);

					if (i >= 8 && i < 16)
					{
						fire = CreateBlock(STAGE_3 * 1100 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
						fire->link = 1;
						blocks.push_back(fire);
					}
					else
					{
						fire = CreateBlock(STAGE_3 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
						fire->link = 1;
						blocks.push_back(fire);
					}
				}
				onoff = CreateBlock(STAGE_3 * 1000 + BLOCK_ONOFF, bmp_BlockOnoff, BLOCK_SIZE_X, BLOCK_SIZE_Y, 4 * BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
				onoff->link = 1;
				blocks.push_back(onoff);

				apple = CreateBlock(STAGE_2 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 35 * BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
				blocks.push_back(apple);

			}


			savepoint = CreateBlock(STAGE_3 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 3 * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			savepoint = CreateBlock(STAGE_3 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 20 * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);

			apple = CreateBlock(STAGE_3 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 35 * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
			blocks.push_back(apple);
			break;
		}

		case STAGE_4:
		{
			for (int i = 0; i < 40; i++)
				for (int j = 0; j <= 3; j++)
				{
					normal = CreateBlock(stageID * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

			for (int i = 0; i < 24; i++)
			{
				normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 39 * BLOCK_SIZE_X, i * BLOCK_SIZE_Y);
				blocks.push_back(normal);
			}

			for (int i = 0; i < 40; i++)
			{
				for (int j = 19; j < 24; j++)
				{
					normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}


				if (i < 8)
				{
					normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}


				if (i < 10)
				{
					normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}


				if (i > 10)
				{
					if (i < 39)
					{
						normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 16 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
					else
					{
						normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 16 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
					normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 17 * BLOCK_SIZE_Y);
					blocks.push_back(normal);

					normal = CreateBlock(STAGE_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

				if (i >= 19 && i <= 21)
				{
					fire = CreateBlock(STAGE_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
					fire->link = 2;
					blocks.push_back(fire);

					fire = CreateBlock(STAGE_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 11 * BLOCK_SIZE_Y);
					fire->link = 2;
					blocks.push_back(fire);

					if (i != 20)
					{
						fire = CreateBlock(STAGE_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
						fire->link = 2;
						blocks.push_back(fire);
					}
				}

			}

			for (int i = 11; i < 16; i++)
			{
				fire = CreateBlock(STAGE_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 13 * BLOCK_SIZE_X, i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}
			for (int i = 35; i <= 37; i++)
			{
				fire = CreateBlock(STAGE_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X - 16, 15 * BLOCK_SIZE_Y);
				fire->link = 3;
				blocks.push_back(fire);
			}

			pedal = CreateBlock(STAGE_4 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, 10 * BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
			pedal->link = 1;
			blocks.push_back(pedal);

			pedal = CreateBlock(STAGE_4 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, 38 * BLOCK_SIZE_X, 16 * BLOCK_SIZE_Y);
			pedal->link = 2;
			blocks.push_back(pedal);

			pedal = CreateBlock(STAGE_4 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, 38 * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
			pedal->link = 3;
			blocks.push_back(pedal);

			ice = CreateBlock(STAGE_4 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 6 * BLOCK_SIZE_X, 10 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			ice = CreateBlock(STAGE_4 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 23 * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			savepoint = CreateBlock(STAGE_4 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0 * BLOCK_SIZE_X, 17 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			savepoint = CreateBlock(STAGE_4 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0 * BLOCK_SIZE_X, 13 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_4 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 20 * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
			blocks.push_back(apple);
			break;

		}

		case STAGE_5:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i > 14 || i < 6)
				{
					if (i <= 20) 
					{
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 20 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
					for (int j = 21; j < 24; j++)
					{
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
				}

				if(i < 19)
				{
					if (i > 6)
					{
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 16 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
					else
					{
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 14 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 15 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 16 * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}

				}

				if (i > 8 && i < 14)
				{
					normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

				if (i > 20)
				{
					normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 18 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
					for (int j = 19; j < 24; j++)
					{
						normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, j * BLOCK_SIZE_Y);
						blocks.push_back(normal);
					}
				}

				if (i > 25)
				{
					normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}
				if (i <= 25 && i > 20)
				{
					fire = CreateBlock(STAGE_5 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
					fire->link = 1;
					blocks.push_back(fire);
				}
			}

			for (int i = 7; i <= 12; i++)
			{
				fire = CreateBlock(STAGE_5 * 1100 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 16 * BLOCK_SIZE_X, i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}

			thorn = CreateBlock(STAGE_5 * 1000 + BLOCK_MOVETHORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, 6 * BLOCK_SIZE_X, 20 * BLOCK_SIZE_Y);
			thorn->m = 6 * BLOCK_SIZE_X; thorn->n = 14 * BLOCK_SIZE_X; thorn->vx = 2.0;
			blocks.push_back(thorn);

			thorn = CreateBlock(STAGE_5 * 1000 + BLOCK_MOVETHORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, 14 * BLOCK_SIZE_X, 12 * BLOCK_SIZE_Y);
			thorn->m = 14 * BLOCK_SIZE_X; thorn->n = 25 * BLOCK_SIZE_X; thorn->vx = 2.0; thorn->link = 1;
			blocks.push_back(thorn);

			normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, 9*BLOCK_SIZE_X, 9 * BLOCK_SIZE_Y);
			blocks.push_back(normal);

			onoff = CreateBlock(STAGE_5 * 1000 + BLOCK_ONOFF, bmp_BlockOnoff, BLOCK_SIZE_X, BLOCK_SIZE_Y, 10 * BLOCK_SIZE_X, 9 * BLOCK_SIZE_Y);
			onoff->link = 1;
			blocks.push_back(onoff);

			normal = CreateBlock(STAGE_5 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, 11*BLOCK_SIZE_X, 9 * BLOCK_SIZE_Y);
			blocks.push_back(normal);

			apple = CreateBlock(STAGE_5 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 35 * BLOCK_SIZE_X, 11 * BLOCK_SIZE_Y);
			blocks.push_back(apple);

			savepoint = CreateBlock(STAGE_5 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0 * BLOCK_SIZE_X, 19 * BLOCK_SIZE_Y);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;
			break;
		}

		case STAGE_HELP_1:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i <= 15 || i >= 25)
				{
					normal = CreateBlock(STAGE_HELP_1 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600);
					blocks.push_back(normal);

					savepoint = CreateBlock(STAGE_HELP_1 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 568);
					blocks.push_back(savepoint);
					CurrentSave = savepoint;
				}
			}

			thorn = CreateBlock(STAGE_HELP_1 * 1000 + BLOCK_MOVETHORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, 16 * BLOCK_SIZE_X, 600);
			thorn->m = 16 * BLOCK_SIZE_X; thorn->n = 24 * BLOCK_SIZE_X; thorn->vx = 2.0;
			blocks.push_back(thorn);

			savepoint = CreateBlock(STAGE_HELP_1 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 568);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_HELP_1 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 800, 568);
			blocks.push_back(apple);
			break;
		}
		case STAGE_HELP_2:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i <= 15 || i >= 25)
				{
					normal = CreateBlock(STAGE_HELP_2 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600);
					blocks.push_back(normal);
				}
				else
				{
					thorn = CreateBlock(STAGE_HELP_2 * 1000 + BLOCK_THORN, bmp_BlockThorn, BLOCK_SIZE_X, BLOCK_SIZE_Y, i * BLOCK_SIZE_X, 600);
					blocks.push_back(thorn);
				}
			}

			for (int i = 0; i < 3; i++)
			{
				fire = CreateBlock(STAGE_HELP_2 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 20 * BLOCK_SIZE_X, 568 - i * BLOCK_SIZE_Y);
				blocks.push_back(fire);
			}



			savepoint = CreateBlock(STAGE_HELP_2 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 568);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_HELP_2 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 800, 568);
			blocks.push_back(apple);
			break;
		}

		case STAGE_HELP_3:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i <= 15)
				{
					normal = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600);
					blocks.push_back(normal);
				}
				else
				{
					normal = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600 - 3 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}
			}

			for (int i = 3; i < 10; i++)
			{
				fire = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 30 * BLOCK_SIZE_X, 568 - i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}

			pedal = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, 33 * BLOCK_SIZE_X, 568- 3 * BLOCK_SIZE_Y);
			pedal->link = 1;
			blocks.push_back(pedal);


			ice = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 19 * BLOCK_SIZE_X, 600 - 7 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			ice = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_ICE, bmp_BlockIce, BLOCK_SIZE_X, BLOCK_SIZE_Y, 5 * BLOCK_SIZE_X, 600 - 4 * BLOCK_SIZE_Y);
			blocks.push_back(ice);

			savepoint = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 568);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			apple = CreateBlock(STAGE_HELP_3 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 36 * BLOCK_SIZE_X, 568 - 3 * BLOCK_SIZE_Y);
			blocks.push_back(apple);
			break;
		}

		case STAGE_HELP_4:
		{
			for (int i = 0; i < 40; i++)
			{
				if (i != 3)
				{
					normal = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600);
					blocks.push_back(normal);
				}
				if (i >= 6 && i <= 8)
				{
					normal = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_NORMAL, bmp_BlockGrass, BLOCK_SIZE_X, BLOCK_SIZE_Y, i*BLOCK_SIZE_X, 600 - 4 * BLOCK_SIZE_Y);
					blocks.push_back(normal);
				}

			}


			for (int i = 0; i < 10; i++)
			{
				fire = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_FIRE, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 10 * BLOCK_SIZE_X, 568 - i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);

				if (i <= 2)
				{
					fire = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_FIRE + 100, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 4 * BLOCK_SIZE_X, 568 - i * BLOCK_SIZE_Y);
					fire->link = 2;
					blocks.push_back(fire);
				}

				fire = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_FIRE + 100, bmp_BlockFire, BLOCK_SIZE_X, BLOCK_SIZE_Y, 20 * BLOCK_SIZE_X, 568 - i * BLOCK_SIZE_Y);
				fire->link = 1;
				blocks.push_back(fire);
			}

			pedal = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_PEDAL, bmp_BlockPedal, BLOCK_SIZE_X, BLOCK_SIZE_Y, 3 * BLOCK_SIZE_X, 600);
			pedal->link = 2;
			blocks.push_back(pedal);

			normal = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 6 * BLOCK_SIZE_X, 600 - 3 * BLOCK_SIZE_Y);
			blocks.push_back(normal);

			normal = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_NORMAL, bmp_BlockDirt, BLOCK_SIZE_X, BLOCK_SIZE_Y, 8 * BLOCK_SIZE_X, 600 - 3 * BLOCK_SIZE_Y);
			blocks.push_back(normal);

			onoff = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_ONOFF, bmp_BlockOnoff, BLOCK_SIZE_X, BLOCK_SIZE_Y, 7 * BLOCK_SIZE_X, 600 - 3 * BLOCK_SIZE_Y);
			onoff->link = 1;
			blocks.push_back(onoff);

			savepoint = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 0, 568);
			blocks.push_back(savepoint);
			CurrentSave = savepoint;

			savepoint = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_SAVE, bmp_BlockSave, BLOCK_SIZE_X, BLOCK_SIZE_Y, 15 * BLOCK_SIZE_X, 568);
			blocks.push_back(savepoint);


			apple = CreateBlock(STAGE_HELP_4 * 1000 + BLOCK_APPLE, bmp_BlockApple, BLOCK_SIZE_X, BLOCK_SIZE_Y, 800, 568);
			blocks.push_back(apple);
			break;
		}
		default:
			break;
	}

	//初始化方块
	InitBlock(hWnd, stageID);
}


#pragma endregion



#pragma region 初始化游戏场景函数

void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) 
		delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (currentStage->stageID == STAGE_STARTMENU)
	{
		nameflag = true;
		vector<int>().swap(namelist);
	}
	//初始化计时器
	currentStage->timeCountDown = 10000;
	currentStage->timerOn = true;

	//初始化BGM
	if (!silent)
	{
		switch (stageID)
		{
			case STAGE_ENDSTORY:
				break;
			case STAGE_ENDSTORY2:
			{
				mciSendString(TEXT("pause BGM"), NULL, 0, hWnd);
				break;
			}
			case STAGE_ENDSTORY3:
			{
				mciSendString(TEXT("resume END"), NULL, 0, hWnd);
				break;
			}
			case STAGE_STARTMENU:
			{
				mciSendString(TEXT("pause END"), NULL, 0, hWnd);
				mciSendString(TEXT("resume BGM"), NULL, 0, hWnd);
				break;
			}
			default:
				break;
		}
	}

	//初始化背景
	switch (stageID)
	{
		case STAGE_STARTMENU:
			currentStage->bg = bmp_TitleBg;
			break;
		case STAGE_STARTSTORY:
			currentStage->bg = bmp_StartBg;
			break;
		case STAGE_SELECT:
			currentStage->bg = bmp_Select;
				break;
		case STAGE_HELP_1:
			currentStage->bg = bmp_HELP_STAGE1;
			break;
		case STAGE_HELP_2:
			currentStage->bg = bmp_HELP_STAGE2;
			break;
		case STAGE_HELP_3:
			currentStage->bg = bmp_HELP_STAGE3;
			break;
		case STAGE_HELP_4:
			currentStage->bg = bmp_HELP_STAGE4;
			break;
		case STAGE_1:
			currentStage->bg = bmp_GameBg;
			break;
		case STAGE_2:
			currentStage->bg = bmp_GameBg2;
			break;
		case STAGE_3:
			currentStage->bg = bmp_GameBg3;
			break;
		case STAGE_4:
			currentStage->bg = bmp_GameBg4;
			break;
		case STAGE_5:
			currentStage->bg = bmp_GameBg5;
			break;
		case STAGE_ENDSTORY:
			currentStage->bg = bmp_EndBg;
			break;
		case STAGE_ENDSTORY2:
			currentStage->bg = bmp_EndBg2;
			break;
		case STAGE_ENDSTORY3:
			currentStage->bg = bmp_EndBg3;
			break;
		default:
			currentStage->bg = bmp_Background;
			break;
	}


	//显示游戏地图
	InitMap(hWnd, stageID);


	//显示游戏界面的按钮
	InitButton(hWnd, stageID);


	// 初始化主角
	if (theHero != NULL)
	{
		delete theHero;
		theHero = NULL;
	}
	switch (stageID)
	{
		case STAGE_1:
			theHero = CreateHero(bmp_Hero, 0, 400);
			break;
		case STAGE_STARTSTORY:
			break;
		case STAGE_SELECT:
			break;
		case STAGE_ENDSTORY:
			break;
		case STAGE_ENDSTORY2:
			break;
		case STAGE_ENDSTORY3:
			break;
		case STAGE_3:
			theHero = CreateHero(bmp_Hero, 0, 13*BLOCK_SIZE_Y);
			break;
		default:
			theHero = CreateHero(bmp_Hero, 0,530);
			break;
	}


	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion



#pragma region 主角状态函数


//陷阱检测
void TrapDetect(HWND hWnd)
{
	int herocenterX = theHero->x + HERO_SIZE_X / 2;
	int herocenterY = theHero->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;


	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i],*body=NULL;
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;
		if (block->visible)
			if (abs(herocenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
				&& abs(herocenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
				switch (block->blockID % 100)
				{
					case BLOCK_THORN://尖刺方块判定
					{
						body = CreateBlock(1000 * currentStage->stageID + BLOCK_STILLBODY, bmp_BloodBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						if (!silent)
							mciSendString(TEXT("play BLEED from 100 to 500"), NULL, 0, hWnd);
						return;
					}
		
					case BLOCK_MOVETHORN://移动尖刺判定
					{
						body = CreateBlock(1000 * currentStage->stageID + BLOCK_STICKBODY, bmp_BloodBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						if (!silent)
							mciSendString(TEXT("play BLEED from 100 to 500"), NULL, 0, hWnd);
						return;
					}

					case BLOCK_FIRE://火焰方块判定
					{
						body = CreateBlock(1000 * currentStage->stageID + BLOCK_BURNEDBODY, bmp_BurnedBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						return;
					}

					case BLOCK_ICE://冰焰方块判定
					{
						body = CreateBlock(1000 * currentStage->stageID + BLOCK_FREEZE, bmp_FreezedBody, HERO_SIZE_X, HERO_SIZE_Y, theHero->x, theHero->y);
						body->visible = true;
						blocks.push_back(body);
						delete theHero;
						theHero = NULL;
						if (!silent)
							mciSendString(TEXT("play ICE from 0 to 1000"), NULL, 0, hWnd);
						return;
					}

					case BLOCK_APPLE:		//终点判定
					{
						if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
							&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2)
						{
							switch (currentStage->stageID)
							{
								case STAGE_5:
								{
									mciSendString(TEXT("play FINISH from 0"), NULL, 0, hWnd);
									InitStage(hWnd, STAGE_ENDSTORY2);
									lucky = theName->frame;
									break;
								}
								case STAGE_HELP_4:
									InitStage(hWnd, STAGE_STARTSTORY);
									break;
								default:
									InitStage(hWnd, currentStage->stageID + 1);
									break;
							}
							return;
						}
					}
					default:
						break;
				}
	}
}



// 碰撞检测
bool CollitionDetect(HWND hWnd)
{
	bool onground = false; //是否落地
	int herocenterX = theHero->x + HERO_SIZE_X / 2;
	int herocenterY = theHero->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;
	bool flag = false;

	//方块检测
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;
		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_THORN:
					break;

				case BLOCK_FIRE:
					break;

				case BLOCK_BURNEDBODY:
					break;

				case BLOCK_APPLE:
					break;

				case BLOCK_SAVE://存档点判定
				{
					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						if (block->frame != 1)
						{
							mciSendString(TEXT("play SAVE from 0"), NULL, 0, hWnd);
							CurrentSave->img = bmp_BlockSave;
							CurrentSave->frame = 0;
							block->frame = 1;
							CurrentSave = block;
						}
					}
					break;
				}


				default://一般方块判定
				{

					if (abs(herocenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(herocenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - blockY)
							< block->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - blockX)) //判定此次碰撞为纵向碰撞
						{
							if (herocenterY < blockY) //判定从方块上方碰撞
							{
								while (theHero->y + HERO_SIZE_Y > block->y)theHero->y -= 1;
								theHero->vy = 0;
							}
							else //判定从方块下方碰撞
							{
								while (theHero->y < block->y + BLOCK_SIZE_Y)theHero->y += 1;
								theHero->vy = 0;
							}
						}
					}

					if (theHero->y + HERO_SIZE_Y == block->y
						&&abs(herocenterX - blockX) < (block->width + HERO_SIZE_X) / 2
						&& theHero->vy == 0) //判定主角落在方块上
					{
						onground = true;
						theHero->basevx = block->vx;
					}

					if (abs(herocenterX - blockX) < (block->width  + HERO_SIZE_X) / 2
						&& abs(herocenterY - blockY) <= (block->height  + HERO_SIZE_Y) / 2) //判定碰撞，边界条件改变
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - blockY) >
							block->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - blockX)) //判定此次碰撞为横向碰撞
						{
							if (herocenterX > blockX) //判定从方块右面碰撞
							{
								while (theHero->x < (block->x + BLOCK_SIZE_X))theHero->x += 1;
								theHero->vx = 0;
							}
							else { //判定从方块左面碰撞
								while ((theHero->x + HERO_SIZE_X) > block->x)theHero->x -= 1;
								theHero->vx = 0;
							}
							flag = true;
						}
					}
					break;
				}
			}
	}
	if (flag)
		return false;
	return onground;
}

//姓名栏刷新
void UpdateName()
{
	srand(time(NULL));
	int r = rand() % 889;
	theName->frame = r;
	namelist.push_back(r);
}

//主角状态刷新

void UpdateHero(HWND hWnd)
{
	//判断陷阱
	if (theHero != NULL)
		TrapDetect(hWnd);

	//主角重生
	if (theHero == NULL && currentStage->stageID == STAGE_STARTMENU)
		theHero = CreateHero(bmp_Hero, CurrentSave->x, CurrentSave->y);

	if (currentStage->stageID >= STAGE_1 &&
		currentStage->stageID <= STAGE_HELP_4)
	{

		if ((theHero == NULL) && keySpaceDown)
		{
			theHero = CreateHero(bmp_Hero, CurrentSave->x, CurrentSave->y);
			if (!silent)
				mciSendString(TEXT("play SAVE from 0"), NULL, 0, hWnd);
		}
	}

	if (theHero != NULL) {

		//碰撞检测，判定落地
		bool onground = CollitionDetect(hWnd);

		//移动判定
		//y方向速度
		if (currentStage->stageID == STAGE_STARTMENU)//主界面
		{
			if (onground)
				theHero->vy = -10.0;
			else if (!onground) //重力
				theHero->vy += GRAVITION;
		}
		else {
			if (keyUpDown && onground) //跳跃
			{
				if (!silent)
					mciSendString(TEXT("play JUMP from 0"), NULL, 0, hWnd);
				theHero->vy = -10.0;
			}
			else if (!onground) //重力
				theHero->vy += GRAVITION;
		}


		//x方向速度
		if (currentStage->stageID == STAGE_STARTMENU)
		{
			theHero->vx = HERO_MAXSPEED / 2;
		}
		else
		{
			if (keyLeftDown && !keyRightDown && theHero->vx > -1 * HERO_MAXSPEED)
				theHero->vx -= HERO_ACCELERATION;
			else if (!keyLeftDown && keyRightDown && theHero->vx < HERO_MAXSPEED)
				theHero->vx += HERO_ACCELERATION;
			else if (theHero->vx > 0)theHero->vx -= HERO_ACCELERATION;
			else if (theHero->vx < 0)theHero->vx += HERO_ACCELERATION;
		}

		//计算位移
		theHero->x += (int)(theHero->vx) + (int)(theHero->basevx);
		theHero->y += (int)(theHero->vy);



		//超出地图边界
		if (theHero->y > WINDOW_HEIGHT ||
			theHero->y + HERO_SIZE_Y < 0 ||
			theHero->x + HERO_SIZE_X<0 ||
			theHero->x>WINDOW_WIDTH)
		{
			delete theHero;
			theHero = NULL;
		}
	}
}


#pragma endregion



#pragma region 环境状态函数


//尸体陷阱检测
void BodyTrapDetect(HWND hwnd,Block*body)
{
	int bodycenterX = body->x + HERO_SIZE_X / 2;
	int bodycenterY = body->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;

	if (body->blockID % 100 == BLOCK_FREEZE)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			blockX = block->x + BLOCK_SIZE_X / 2;
			blockY = block->y + BLOCK_SIZE_Y / 2;
			if (block->visible)
				if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
					&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					switch (block->blockID % 100)
					{
						case BLOCK_FIRE://火焰方块判定
						{
							body->img = bmp_BloodBody;
							body->m ++;
							if (body->m == 25) 
							{
								body->blockID = currentStage->stageID * 1000 + BLOCK_MOVABLEBODY;
								body->m = 0;
							}
						}
						default:
							break;
					}
		}
	}

	if ((body->blockID % 100 == BLOCK_MOVABLEBODY)||
		(body->blockID % 100 == BLOCK_STICKBODY)||
		(body->blockID % 100 == BLOCK_STILLBODY))
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			blockX = block->x + BLOCK_SIZE_X / 2;
			blockY = block->y + BLOCK_SIZE_Y / 2;
			if (block->visible)
				if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
					&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					switch (block->blockID % 100)
					{
						case BLOCK_FIRE://火焰方块判定
						{
							body->blockID = 1000 * currentStage->stageID + BLOCK_BURNEDBODY;
							body->img = bmp_BurnedBody;
							body->n = 0;
							return;
						}
						default:
							break;
					}
		}
	}
}

//尸体碰撞检测
bool BodyCollitionDetect(HWND hwnd, Block*body)
{
	bool onground = false; //是否落地
	bool flag = false;
	int bodycenterX = body->x + HERO_SIZE_X / 2;
	int bodycenterY = body->y + HERO_SIZE_Y / 2;
	int blockX = 0, blockY = 0;

	//尸体与主角交互
	if (theHero != NULL)
	{
		int herocenterX = theHero->x + HERO_SIZE_X / 2;
		int herocenterY = theHero->y + HERO_SIZE_Y / 2;
		if (abs(herocenterX - bodycenterX) <= body->width / 2 + HERO_SIZE_X / 2
			&& abs(herocenterY - bodycenterY) <= body->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
		{
			if (abs(bodycenterX - herocenterX) < body->width / 2 + HERO_SIZE_X / 2
				&& abs(bodycenterY - blockY) < body->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
			{
				if (body->width / 2 + HERO_SIZE_Y / 2 - abs(bodycenterY - blockY)
					< body->height / 2 + HERO_SIZE_X / 2 - abs(bodycenterX - herocenterX)) //判定此次碰撞为纵向碰撞
				{
					if (bodycenterY < blockY) //判定从方块下方碰撞
					{
						while (body->y + HERO_SIZE_Y > theHero->y)body->y -= 1;
						body->vy = 0;
					}
				}
			}

			if (body->y + HERO_SIZE_Y == theHero->y
				&&abs(bodycenterX - herocenterX) < body->width / 2 + HERO_SIZE_X / 2) //判定尸体落在主角上
				onground = true;

			if (body->width / 2 + HERO_SIZE_Y / 2 - abs(herocenterY - bodycenterY) >
				body->height / 2 + HERO_SIZE_X / 2 - abs(herocenterX - bodycenterX))//判定此次碰撞为横向碰撞
			{
				if (herocenterX > bodycenterX) //判定从方块右面碰撞
				{
					while (theHero->x <= body->x + BLOCK_SIZE_X)body->x -= 1;
				}
				else { //判定从方块左面碰撞
					while (theHero->x + HERO_SIZE_X >= body->x)body->x += 1;
				}
				body->vx = theHero->vx;
			}
		}
	}

	//方块检测
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if ((block->x == body->x)
			&& (block->y == body->y))
			if (i == blocks.size())
				break;
			else
				continue;
			
		blockX = block->x + BLOCK_SIZE_X / 2;
		blockY = block->y + BLOCK_SIZE_Y / 2;



		if (block->visible)
			switch (block->blockID % 100)
			{
				case BLOCK_THORN:
					break;

				case BLOCK_FIRE:
					break;

				case BLOCK_BURNEDBODY:
					break;

				case BLOCK_SAVE:
					break;

				case BLOCK_ICE:
					break;

				case BLOCK_APPLE:
					break;


				default://一般方块判定
				{
					if (abs(bodycenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2
						&& abs(bodycenterY - blockY) < block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(bodycenterY - blockY)
							< block->height / 2 + HERO_SIZE_X / 2 - abs(bodycenterX - blockX)) //判定此次碰撞为纵向碰撞
						{
							if (bodycenterY < blockY) //判定从方块上方碰撞
							{
								while (body->y + HERO_SIZE_Y > block->y)body->y -= 1;
								body->vy = 0;
							}
						}
					}

					if (body->y + HERO_SIZE_Y == block->y
						&&abs(bodycenterX - blockX) < block->width / 2 + HERO_SIZE_X / 2) //判定尸体落在方块上
						onground = true;

					if (abs(bodycenterX - blockX) <= block->width / 2 + HERO_SIZE_X / 2
						&& abs(bodycenterY - blockY) <= block->height / 2 + HERO_SIZE_Y / 2) //判定碰撞，边界条件改变
					{
						if (block->width / 2 + HERO_SIZE_Y / 2 - abs(bodycenterY - blockY) >
							block->height / 2 + HERO_SIZE_X / 2 - abs(bodycenterX - blockX)) //判定此次碰撞为横向碰撞
						{
							if (bodycenterX > blockX) //判定方块右面碰撞尸体左面
							{
								while (body->x < block->x + BLOCK_SIZE_X)body->x += 1;
								if ((block->blockID % 100 == BLOCK_FREEZE)
									|| (block->blockID % 100 == BLOCK_MOVABLEBODY)
									|| (block->blockID % 100 == BLOCK_STICKBODY))
								{
									if (block->blockID % 100 == BLOCK_STICKBODY)
										body->vx = block->vx;
									else if (abs(body->vx) > abs(block->vx))
										body->vx = block->vx;
									else
										block->vx = body->vx;
								}
								else
									body->vx = block->vx;
							}
							else { //判定方块左面碰撞尸体右面
								while (body->x + HERO_SIZE_X > block->x)body->x -= 1;
								if ((block->blockID % 100 == BLOCK_FREEZE)
									|| (block->blockID % 100 == BLOCK_MOVABLEBODY)
									|| (block->blockID % 100 == BLOCK_STICKBODY))
								{
									if (block->blockID % 100 == BLOCK_STICKBODY)
										body->vx = block->vx;
									else if (abs(body->vx) > abs(block->vx))
										body->vx = block->vx;
									else
										block->vx = body->vx;
								}
								else
									body->vx = block->vx;
							}
						}
					}
				}
			}	
	}
	return onground;
}

//刷新尸体
void UpdateBody(HWND hWnd, Block*body)
{
	//判断陷阱
	if ((body->blockID % 100 == BLOCK_MOVABLEBODY)
		|| (body->blockID % 100 == BLOCK_FREEZE))
		BodyTrapDetect(hWnd, body);

	if ((body->blockID % 100 == BLOCK_MOVABLEBODY)
		|| (body->blockID % 100 == BLOCK_FREEZE))
	{

		bool onground = BodyCollitionDetect(hWnd,body);//碰撞检测

		//移动判定
		//y方向速度
		 if (!onground) //重力
			body->vy += GRAVITION;

		//x方向速度
		
		//计算位移
		body->x += (int)(body->vx);
		body->y += (int)(body->vy);


		//超出地图边界
		if (body->y > WINDOW_HEIGHT)
		{
			body->blockID = BLOCK_NORMAL;
			body->visible = false;
			body->vy = 0;
		}
	}
}


//踏板触发
void TriggerP(HWND hWnd, Block*pedal)
{
	pedal->img = bmp_BlockPedal;
	int pedalcenterX = pedal->x + BLOCK_SIZE_X / 2;
	int pedalcenterY = pedal->y + BLOCK_SIZE_Y / 2;

	if (theHero != NULL)
	{
		int herocenterX = theHero->x + HERO_SIZE_X / 2;
		int herocenterY = theHero->y + HERO_SIZE_Y / 2;
		if (abs(herocenterX - pedalcenterX) <= pedal->width / 2 + HERO_SIZE_X / 2
			&& abs(herocenterY - pedalcenterY) <= pedal->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
		{
			if (!silent&&pedal->m == 0)
			{
				mciSendString(TEXT("play ONOFF from 0"), NULL, 0, hWnd);
				pedal->m = 1;
			}
			pedal->img = bmp_BlockPedalon;
		}
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		if ((blocks[i]->x == pedal->x)
			&& (blocks[i]->y == pedal->y))
			if (i == blocks.size())
				break;
			else
				continue;
		if ((blocks[i]->blockID % 100 == BLOCK_MOVABLEBODY)
			|| (blocks[i]->blockID % 100 == BLOCK_FREEZE))
		{
			int blockX = blocks[i]->x + BLOCK_SIZE_X / 2;
			int blockY = blocks[i]->y + BLOCK_SIZE_Y / 2;
			if (abs(blockX - pedalcenterX) <= pedal->width / 2 + BLOCK_SIZE_X / 2
				&& abs(blockY - pedalcenterY) <= pedal->height / 2 + BLOCK_SIZE_Y / 2) //判定碰撞
			{
				if (!silent&&pedal->m == 0)
				{
					mciSendString(TEXT("play ONOFF from 0"), NULL, 0, hWnd);
					pedal->m = 1;
				}
				pedal->img = bmp_BlockPedalon;
			}
		}
	}

	if (pedal->img == bmp_BlockPedalon)
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if ((block->x == pedal->x)
				&& (block->y == pedal->y))
				if (i == blocks.size())
					break;
				else
					continue;
			if (block->blockID / 1000 == currentStage->stageID)
				if (block->link == pedal->link)
				{
					switch (block->blockID % 100)
					{
						case BLOCK_MOVETHORN:
						{
							block->turnon = (block->blockID % 1000) / 100;
							break;
						}
						case BLOCK_FIRE:
						{
							block->visible = (block->blockID % 1000) / 100;
							break;
						}
						case BLOCK_ICE:
						{
							block->visible = (block->blockID % 1000) / 100;
							break;
						}
					}
				}
		}
	else
	{
		pedal->m = 0;
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if ((block->x == pedal->x)
				&& (block->y == pedal->y))
				if (i == blocks.size())
					break;
				else
					continue;
			if (block->blockID / 1000 == currentStage->stageID)
				if (block->link == pedal->link)
				{
					switch (block->blockID % 100)
					{
						case BLOCK_MOVETHORN:
						{
							block->turnon = !((block->blockID % 1000) / 100);
							break;
						}
						case BLOCK_FIRE:
						{
							block->visible = !((block->blockID % 1000) / 100);
							break;
						}
						case BLOCK_ICE:
						{
							block->visible = !((block->blockID % 1000) / 100);
							break;
						}
					}
				}
		}
	}
	return;
}

//开关触发判定
void TriggerOnOff(HWND hWnd, Block*onoff)
{
	int onoffcenterX = onoff->x + BLOCK_SIZE_X / 2;
	int onoffcenterY = onoff->y + BLOCK_SIZE_Y / 2;

	if (theHero != NULL)
	{
		int herocenterX = theHero->x + HERO_SIZE_X / 2;
		int herocenterY = theHero->y + HERO_SIZE_Y / 2;
		if (abs(herocenterX - onoffcenterX) < onoff->width / 2 + HERO_SIZE_X / 2
			&& abs(herocenterY - onoffcenterY) < onoff->height / 2 + HERO_SIZE_Y / 2) //判定碰撞
		{
			if (!silent)
				mciSendString(TEXT("play ONOFF from 0"), NULL, 0, hWnd);
			if (onoff->img == bmp_BlockPedalon)
				onoff->img = bmp_BlockOnoff;
			else
				onoff->img = bmp_BlockPedalon;
		}
	}

	if (onoff->img == bmp_BlockPedalon)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if ((block->x == onoff->x)
				&& (block->y == onoff->y))
				if (i == blocks.size())
					break;
				else
					continue;
			if (block->blockID / 1000 == currentStage->stageID)
				if (block->link == onoff->link)
					switch (block->blockID % 100)
					{
						case BLOCK_MOVETHORN:
						{
							block->turnon = (block->blockID % 1000) / 100;
							break;
						}
						case BLOCK_FIRE:
						{
							block->visible = (block->blockID % 1000) / 100;
							break;
						}
						case BLOCK_ICE:
						{
							block->visible = (block->blockID % 1000) / 100;
							break;
						}
					}
		}
	}
	else
		for (int i = 0; i < blocks.size(); i++)
		{
			Block* block = blocks[i];
			if ((block->x == onoff->x)
				&& (block->y == onoff->y))
				if (i == blocks.size())
					break;
				else
					continue;
			if (block->blockID / 1000 == currentStage->stageID)
				if (block->link == onoff->link)
				{
					switch (block->blockID % 100)
					{
						case BLOCK_MOVETHORN:
						{
							block->turnon = !((block->blockID % 1000) / 100);
							break;
						}
						case BLOCK_FIRE:
						{
							block->visible = !((block->blockID % 1000) / 100);
							break;
						}
						case BLOCK_ICE:
						{
							block->visible = !((block->blockID % 1000) / 100);
							break;
						}
					}
				}
		}
	return;
}



//刷新环境
void UpdateSurround(HWND hWnd)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		Block*block = blocks[i];
		if (block->blockID / 1000 == currentStage->stageID)
			if (block->visible)
				switch (block->blockID % 100)
				{
					case BLOCK_BURNEDBODY://燃烧尸体
					{
						if (!block->n)
						{
							if(!silent)
								mciSendString(TEXT("play FIRE from 82"), NULL, 0, hWnd);
							block->n = 1;
						}
						block->m++;
						if (block->m == 5)
						{
							block->frame++;
							block->m = 0;
						}
						if (block->frame > 10)
							block->visible = false;
						break;
					}

					case BLOCK_FIRE://火焰动画
					{
						block->frame++;
						if (block->frame > 31)
							block->frame = 0;
						break;
					}

					case BLOCK_ICE://冰焰动画
					{
						block->frame++;
						if (block->frame > 31)
							block->frame = 0;
						break;
					}

					case BLOCK_MOVABLEBODY://可移动尸体
					{
						UpdateBody(hWnd, block);
						break;
					}
					case BLOCK_FREEZE://冰冻尸体
					{
						UpdateBody(hWnd, block);
						break;
					}

					case BLOCK_PEDAL://踏板
					{
						TriggerP(hWnd, block);
						break;
					}

					case BLOCK_ONOFF://开关
					{
						TriggerOnOff(hWnd, block);
						break;
					}

					case BLOCK_MOVETHORN://移动尖刺
					{
						if (block->turnon == true)
						{
							if (block->vx == 0)
							{
								block->vx = block->prevx;
							}
							if ((block->x >= block->n && block->vx >= 0)
								|| (block->x <= block->m && block->vx <= 0))
								block->vx = -block->vx;

							if ((block->y >= block->n && block->vy >= 0)
								|| (block->y <= block->m && block->vy <= 0))
								block->vy = -block->vy;

							//计算位移
							block->x += (int)(block->vx);
						}
						else if (block->vx != 0)
						{
							block->prevx = block->vx;
							block->vx = 0;
						}
						break;
					}

					case BLOCK_STILLBODY:
					{
						BodyTrapDetect(hWnd, block);
						break;
					}

					case BLOCK_STICKBODY://插在尖刺上的尸体
					{
						int min = 1638400;
						for (int i = 0; i < blocks.size(); i++)
						{
							Block* block2 = blocks[i];
							int dirX = block2->x - block->x;
							int dirY = block2->y - block->y;
							if (block2->visible &&
								(block2->blockID % 100 == BLOCK_MOVETHORN))
							{
								if (dirX*dirX + dirY * dirY < min)
								{
									block->vx = block2->vx;
									block->vy = block2->vy;
									min = dirX * dirX + dirY * dirY;
								}
							}
						}

						//计算位移
						block->x += (int)(block->vx);
						block->y += (int)(block->vy);

						BodyTrapDetect(hWnd, block);//陷阱检测

						break;
					}


					default:
						break;
				}
	}
}


#pragma endregion



#pragma region 绘图函数


// 绘图函数
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);



	//绘制方块到缓存
	for (int i = 0; i < blocks.size(); i++)
	{
		Block* block = blocks[i];
		if (block->visible)
		{
			SelectObject(hdc_loadBmp, block->img);
			TransparentBlt(
				hdc_memBuffer, block->x, block->y,
				block->width, block->height,
				hdc_loadBmp, 0, block->frame*BLOCK_SIZE_Y, block->width, block->height,
				RGB(255, 255, 255)
			);
		}
	}


	// 按场景分类绘制主角到缓存
	if (theHero != NULL)
		switch (currentStage->stageID) 
		{
			case STAGE_STARTSTORY:
				break;
			case STAGE_ENDSTORY:
				break;
			default:
				SelectObject(hdc_loadBmp, theHero->img);
				TransparentBlt(
					hdc_memBuffer, theHero->x, theHero->y,
					HERO_SIZE_X, HERO_SIZE_Y,
					hdc_loadBmp, 0, 0, HERO_SIZE_X, HERO_SIZE_Y,
					RGB(255, 255, 255)
				);
				break;
		}

	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}

	//绘制姓名栏到缓存
	if (theName->visible = true)
		switch (currentStage->stageID)
		{	
			case STAGE_STARTSTORY:
				break;
			case STAGE_SELECT:
				break;
			case STAGE_ENDSTORY:
				break;
			case STAGE_ENDSTORY2:
				break;
			case STAGE_ENDSTORY3:
				break;
		
			default:
			{
				SelectObject(hdc_loadBmp, bmp_Background);
				TransparentBlt(
					hdc_memBuffer, theName->x, theName->y,
					NAME_WIDTH, NAME_HEIGHT,
					hdc_loadBmp, theName->x, theName->y, NAME_WIDTH, NAME_HEIGHT,
					RGB(255, 255, 255));

				SelectObject(hdc_loadBmp, theName->img);
				TransparentBlt(
					hdc_memBuffer, theName->x, theName->y,
					NAME_WIDTH, NAME_HEIGHT,
					hdc_loadBmp, 0, theName->frame*NAME_HEIGHT, NAME_WIDTH, NAME_HEIGHT,
					RGB(255, 255, 255));

			}
		}
		
	int namex = 0, namey = 0, nameframe = 0;
	if (currentStage->stageID == STAGE_ENDSTORY)
	{
		if (nameflag) 
		{
			BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);
			Sleep(500);
			for (int i = 0; i < namelist.size(); i++)
			{
				if (i > 0)
					if (namelist[i] == namelist[i - 1] || namelist[i] == lucky)
						continue;
				if (namex > 5)
					continue;
				nameframe = namelist[i];
				SelectObject(hdc_loadBmp, theName->img);
				TransparentBlt(
					hdc_memBuffer, NAME_WIDTH*namex, 90+NAME_HEIGHT*namey,
					NAME_WIDTH, NAME_HEIGHT,
					hdc_loadBmp, 0, NAME_HEIGHT*nameframe, NAME_WIDTH, NAME_HEIGHT,
					RGB(255, 255, 255));
				namey++;
				if (namey == 15)
				{
					namex++;
					namey = 0;
				}
				BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);
				Sleep(100);
			}
			nameflag = false;
		}
		else
		{
			for (int i = 0; i < namelist.size(); i++)
			{
				if (i > 0 && i)
					if (namelist[i] == namelist[i - 1] || namelist[i] == lucky)
						continue;
				if (namex > 5)
					continue;
				nameframe = namelist[i];
				SelectObject(hdc_loadBmp, theName->img);
				TransparentBlt(
					hdc_memBuffer, NAME_WIDTH*namex, 90 + NAME_HEIGHT*namey,
					NAME_WIDTH, NAME_HEIGHT,
					hdc_loadBmp, 0, NAME_HEIGHT*nameframe, NAME_WIDTH, NAME_HEIGHT,
					RGB(255, 255, 255));
				namey++;
				if (namey == 15)
				{
					namex++;
					namey = 0;
				}
			}
		}
	}

	if (currentStage->stageID == STAGE_ENDSTORY2)
	{
		SelectObject(hdc_loadBmp, theName->img);
		TransparentBlt(
			hdc_memBuffer, 0, 600 - 5*NAME_HEIGHT,
			5 * NAME_WIDTH, 5 * NAME_HEIGHT,
			hdc_loadBmp, 0, NAME_HEIGHT*lucky, NAME_WIDTH, NAME_HEIGHT,
			RGB(255, 255, 255));
	}

	if (currentStage->stageID == STAGE_ENDSTORY3)
	{
		SelectObject(hdc_loadBmp, theName->img);
		TransparentBlt(
			hdc_memBuffer, 0,160,
			2 * NAME_WIDTH, 2 * NAME_HEIGHT,
			hdc_loadBmp, 0, NAME_HEIGHT*lucky, NAME_WIDTH, NAME_HEIGHT,
			RGB(255, 255, 255));

		SelectObject(hdc_loadBmp, theName->img);
		TransparentBlt(
			hdc_memBuffer, 330, 240,
			2 * NAME_WIDTH, 2 * NAME_HEIGHT,
			hdc_loadBmp, 0, NAME_HEIGHT*lucky, NAME_WIDTH, NAME_HEIGHT,
			RGB(255, 255, 255));
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}
#pragma endregion