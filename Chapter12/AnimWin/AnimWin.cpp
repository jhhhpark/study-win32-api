#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("AnimWin");

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
	CONST TCHAR* Mes = TEXT("마우스 왼쪽 버튼을 누르시면 애니메이션을 볼 수 있습니다.");
	int i;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		i = (LOWORD(lParam) - 10 )/ 50;
		ShowWindow(hWnd, SW_HIDE);
		switch (i)
		{
		case 0:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_POSITIVE);
			break;
		case 1:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_NEGATIVE);
			break;
		case 2:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_VER_POSITIVE);
			break;
		case 3:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_VER_NEGATIVE);
			break;
		case 4:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_POSITIVE | AW_VER_POSITIVE);
			break;
		case 5:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_HOR_NEGATIVE | AW_VER_NEGATIVE);
			break;
		case 6:
			AnimateWindow(hWnd, 500, AW_SLIDE | AW_BLEND);
			break;
		default:
			AnimateWindow(hWnd, 500, AW_CENTER);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE); 
		SetForegroundWindow(hWnd);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		for (i = 10; i <= 360; i += 50)
			Rectangle(hdc, i, 50, i + 50, 100);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}