#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("DeferWindowPos");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HWND hBtn[5];
	int i;
	HDWP hdwp;

	switch (iMessage)
	{
	case WM_CREATE:
		for (i = 0; i < 5; ++i)
		{
			hBtn[i] = CreateWindow(TEXT("button"), TEXT("Test"),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				0, 0, 0, 0, hWnd, (HMENU)i, g_hInst, NULL);
				
		}
		return 0;
	case WM_LBUTTONDOWN:
		hdwp = BeginDeferWindowPos(5);
		for (i = 0; i < 5; ++i)
		{
			DeferWindowPos(hdwp, hBtn[i], NULL, i * 120, i * 50, 100, 20 + i * 8, SWP_NOZORDER);
		}
		EndDeferWindowPos(hdwp);
		return 0;
	case WM_RBUTTONDOWN:
		hdwp = BeginDeferWindowPos(5);
		for (i = 0; i < 5; ++i)
		{
			DeferWindowPos(hdwp, hBtn[i], NULL, 480 - i * 120, i * 50, 100, 52 - i * 8, SWP_NOZORDER);
		}
		EndDeferWindowPos(hdwp);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}