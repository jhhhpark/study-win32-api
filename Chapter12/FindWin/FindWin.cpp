#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("FindWin");

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
	HWND hFirst;
	static HWND h1, h2, h3;

	switch (iMessage)
	{
	case WM_CREATE:
		h1 = CreateWindow(TEXT("button"), TEXT("버튼"), WS_CHILD | WS_VISIBLE |
			BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);

		h2 = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			20, 50, 100, 50, hWnd, (HMENU)1, g_hInst, NULL);

		h3 = CreateWindow(TEXT("edit"), TEXT("에디트"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			20, 110, 100, 25, hWnd, (HMENU)2, g_hInst, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		hFirst = FindWindow(NULL, TEXT("First"));
		if (hFirst == NULL)
		{
			MessageBox(hWnd, TEXT("First 예제를 찾을 수 없습니다."), TEXT("알림"), MB_OK);
		}
		else
		{
			MessageBox(hWnd, TEXT("First 예제가 실행중입니다."), TEXT("알림"), MB_OK);
		}
		return 0;
	case WM_RBUTTONDOWN:
		hFirst = FindWindowEx(hWnd, NULL, NULL, TEXT("버튼"));
		if (hFirst == NULL)
		{
			MessageBox(hWnd, TEXT("차일드 컨트롤이 없습니다."), TEXT("알림"), MB_OK);
		}
		else
		{
			MessageBox(hWnd, TEXT("차일드 컨트롤을 찾았습니다."), TEXT("알림"), MB_OK);
		}

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}