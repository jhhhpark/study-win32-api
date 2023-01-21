#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WmdRgn");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_POPUP | WS_SYSMENU,
		100, 100, 200, 200,
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
	HDC hdc;
	PAINTSTRUCT ps;
	UINT nHit;
	static HRGN hWndRgn;

	switch (iMessage)
	{
	case WM_CREATE:
		hWndRgn = CreateEllipticRgn(0, 0, 200, 200);
		SetWindowRgn(hWnd, hWndRgn, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Ellipse(hdc, 0, 0, 199, 199);
		Rectangle(hdc, 40, 50, 160, 150);
		TextOut(hdc, 45, 90, TEXT("스페이스-종료"), 13);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_NCHITTEST:
		nHit = DefWindowProc(hWnd, iMessage, wParam, lParam);
		if (nHit == HTCLIENT)
			nHit = HTCAPTION;
		return nHit;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			DestroyWindow(hWnd);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}