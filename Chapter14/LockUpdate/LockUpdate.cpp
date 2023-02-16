#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("LockUpdate");

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

struct tagLines
{
	int x, y;
	BOOL Move;
} Line[10000];

int index = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	static int x, y;
	static BOOL bNowDraw = FALSE;
	switch (iMessage)
	{
	case WM_CREATE:
		SetWindowText(hWnd, TEXT("LŰ:�׸��� ����, UŰ: �׸��� �㰡"));
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'L')
		{
			SetWindowText(hWnd, TEXT("�׸��� ����"));
			LockWindowUpdate(hWnd);
		}
		else if (wParam == 'U')
		{
			SetWindowText(hWnd, TEXT("�׸��� �㰡"));
			LockWindowUpdate(NULL);
		}
		return 0;
	case WM_LBUTTONDOWN:
		bNowDraw = TRUE;
		Line[index].x = (int)LOWORD(lParam);
		Line[index].y = (int)HIWORD(lParam);
		Line[index].Move = TRUE;
		index++;
		return 0;
	case WM_RBUTTONDOWN:
		index = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_MOUSEMOVE:
		if (bNowDraw == TRUE)
		{
			hdc = GetDC(hWnd);
			MoveToEx(hdc, Line[index - 1].x, Line[index - 1].y, NULL);
			Line[index].x = LOWORD(lParam);
			Line[index].y = HIWORD(lParam);
			Line[index].Move = FALSE;
			LineTo(hdc, Line[index].x, Line[index].y);
			index++;
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw = FALSE;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (i = 0; i < index; ++i)
		{
			if (Line[i].Move == TRUE)
			{
				MoveToEx(hdc, Line[i].x, Line[i].y, NULL);
			}
			else
			{
				LineTo(hdc, Line[i].x, Line[i].y);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}