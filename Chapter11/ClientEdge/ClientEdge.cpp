#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ClientEdge");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS wndClass;
	g_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
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

// 경계선 장식을 위한 확장 스타일 적용 예시
// 이중 WS_EX_CLIENTEDGE 스타일이 가장 볼만 함.(3차원 느낌이 나고, 다이얼로그의 컨트롤들이 이 스타일을 사용함)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE,
			10, 10, 90, 90, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE,
			110, 10, 90, 90, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindowEx(WS_EX_STATICEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE,
			210, 10, 90, 90, hWnd, (HMENU)2, g_hInst, NULL);
		CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE,
			310, 10, 90, 90, hWnd, (HMENU)3, g_hInst, NULL);
		CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 110, 90, 90, hWnd, (HMENU)4, g_hInst, NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			110, 110, 90, 90, hWnd, (HMENU)5, g_hInst, NULL);
		CreateWindowEx(WS_EX_STATICEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			210, 110, 90, 90, hWnd, (HMENU)6, g_hInst, NULL);
		CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			310, 110, 90, 90, hWnd, (HMENU)7, g_hInst, NULL);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}