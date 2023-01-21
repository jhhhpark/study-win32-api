#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MinMax");

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
	LPMINMAXINFO pmmi;

	RECT wrt;
	int Width, Height;

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
		// 이렇게 하면 안된다.
	case WM_SIZE:
		GetWindowRect(hWnd, &wrt);
		Width = wrt.right - wrt.left;
		Height = wrt.bottom - wrt.top;
		Width = min(max(Width, 300), 600);
		Height = min(max(Height, 200), 400);
		SetWindowPos(hWnd, 0, 0, 0, Width, Height, SWP_NOMOVE);
		return 0;
	//case WM_GETMINMAXINFO:
	//	pmmi = (LPMINMAXINFO)lParam;
	//	// 경계선 드래그 시 최대, 최소 사이즈 제한
	//	pmmi->ptMinTrackSize.x = 300;
	//	pmmi->ptMinTrackSize.y = 200;
	//	pmmi->ptMaxTrackSize.x = 600;
	//	pmmi->ptMaxTrackSize.y = 400;

	//	// 최대화 시 크기, 위치 지정
	//	pmmi->ptMaxSize.x = 500;
	//	pmmi->ptMaxSize.y = 300;
	//	pmmi->ptMaxPosition.x = 100;
	//	pmmi->ptMaxPosition.y = 100;
	//	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}