#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

void OnIdle();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("FindIdle");

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
	BOOL AllowIdle = TRUE;
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
				AllowIdle = TRUE;
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
		else
		{
			// OnIdle()�� �޽����� ó���Ǵ� �߰��߰��� �ѹ��� ��� ȣ��Ǹ�
			// ���ʿ��ϰ� ���������� �� �� ȣ����� �ʴ´�.
			if (AllowIdle)
			{
				OnIdle();
				AllowIdle = FALSE;
			}
			// �� �Լ��� ť�� �޽����� ���� ������ ����ϵ� ���� �޽����� ������ �ٸ� ���μ������� ���� �ð��� �纸�Ѵ�.
			// GetMessage() �Լ��� �޽����� ������ ����� �����ϰ� 
			// �޽����� ���ö����� ������ ��ٸ��� ��ɸ��� ���� �Լ���� �����ϸ� �ȴ�.
			WaitMessage();
		}
	}

	return (int)Message.wParam;
}

void OnIdle()
{
	HDC hdc;
	hdc = GetDC(hWndMain);
	int x, y, r;
	HBRUSH hBrush, hOldBrush;

	x = rand() % 640;
	y = rand() % 480;
	r = rand() % 10 + 10;
	hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Ellipse(hdc, x, y, x + r, y + r);

	DeleteObject(SelectObject(hdc, hOldBrush));
	ReleaseDC(hWndMain, hdc);
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[128] = {};

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_MOUSEMOVE:
		wsprintf(str, TEXT("���콺 ��ġ = (%d, %d)"), LOWORD(lParam), HIWORD(lParam));
		SetWindowText(hWnd, str);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}