#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("PeekMessage2");

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

	srand((unsigned int)time(NULL));
	for (;;)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int i;
	TCHAR str[256] = {};

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		/*
		* 200������ ���� ��� ���� CPU�� ���� �� ��� �ϸ� �Ѵ�.
		* �� �۾��� ����, ������, ������, ���� ���� � �����غ� �� �ִ�.
		* �� �ð����ȿ��� ��� �޽����� ó������ ���Ѵ�.
		* ������ WM_CHAR �޽����� �޾��� �� �Էµ� ���ڸ� �����ؼ� Ÿ��Ʋ�ٿ� ����ϴ� �ڵ尡
		* �ۼ��Ǿ� �ִµ� ���� ��� ���ȿ��� ó������ �ʴ´�.
		*/
		for (i = 0; i < 2000000; ++i)
		{
			SetPixel(hdc, rand() % 500, rand() % 400,
				RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		ReleaseDC(hWnd, hdc);
		return 0; 
	case WM_CHAR:
		wsprintf(str, TEXT("%c �����Է�"), wParam);
		SetWindowText(hWnd, str);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}