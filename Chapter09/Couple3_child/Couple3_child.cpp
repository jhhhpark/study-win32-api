#include <Windows.h>
#include "resource.h"
#define random(n) (rand() % n)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Couple3_Child");

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

	// ���ϵ� ������ Ŭ������ ���� ����Ʈ���� ����, 0-4����Ʈ : ����(ī�� ��), 4-8����Ʈ : ����(ī�� ����)
	wndClass.cbWndExtra = 8;
	wndClass.hbrBackground = NULL;
	wndClass.lpfnWndProc = ChildProc;
	wndClass.lpszClassName = "CoupleChild";
	RegisterClass(&wndClass);

	// WS_CLIPCHILDREN : �θ� WINDOW���� DRAW�� �� �� �ڽ� WINDOW�κ��� ���ܽ�Ų��.
	hWnd = CreateWindow(lpszClass, lpszClass, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |
		WS_BORDER | WS_CLIPCHILDREN, 
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

enum Status { HIDDEN, FLIP, TEMPFLIP };
enum { RUN, HINT, VIEW } GameStatus;
int count;
HBITMAP hShape[9];
HWND hChild[16];
// Child ������ Ŭ���� ��� �� �߰��ߴ� ���� ����Ʈ�� 
// ���� �����͸� ���ų� ���� �� ��ġ�� ����
// �� ���α׷������� 0~4 : ī�� ����, 4~8 : ī�� ����
#define EB_NUM		0
#define EB_STATUS	4

void InitGame();
HWND GetTempFlip();
int GetRemain();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	RECT crt;
	TCHAR Mes[128];

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
		// ���ϵ� ������ ����
		for (i = 0; i < 16; ++i)
		{
			hChild[i] = CreateWindow("CoupleChild", NULL, WS_CHILD | WS_VISIBLE,
				(i / 4) * 64, (i % 4) * 64, 64, 64, hWnd, (HMENU)i, g_hInst, NULL);
		}
		srand(GetTickCount());
		InitGame();
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			KillTimer(hWnd, 0);
			GameStatus = RUN;
			for (i = 0; i < 16; ++i)
			{
				InvalidateRect(hChild[i], NULL, FALSE);
			}
			break;
		case 1:
			KillTimer(hWnd, 1);
			GameStatus = RUN;
			for (i = 0; i < 16; ++i)
			{
				if (GetWindowLong(hChild[i], EB_STATUS) == TEMPFLIP)
				{
					SetWindowLong(hChild[i], EB_STATUS, HIDDEN);
					InvalidateRect(hChild[i], NULL, FALSE);
				}
			}
			break;
		default:
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		lstrcpy(Mes, "¦ ���߱� ���� Ver 1.2");
		TextOut(hdc, 300, 10, Mes, lstrlen(Mes));
		wsprintf(Mes, "�� �õ� ȸ�� : %d", count);
		TextOut(hdc, 300, 50, Mes, lstrlen(Mes));
		wsprintf(Mes, "���� �� ã�� �� : %d   ", GetRemain());
		TextOut(hdc, 300, 70, Mes, lstrlen(Mes));
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

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hPartner;
	int image;

	switch (iMessage)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, EB_NUM, 0);
		SetWindowLong(hWnd, EB_STATUS, (LONG)HIDDEN);
		return 0;
	case WM_LBUTTONDOWN:
		if (GameStatus != RUN || GetWindowLong(hWnd, EB_STATUS) != HIDDEN)
		{
			return 0;
		}
		else
		{
			// Ŭ�� �� ���ǿ� ������� �ٽ� �׸���.
			InvalidateRect(hWnd, NULL, FALSE);
			InvalidateRect(hWndMain, NULL, FALSE);

			hPartner = GetTempFlip();
			if (hPartner == NULL)
			{
				SetWindowLong(hWnd, EB_STATUS, TEMPFLIP);
			}
			else
			{
				count++;
				// ������ �� ī�尡 �������� üũ
				if (GetWindowLong(hWnd, EB_NUM) == GetWindowLong(hPartner, EB_NUM))
				{
					// ����.
					SetWindowLong(hWnd, EB_STATUS, FLIP);
					SetWindowLong(hPartner, EB_STATUS, FLIP);
					if (GetRemain() == 0)
					{
						MessageBox(hWndMain, "�����մϴ�. �ٽ� �����մϴ�.", "�˸�", MB_OK);
						InitGame();
					}
				}
				else
				{
					// �ٸ���.
					SetWindowLong(hWnd, EB_STATUS, TEMPFLIP);
					GameStatus = VIEW;
					SetTimer(hWndMain, 1, 1000, NULL);
				}
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (GameStatus == HINT || GetWindowLong(hWnd, EB_STATUS) != HIDDEN)
		{
			image = GetWindowLong(hWnd, EB_NUM) - 1;
		}
		else
		{
			image = 8;
		}
		DrawBitmap(hdc, 0, 0, hShape[image]);
		EndPaint(hWnd, &ps);
		break;
	default:
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void InitGame()
{
	int i, j;
	int c;
	count = 0;

	// ���ϵ� ������ �ʱ�ȭ
	for (i = 0; i < 16; ++i)
	{
		SetWindowLong(hChild[i], EB_STATUS, HIDDEN);
		SetWindowLong(hChild[i], EB_NUM, 0);
	}

	// ���ϵ忡 ���� ����
	for (i = 1; i <= 8; ++i)
	{
		for (j = 0; j < 2; ++j)
		{
			do
			{
				c = random(16);
			} while (GetWindowLong(hChild[c], EB_NUM) != 0);
			SetWindowLong(hChild[c], EB_NUM, i);
		}
	}

	// �ʱ⿡ 2�ʰ� ��ü ī�� ����
	GameStatus = HINT;
	for (i = 0; i < 16; ++i)
	{
		InvalidateRect(hChild[i], NULL, FALSE);
	}
	InvalidateRect(hWndMain, NULL, TRUE);
	SetTimer(hWndMain, 0, 2000, NULL);
	return;
}

HWND GetTempFlip()
{
	int i;
	for (i = 0; i < 16; ++i)
	{
		if (GetWindowLong(hChild[i], EB_STATUS) == TEMPFLIP)
		{
			return hChild[i];
		}
	}
	return NULL;
}

int GetRemain()
{
	int i;
	int remain = 16;
	for (i = 0; i < 16; ++i)
	{
		if (GetWindowLong(hChild[i], EB_STATUS) == FLIP)
		{
			--remain;
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
