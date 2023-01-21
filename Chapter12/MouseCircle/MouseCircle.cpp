#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MouseCircle");

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

HWND hBtn;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT brt;
	
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		hBtn = CreateWindow(TEXT("button"), TEXT("테스트"), WS_CHILD | WS_VISIBLE |
			BS_PUSHBUTTON, 168, 125, 96, 42, hWnd, (HMENU)0, g_hInst, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetWindowRect(hBtn, &brt);
		InflateRect(&brt, 50, 50);
		// 이렇게 두번 호출된 이유가 두번째 인수의 타입이 LPPOINT임 그래서 좌상, 우하로 나누어서 LPPOINT로 캐스팅
		ScreenToClient(hWnd, (LPPOINT)&brt);
		ScreenToClient(hWnd, (LPPOINT)&brt.right);
		Rectangle(hdc, brt.left, brt.top, brt.right, brt.bottom);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}