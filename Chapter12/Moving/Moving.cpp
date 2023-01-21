#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Moving");

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

//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	CONST TCHAR* Mes = TEXT("16�ȼ� �����θ� �����Դϴ�.");
//
//	switch (iMessage)
//	{
//	case WM_WINDOWPOSCHANGING:
//		/*
//		* �� ������� ���� 4��Ʈ�� ������ 0���� ���巷 ������. �׷��� ��ǥ�� 0, 15, 32, 48, 64 ��� ����
//		* 16�� ����θ� �����Ǹ� �����찡 16�ȼ� ���� ���ĵȴ�.
//		* ũ�⵵ ���������� 16�ȼ� �����θ� ���/Ȯ��� ���̴�.
//		* lParam���� ���޵Ǵ� WINDOWPOS ����ü�� ���� �̵����� �������� ������ �˷��ֱ⵵ ������
//		* �� ���� �����ϸ� �� ����� �� ��ġ���� �ݿ��ǹǷ� ����¿� �μ���� �� �� �ִ�.
//		*/
//		((LPWINDOWPOS)lParam)->x &= 0xfffffff0;
//		((LPWINDOWPOS)lParam)->y &= 0xfffffff0;
//		((LPWINDOWPOS)lParam)->cx &= 0xfffffff0;
//		((LPWINDOWPOS)lParam)->cy &= 0xfffffff0;
//		return 0;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		TextOut(hdc, 100, 100, Mes, lstrlen(Mes));
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
//}

// ��ũ�� �����ڸ� ��ó�� ���� �����ڸ��� �� �޶�ٴ� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx, sy;
	int t;
	LPWINDOWPOS wp;
	CONST TCHAR* Mes = TEXT("ȭ�� �����ڸ��� �����մϴ�.");

	switch (iMessage)
	{
	case WM_CREATE:
		sx = GetSystemMetrics(SM_CXSCREEN);
		sy = GetSystemMetrics(SM_CYSCREEN);
		return 0;
	case WM_WINDOWPOSCHANGING:
		wp = (LPWINDOWPOS)lParam;
		t = wp->x;
		if (t < 30 && t > -30) t = -10;
		if (t + wp->cx > sx - 30 && t + wp->cx < sx + 30)
			t = sx - wp->cx;
		wp->x = t;

		t = wp->y; 
		if (t < 30 && t > -30) t = 0;
		if (t + wp->cy > sy - 60 && t + wp->cy < sy + 60)
			t = sy - wp->cy - 30;
		wp->y = t;

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}