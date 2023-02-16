#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WmPaint");

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

BOOL bRect = TRUE;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	int i;
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		MessageBeep(0);
		hdc = BeginPaint(hWnd, &ps);
		for (i = 0; i < 1024; i += 5)
		{
			MoveToEx(hdc, 0, i, NULL);
			LineTo(hdc, 1280, i); 
		}

		for (i = 0; i < 1280; i += 5)
		{
			MoveToEx(hdc, i, 0, NULL);
			LineTo(hdc, i, 1024);
		}

		if (bRect)
		{
			Rectangle(hdc, 10, 10, 200, 200);
		}
		else
		{
			Ellipse(hdc, 10, 10, 200, 200);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		bRect = !bRect;
		SetRect(&rt, 10, 10, 200, 200);
		// 그릴 필요가 없는 영역이기에 WM_PAINT는 전달되지 않음
		//SetRect(&rt, -100, -100, -10, -10);
		//InvalidateRect(hWnd, &rt, TRUE);
		// 무효화 영역이 지정되지 않으면 다시 그리지 않음
		SendMessage(hWnd, WM_PAINT, 0, 0); 
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}