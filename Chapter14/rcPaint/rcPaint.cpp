#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ctime>
#include <cstdlib>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("rcPaint");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	srand((UINT)GetTickCount());
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

void Swap(int& a, int& b)
{
	int t;
	t = a; a = b; b = t;
	return;
}

COLORREF CalcColor()
{
	int r, g, b, i;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	for (i = 0; i < 10; ++i) Swap(r, g);
	return RGB(r, g, b);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	int x, y;

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		// �����츦 ����ȭ���� �񰡽ÿ������� �Ϻθ� ���ٰ� �ٽ� �����Ű��
		// ���̸� �и��� �� �� �ִ�.
		//for (y = 0; y < rt.bottom; ++y)
		//{
		//	for (x = 0; x < rt.right; ++x)
		//	{
		//		SetPixel(hdc, x, y, CalcColor());
		//	}
		//}
		for (y = ps.rcPaint.top; y < ps.rcPaint.bottom; ++y)
		{
			for (x = ps.rcPaint.left; x < ps.rcPaint.right; ++x)
			{
				SetPixel(hdc, x, y, CalcColor());
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