#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RopMode");

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
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&wndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int sx, sy, oldx, oldy;
	static BOOL bNowDraw = FALSE;

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		break;
	case WM_LBUTTONDOWN:
	{
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		oldx = sx;
		oldy = sy;
		bNowDraw = TRUE;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (bNowDraw == TRUE)
		{
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT);
			MoveToEx(hdc, sx, sy, nullptr);
			LineTo(hdc, oldx, oldy);
			oldx = LOWORD(lParam);
			oldy = HIWORD(lParam);
			MoveToEx(hdc, sx, sy, nullptr);
			LineTo(hdc, oldx, oldy);
			ReleaseDC(hWnd, hdc);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		bNowDraw = FALSE;
		hdc = GetDC(hWnd);
		MoveToEx(hdc, sx, sy, nullptr);
		LineTo(hdc, oldx, oldy);
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}