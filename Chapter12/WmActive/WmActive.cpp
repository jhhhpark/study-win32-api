#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PopupProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WmActive");

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
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndClass);

	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndClass.lpfnWndProc = PopupProc;
	wndClass.lpszClassName = "PopupClass";
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
	PAINTSTRUCT ps;
	static TCHAR Mes[128] = {};
	static HWND hPopup;

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		hPopup = CreateWindow(TEXT("PopupClass"), TEXT("팝업 윈도우"),
			WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
			410, 100, 300, 300,
			hWnd, (HMENU)0, g_hInst, NULL);
		return 0;
	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			ShowWindow(hPopup, SW_SHOWNOACTIVATE);
		}
		else
		{
			ShowWindow(hPopup, SW_HIDE);
		}
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			lstrcpy(Mes, TEXT("비활성화 상태입니다."));
		}
		else
		{
			lstrcpy(Mes, TEXT("저는 지금 활성화되어 있습니다."));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}


LRESULT CALLBACK PopupProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR Mes[128] = {};

	switch (iMessage)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			lstrcpy(Mes, TEXT("비활성화 상태입니다."));
		}
		else
		{
			lstrcpy(Mes, TEXT("저는 지금 활성화되어 있습니다."));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}