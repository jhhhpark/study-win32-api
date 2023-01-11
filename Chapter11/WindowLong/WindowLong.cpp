#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WindowLong");

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

// WM_NCPAINT�� �������� �� �۾������� �ٽ� �׸���� �޽����̴�.
// Get(Set)WindowLongPtr() �Լ��� �������� ��Ż�̳� Ȯ�� ��Ÿ���� �����߿� ����, ������ �� ����Ѵ�.
// ��, WS_EX_TOPMOST Ȯ�� ��Ÿ���� �� �Լ��δ� ���� �� �� ������, SetWindowPos() �Լ��� ����ؾ� �Ѵ�.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = TEXT("���콺 �¿� ��ư���� �ִ�ȭ ��ư ���");
	LONG Style;
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		Style = GetWindowLongPtr(hWnd, GWL_STYLE);
		Style |= WS_MAXIMIZEBOX;
		SetWindowLongPtr(hWnd, GWL_STYLE, Style);
		SendMessage(hWnd, WM_NCPAINT, 1, 0);
		return 0;
	case WM_RBUTTONDOWN:
		Style = GetWindowLongPtr(hWnd, GWL_STYLE);
		Style &= ~WS_MAXIMIZEBOX;
		SetWindowLongPtr(hWnd, GWL_STYLE, Style);
		SendMessage(hWnd, WM_NCPAINT, 1, 0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}