#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MiniSpy");

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
	HDC hdc;
	PAINTSTRUCT ps;
	POINT pt;
	HWND hWndPoint, hWndParent;
	static HWND hWndOld;
	static TCHAR str[256];
	TCHAR sCaption[256], sClass[256];
	RECT crt;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 500, NULL);
		return 0;
	case WM_TIMER:
		GetCursorPos(&pt);
		hWndPoint = WindowFromPoint(pt);
		if (hWndPoint != hWndOld)
		{
			hWndOld = hWndPoint;
			if (hWndPoint == NULL)
			{
				lstrcpy(str, TEXT("윈도우 없음"));
			}
			else
			{
				GetWindowText(hWndPoint, sCaption, 256);
				GetClassName(hWndPoint, sClass, 256);
				hWndParent = GetParent(hWndPoint);
				GetWindowRect(hWndPoint, &crt);
				wsprintf(str, TEXT("핸들 : %d(0x%x)\r\n클래스 : %s\r\n캡션 : %s\r\n")
				TEXT("Parent : %d(0x%x)\r\n")
				TEXT("Rect : (%d, %d) - (%d, %d), %d * %d"),
					(int)hWndPoint, (int)hWndPoint, sClass, sCaption, (int)hWndParent, (int)hWndParent,
					crt.left, crt.top, crt.right, crt.bottom, 
					crt.right - crt.left, crt.bottom - crt.top);
			};
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		DrawText(hdc, str, lstrlen(str), &crt, 0);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}