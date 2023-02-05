#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MoveSharp");

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

#define ACC 5
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x = 100, y = 100;
	static int delta = 1;
	BOOL bPress;
	RECT rt = {};
	switch (iMessage)
	{
	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			SetTimer(hWnd, 1, 50, NULL);
		}
		else
		{
			KillTimer(hWnd, 1);
		}
		return 0;
	case WM_TIMER:
		bPress = FALSE;
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			x -= (delta / ACC);
			bPress = TRUE;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			x += (delta / ACC);
			bPress = TRUE;
		}
		if (GetKeyState(VK_UP) & 0x8000)
		{
			y -= (delta / ACC);
			bPress = TRUE;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			y += (delta / ACC);
			InvalidateRect(hWnd, &rt, TRUE);
		}

		if (bPress)
		{
			delta = min(delta + 1, ACC * 10);
			SetRect(&rt, x - 20, y - 40, x + 20, y + 40);
			InvalidateRect(hWnd, &rt, TRUE);
		}
		else
		{
			delta = 1;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, TEXT("#"), 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}