#include <Windows.h>
#include "resource.h"
#define random(n) (rand() % n)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Couple2");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS wndClass;
	g_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&wndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

enum Status { HIDDEN, FLIP, TEMPFLIP };
struct tag_Cell
{
	int Num;
	Status St;
};

tag_Cell arCell[4][4];
int count;
HBITMAP hShape[9];
enum { RUN, HINT, VIEW } GameStatus;

void initGame();
void DrawScreen(HDC hdc);
void GetTempFlip(int* tx, int* ty);
int GetRemain();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int nx, ny;
	int i, j;
	int tx, ty;
	RECT crt;

	switch (iMessage)
	{
	case WM_CREATE:
		SetRect(&crt, 0, 0, 64 * 4 + 250, 64 * 4);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);
		hWndMain = hWnd;
		for (i = 0; i < sizeof(hShape) / sizeof(hShape[0]); ++i)
		{
			hShape[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}
		srand(GetTickCount());
		initGame();
		return 0;
	case WM_LBUTTONDOWN:
		nx = LOWORD(lParam) / 64;
		ny = HIWORD(lParam) / 64;
		if (GameStatus != RUN || nx > 3 || ny > 3 || arCell[nx][ny].St != HIDDEN)
		{
			return 0;
		}
		else
		{
			GetTempFlip(&tx, &ty);
			if (tx == -1)
			{
				arCell[nx][ny].St = TEMPFLIP;
				InvalidateRect(hWnd, NULL, FALSE);
			} 
			else
			{
				++count;
				if (arCell[tx][ty].Num == arCell[nx][ny].Num)
				{
					MessageBeep(0);
					arCell[tx][ty].St = FLIP;
					arCell[nx][ny].St = FLIP;
					InvalidateRect(hWnd, NULL, FALSE);
					if (GetRemain() == 0)
					{
						MessageBox(hWnd, "??????????. ???? ??????????.", "????", MB_OK);
						initGame();
					}
				}
				else
				{
					arCell[nx][ny].St = TEMPFLIP;
					InvalidateRect(hWnd, NULL, FALSE);
					GameStatus = VIEW;
					SetTimer(hWnd, 1, 1000, NULL);
				}
			}
		}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			KillTimer(hWnd, 0);
			GameStatus = RUN;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 1:
			KillTimer(hWnd, 1);
			GameStatus = RUN;
			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
				{
					if (arCell[i][j].St == TEMPFLIP)
					{
						arCell[i][j].St = HIDDEN;
					}
				}
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		default:
			break;
		}

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		for (i = 0; i < sizeof(hShape) / sizeof(hShape[0]); ++i)
		{
			DeleteObject(hShape[i]);
		}
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void initGame()
{
	int i, j;
	int x, y;
	count = 0;
	memset(arCell, 0, sizeof(arCell));
	for (i = 1; i <= 8; ++i)
	{
		for (j = 0; j < 2; ++j)
		{
			do
			{
				x = random(4);
				y = random(4);
			} while (arCell[x][y].Num != 0);
			arCell[x][y].Num = i;
		}
	}
	GameStatus = HINT;
	InvalidateRect(hWndMain, NULL, TRUE);

	SetTimer(hWndMain, 0, 2000, NULL);
	return;
}

void DrawScreen(HDC hdc)
{
	int x, y, image;
	TCHAR Mes[128];

	for (x = 0; x < 4; ++x)
	{
		for (y = 0; y < 4; ++y)
		{
			if (GameStatus == HINT || arCell[x][y].St != HIDDEN)
			{
				image = arCell[x][y].Num - 1;
			}
			else
			{
				image = 8;
			}
			DrawBitmap(hdc, x * 64, y * 64, hShape[image]);
		}
	}

	lstrcpy(Mes, "?? ?????? ???? Ver 1.2");
	TextOut(hdc, 300, 10, Mes, lstrlen(Mes));
	wsprintf(Mes, "?? ???? ????: %d", count);
	TextOut(hdc, 300, 50, Mes, lstrlen(Mes));
	wsprintf(Mes, "???? ?? ???? ?? : %d", GetRemain());
	TextOut(hdc, 300, 70, Mes, lstrlen(Mes));
	return;
}

void GetTempFlip(int* tx, int* ty)
{
	int i, j;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (arCell[i][j].St == TEMPFLIP)
			{
				*tx = i;
				*ty = j;
				return;
			}
		}
	}

	*tx = -1;
	return;
}

int GetRemain()
{
	int i, j;
	int remain = 16;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (arCell[i][j].St == FLIP)
			{
				--remain;
			}
		}
	}

	return remain;
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

	SelectObject(hdc, OldBitmap);
	DeleteDC(MemDC);
	return;
}
