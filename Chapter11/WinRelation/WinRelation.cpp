#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PopupProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WinRelation");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM);
	WndClass.lpfnWndProc = ChildProc;
	WndClass.lpszClassName = TEXT("ChildClass");
	RegisterClass(&WndClass);

	WndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	WndClass.lpfnWndProc = PopupProc;
	WndClass.lpszClassName = TEXT("PopupClass");
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hChild, hPopup, h1, h2, h11, h12, h21, h22;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hParent, hOwner;
	TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		hChild = CreateWindow(TEXT("ChildClass"), NULL, WS_CHILD | WS_VISIBLE,
			10, 10, 200, 200,
			hWnd, (HMENU)0, g_hInst, NULL);
		hPopup = CreateWindow(TEXT("PopupClass"), TEXT("팝업 윈도우"), WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
			10, 10, 200, 200,
			hWnd, (HMENU)0, g_hInst, NULL);
		h1 = CreateWindow(TEXT("button"), TEXT("버튼1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		h2 = CreateWindow(TEXT("button"), TEXT("버튼2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("My Handle is %d"), (int)hWnd);
		TextOut(hdc, 10, 220, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hParent = GetParent(hWnd);
		hOwner = GetWindow(hWnd, GW_OWNER);
		wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
		MessageBox(hWnd, str, TEXT("결과"), MB_OK);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 0:
			hParent = GetParent(h1);
			hOwner = GetWindow(h1, GW_OWNER);
			wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
			MessageBox(hWnd, str, TEXT("결과"), MB_OK);
			return 0;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hParent, hOwner;
	TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		h11 = CreateWindow(TEXT("button"), TEXT("버튼11"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		h12 = CreateWindow(TEXT("button"), TEXT("버튼12"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("My Handle is %d"), (int)hWnd);
		TextOut(hdc, 20, 80, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hParent = GetParent(hWnd);
		hOwner = GetWindow(hWnd, GW_OWNER);
		wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
		MessageBox(hWnd, str, TEXT("결과"), MB_OK);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 0:
			hParent = GetParent(h11);
			hOwner = GetWindow(h11, GW_OWNER);
			wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
			MessageBox(hWnd, str, TEXT("결과"), MB_OK);
			return 0;
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK PopupProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hParent, hOwner;
	TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		h21 = CreateWindow(TEXT("button"), TEXT("버튼21"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		h22 = CreateWindow(TEXT("button"), TEXT("버튼22"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("My Handle is %d"), (int)hWnd);
		TextOut(hdc, 20, 80, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hParent = GetParent(hWnd);
		hOwner = GetWindow(hWnd, GW_OWNER);
		wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
		MessageBox(hWnd, str, TEXT("결과"), MB_OK);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 0:
			hParent = GetParent(h21);
			hOwner = GetWindow(h21, GW_OWNER);
			wsprintf(str, TEXT("Parent=%d, Owner=%d"), (int)hParent, (int)hOwner);
			MessageBox(hWnd, str, TEXT("결과"), MB_OK);
			return 0;
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
