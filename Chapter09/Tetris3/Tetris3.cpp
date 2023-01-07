#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Tetris3");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = 0;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&Message, NULL, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

// 함수 원형
void DrawScreen(HDC hdc);
void MakeNewBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void PrintTile(HDC hdc, int x, int y, int c);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);

// 매크로 및 전역 변수들
#define BW 10
#define BH 20
#define random(n) (rand()%n)
#define TS 24
struct Point {
	int x, y;
};
Point Shape[][4][4] = {
	{ {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },
	{ {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },
	{ {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },
	{ {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },
	{ {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },
	{ {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },
	{ {0,0,-1,0,1,0,0,1}, {0,0,0,-1,0,1,1,0}, {0,0,-1,0,1,0,0,-1}, {0,0,-1,0,0,-1,0,1} },
	{ {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0} },
	{ {0,0,0,0,0,-1,1,0},{0,0,0,0,-1,0,0,-1},{0,0,0,0,0,1,-1,0},{0,0,0,0,0,1,1,0} },
};
enum { EMPTY, BRICK, WALL = sizeof(Shape) / sizeof(Shape[0]) + 1 };
int board[BW + 2][BH + 2];
int nx, ny;
int brick, rot;
int nbrick;
int score;
int bricknum;
enum tag_Status { GAMEOVER, RUNNING, PAUSE };
tag_Status GameStatus;
int Interval;
HBITMAP hBit[11];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int i;
	RECT crt;
	int trot;
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, (BW + 12) * TS, (BH + 2) * TS);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
		SetWindowPos(hWndMain, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);

		GameStatus = GAMEOVER;
		srand(GetTickCount());
		for (i = 0; i < 11; i++) {
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_GAME_START:
			if (GameStatus != GAMEOVER) {
				break;
			}
			for (x = 0; x < BW + 2; x++) {
				for (y = 0; y < BH + 2; y++) {
					board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
				}
			}
			score = 0;
			bricknum = 0;
			GameStatus = RUNNING;
			nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
			MakeNewBrick();
			Interval = 1000;
			SetTimer(hWnd, 1, Interval, NULL);
			break;
		case IDM_GAME_PAUSE:
			if (GameStatus == RUNNING) {
				GameStatus = PAUSE;
				KillTimer(hWnd, 1);
			}
			else if (GameStatus == PAUSE) {
				GameStatus = RUNNING;
				SetTimer(hWnd, 1, Interval, NULL);
			}
			break;
		case IDM_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_TIMER:
		if (MoveDown() == TRUE) {
			MakeNewBrick();
		}
		return 0;
	case WM_KEYDOWN:
		if (GameStatus != RUNNING || brick == -1)
			return 0;
		switch (wParam) {
		case VK_LEFT:
			if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
				nx--;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_RIGHT:
			if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
				nx++;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_UP:
			trot = (rot + 1) % (sizeof(Shape[0]) / sizeof(Shape[0][0]));
			if (GetAround(nx, ny, brick, trot) == EMPTY) {
				rot = trot;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case VK_DOWN:
			if (MoveDown() == TRUE) {
				MakeNewBrick();
			}
			break;
		case VK_SPACE:
			while (MoveDown() == FALSE) { ; }
			MakeNewBrick();
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWndMain, 1);
		for (i = 0; i < 11; i++) {
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawScreen(HDC hdc)
{
	int x, y, i;
	TCHAR str[128];

	// 테두리 그림
	for (x = 1; x < BW + 1; x++) {
		PrintTile(hdc, x, 0, WALL);
		PrintTile(hdc, x, BH + 1, WALL);
	}
	for (y = 0; y < BH + 2; y++) {
		PrintTile(hdc, 0, y, WALL);
		PrintTile(hdc, BW + 1, y, WALL);
	}

	// 게임판 그림
	for (x = 1; x < BW + 1; x++) {
		for (y = 1; y < BH + 1; y++) {
			PrintTile(hdc, x, y, board[x][y]);
		}
	}

	// 이동중인 벽돌 그림
	if (GameStatus != GAMEOVER && brick != -1) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, nx + Shape[brick][rot][i].x, ny + Shape[brick][rot][i].y, brick + 1);
		}
	}

	// 다음 벽돌 그림
	for (x = BW + 3; x <= BW + 11; x++) {
		for (y = BH - 5; y <= BH + 1; y++) {
			if (x == BW + 3 || x == BW + 11 || y == BH - 5 || y == BH + 1) {
				PrintTile(hdc, x, y, WALL);
			}
			else {
				PrintTile(hdc, x, y, 0);
			}
		}
	}
	if (GameStatus != GAMEOVER) {
		for (i = 0; i < 4; i++) {
			PrintTile(hdc, BW + 7 + Shape[nbrick][0][i].x, BH - 2 + Shape[nbrick][0][i].y, nbrick + 1);
		}
	}

	// 정보 출력
	lstrcpy(str, TEXT("Tetris Ver 1.2"));
	TextOut(hdc, (BW + 4) * TS, 30, str, lstrlen(str));
	wsprintf(str, TEXT("점수 : %d   "), score);
	TextOut(hdc, (BW + 4) * TS, 60, str, lstrlen(str));
	wsprintf(str, TEXT("벽돌 : %d 개   "), bricknum);
	TextOut(hdc, (BW + 4) * TS, 80, str, lstrlen(str));
}

void MakeNewBrick()
{
	bricknum++;
	brick = nbrick;
	nbrick = random(sizeof(Shape) / sizeof(Shape[0]));
	nx = BW / 2;
	ny = 3;
	rot = 0;
	InvalidateRect(hWndMain, NULL, FALSE);
	if (GetAround(nx, ny, brick, rot) != EMPTY) {
		KillTimer(hWndMain, 1);
		GameStatus = GAMEOVER;
		MessageBox(hWndMain, TEXT("게임이 끝났습니다. 다시 시작하려면 게임/시작")
			TEXT(" 항목(S)을 선택해 주십시오."), TEXT("알림"), MB_OK);
	}
}

int GetAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	ny++;
	// 즉시 그려서 벽돌이 내려가는 모양을 보여 준다.
	InvalidateRect(hWndMain, NULL, FALSE);
	UpdateWindow(hWndMain);
	return FALSE;
}

void TestFull()
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = { 0,1,3,8,20 };

	for (i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brick + 1;
	}
	// 이동중인 벽돌이 잠시 없는 상태. 
	brick = -1;

	for (y = 1; y < BH + 1; y++) {
		for (x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) break;
		}
		if (x == BW + 1) {
			count++;
			for (ty = y; ty > 1; ty--) {
				for (x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			InvalidateRect(hWndMain, NULL, FALSE);
			UpdateWindow(hWndMain);
			Sleep(150);
		}
	}
	score += arScoreInc[count];
	if (bricknum % 10 == 0 && Interval > 200) {
		Interval -= 50;
		SetTimer(hWndMain, 1, Interval, NULL);
	}
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void PrintTile(HDC hdc, int x, int y, int c)
{
	DrawBitmap(hdc, x * TS, y * TS, hBit[c]);
	return;
}
