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
//	CONST TCHAR* Mes = TEXT("16픽셀 단위로만 움직입니다.");
//
//	switch (iMessage)
//	{
//	case WM_WINDOWPOSCHANGING:
//		/*
//		* 이 멤버들의 하위 4비트를 무조건 0으로 만드렁 버린다. 그래서 좌표가 0, 15, 32, 48, 64 등과 같이
//		* 16의 배수로만 지정되며 윈도우가 16픽셀 경계로 정렬된다.
//		* 크기도 마찬가지로 16픽셀 단위로만 축소/확장될 것이다.
//		* lParam으로 전달되는 WINDOWPOS 구조체는 현재 이동중인 윈도우의 영역을 알려주기도 하지만
//		* 이 값을 편집하면 그 결과가 새 위치에도 반영되므로 이출력용 인수라고 할 수 있다.
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

// 스크린 가장자리 근처에 가면 가장자리에 착 달라붙는 예제
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx, sy;
	int t;
	LPWINDOWPOS wp;
	CONST TCHAR* Mes = TEXT("화면 가장자리에 밀착합니다.");

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