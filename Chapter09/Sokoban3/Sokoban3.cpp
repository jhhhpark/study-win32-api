#include <windows.h>
#include <mmsystem.h>
#include "resource.h"

#pragma comment(lib, "Winmm.lib")

#define MAXSTAGE 3
#define BW 32
#define BH 32
#define MAXUNDO 1000

void DrawScreen(HDC hdc);
BOOL TestEnd();
void Move(int dir);
void InitStage();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void ErasePack(int x, int y);
void Undo();
void Redo();

char ns[18][21];
int nStage;
int nx, ny;
int nMove;
HBITMAP hBit[8];
int ManBit;
struct tag_MoveInfo {
	char dx : 3;
	char dy : 3;
	char bWithPack : 2;
};
tag_MoveInfo MoveInfo[MAXUNDO];
int UndoIdx;
BOOL bSound = TRUE;

char arStage[MAXSTAGE][18][21] = {
	{
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####O  ############",
	"#####  O############",
	"###  O O ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# O  O   @      ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"####..  #     ######",
	"####..  # O  O  ####",
	"####..  #O####  ####",
	"####..    @ ##  ####",
	"####..  # #  O #####",
	"######### ##O O ####",
	"###### O  O O O ####",
	"######    #     ####",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
	{
	"####################",
	"####################",
	"####################",
	"####################",
	"##########     @####",
	"########## O#O #####",
	"########## O  O#####",
	"###########O O #####",
	"########## O # #####",
	"##....  ## O  O  ###",
	"###...    O  O   ###",
	"##....  ############",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
	},
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Sokoban3");

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
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	int i;
	TCHAR Message[256];

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, 900, BH * 18);
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
			SWP_NOMOVE | SWP_NOZORDER);
		for (i = 0; i < 8; i++) {
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WALL + i));
		}
		nStage = 0;
		InitStage();
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			Move(wParam);
			if (TestEnd()) {
				wsprintf(Message, TEXT("%d 스테이지를 풀었습니다. ")
					TEXT("다음 스테이지로 이동합니다"), nStage + 1);
				MessageBox(hWnd, Message, TEXT("알림"), MB_OK);
				if (nStage < MAXSTAGE - 1) {
					nStage++;
				}
				InitStage();
			}
			break;
		case 'Q':
			DestroyWindow(hWnd);
			break;
		case 'R':
			InitStage();
			break;
		case 'N':
			if (nStage < MAXSTAGE - 1) {
				nStage++;
				InitStage();
			}
			break;
		case 'P':
			if (nStage > 0) {
				nStage--;
				InitStage();
			}
			break;
		case 'Z':
			Undo();
			break;
		case 'Y':
			Redo();
			break;
		case 'S':
			bSound = !bSound;
			break;
		}
		return 0;
	case WM_DESTROY:
		for (i = 0; i < 8; i++) {
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawScreen(HDC hdc)
{
	int x, y;
	int iBit;
	TCHAR Message[256];

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			switch (ns[y][x]) {
			case '#':
				iBit = 0;
				break;
			case 'O':
				iBit = 1;
				break;
			case '.':
				iBit = 2;
				break;
			case ' ':
				iBit = 3;
				break;
			}
			DrawBitmap(hdc, x * BW, y * BH, hBit[iBit]);
		}
	}
	DrawBitmap(hdc, nx * BW, ny * BH, hBit[ManBit]);

	wsprintf(Message, TEXT("SOKOBAN"));
	TextOut(hdc, 700, 10, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Q:종료, R:다시 시작"));
	TextOut(hdc, 700, 30, Message, lstrlen(Message));
	wsprintf(Message, TEXT("N:다음, P:이전"));
	TextOut(hdc, 700, 50, Message, lstrlen(Message));
	wsprintf(Message, TEXT("Z:취소, Y:재실행"));
	TextOut(hdc, 700, 70, Message, lstrlen(Message));
	wsprintf(Message, TEXT("S:소리 토글"));
	TextOut(hdc, 700, 90, Message, lstrlen(Message));
	wsprintf(Message, TEXT("스테이지 : %d"), nStage + 1);
	TextOut(hdc, 700, 120, Message, lstrlen(Message));
	wsprintf(Message, TEXT("이동 회수 : %d"), nMove);
	TextOut(hdc, 700, 140, Message, lstrlen(Message));
}

BOOL TestEnd()
{
	int x, y;

	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (arStage[nStage][y][x] == '.' && ns[y][x] != 'O') {
				return FALSE;
			}
		}
	}
	return TRUE;
}

void Move(int dir)
{
	int dx = 0, dy = 0;
	BOOL bWithPack = FALSE;
	int i;

	switch (dir) {
	case VK_LEFT:
		ManBit = 4;
		dx = -1;
		break;
	case VK_RIGHT:
		ManBit = 5;
		dx = 1;
		break;
	case VK_UP:
		ManBit = 6;
		dy = -1;
		break;
	case VK_DOWN:
		ManBit = 7;
		dy = 1;
		break;
	}

	if (ns[ny + dy][nx + dx] != '#') {
		if (ns[ny + dy][nx + dx] == 'O') {
			if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') {
				ErasePack(nx + dx, ny + dy);
				ns[ny + dy * 2][nx + dx * 2] = 'O';
				bWithPack = TRUE;
			}
			else {
				return;
			}
		}
		nx += dx;
		ny += dy;
		nMove++;

		MoveInfo[UndoIdx].dx = dx;
		MoveInfo[UndoIdx].dy = dy;
		MoveInfo[UndoIdx].bWithPack = bWithPack;
		UndoIdx++;
		MoveInfo[UndoIdx].dx = -2;

		if (UndoIdx == MAXUNDO - 1) {
			for (i = 100; i < UndoIdx; i++) {
				MoveInfo[i - 100] = MoveInfo[i];
			}
			for (i = MAXUNDO - 100; i < MAXUNDO; i++) {
				MoveInfo[i].dx = -2;
			}
			UndoIdx -= 100;
		}
		if (bSound) {
			PlaySound(MAKEINTRESOURCE(bWithPack ? IDR_WITHPACK : IDR_MOVE),
				g_hInst, SND_RESOURCE | SND_ASYNC);
		}
		InvalidateRect(hWndMain, NULL, FALSE);
	}
}

void InitStage()
{
	int x, y;

	memcpy(ns, arStage[nStage], sizeof(ns));
	for (y = 0; y < 18; y++) {
		for (x = 0; x < 20; x++) {
			if (ns[y][x] == '@') {
				nx = x;
				ny = y;
				ns[y][x] = ' ';
			}
		}
	}
	nMove = 0;
	UndoIdx = 0;
	for (x = 0; x < MAXUNDO; x++) {
		MoveInfo[x].dx = -2;
	}
	ManBit = 4;
	InvalidateRect(hWndMain, NULL, TRUE);
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

void ErasePack(int x, int y)
{
	if (arStage[nStage][y][x] == '.') {
		ns[y][x] = '.';
	}
	else {
		ns[y][x] = ' ';
	}
}

void Undo()
{
	if (UndoIdx != 0) {
		UndoIdx--;
		if (MoveInfo[UndoIdx].bWithPack) {
			ErasePack(nx + MoveInfo[UndoIdx].dx, ny + MoveInfo[UndoIdx].dy);
			ns[ny][nx] = 'O';
		}
		nx -= MoveInfo[UndoIdx].dx;
		ny -= MoveInfo[UndoIdx].dy;
		InvalidateRect(hWndMain, NULL, FALSE);
	}
}

void Redo()
{
	if (MoveInfo[UndoIdx].dx != -2) {
		nx += MoveInfo[UndoIdx].dx;
		ny += MoveInfo[UndoIdx].dy;
		if (MoveInfo[UndoIdx].bWithPack) {
			ErasePack(nx, ny);
			ns[ny + MoveInfo[UndoIdx].dy][nx + MoveInfo[UndoIdx].dx] = 'O';
		}
		InvalidateRect(hWndMain, NULL, FALSE);
		UndoIdx++;
	}
}
