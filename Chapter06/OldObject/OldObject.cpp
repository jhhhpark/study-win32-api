#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("OldObject");

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

void TestResource(BOOL bUnSel, BOOL bDelete, int num)
{
	HDC hdc;
	HPEN hPen, hOldPen;
	int i;
	TCHAR str[256];

	hdc = GetDC(hWndMain);
	for (i = 0; i < num; ++i)
	{
		hPen = CreatePen(PS_SOLID, rand() % 5 + 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, rand() % 300, rand() % 200 + 40, NULL);
		LineTo(hdc, rand() % 300, rand() % 200 + 40);
		if (bUnSel == TRUE) SelectObject(hdc, hOldPen);
		if (bDelete == TRUE) DeleteObject(hPen);
		wsprintf(str, TEXT("%d"), i);
		SetWindowText(hWndMain, str);
	}
	ReleaseDC(hWndMain, hdc);
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	const TCHAR* Mes = TEXT("1. 정상, 2. 해제 x, 삭제 o, 3. 해제 x, 삭제 x");
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
			TestResource(TRUE, TRUE, 10000);
			break;
		case '2':
			TestResource(FALSE, TRUE, 10000);
			break;
		case '3':
			TestResource(FALSE, FALSE, 10000);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}