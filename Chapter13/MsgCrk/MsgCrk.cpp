#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MsgCrk");

BOOL MsgCrk_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void MsgCrk_OnDestroy(HWND hwnd);
void MsgCrk_OnPaint(HWND hwnd);
void MsgCrk_OnKey(HWND hwnd, UINT vk, BOOL fDown, int nRepeat, UINT flags);
void MsgCrk_OnLButtonDown(HWND hwnd, BOOL fDownClick, int x, int y, UINT keyFlags);
void MsgCrk_OnTimer(HWND hwnd, UINT id);
void MsgCrk_OnSize(HWND HWND, UINT state, int cx, int cy);


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
	switch (iMessage)
	{
		HANDLE_MSG(hWnd, WM_CREATE, MsgCrk_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, MsgCrk_OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, MsgCrk_OnPaint);
		HANDLE_MSG(hWnd, WM_KEYDOWN, MsgCrk_OnKey);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, MsgCrk_OnLButtonDown);
		HANDLE_MSG(hWnd, WM_TIMER, MsgCrk_OnTimer);
		HANDLE_MSG(hWnd, WM_SIZE, MsgCrk_OnSize);
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL MsgCrk_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	SetTimer(hwnd, 0, 1000, NULL);
	return TRUE;
}

void MsgCrk_OnDestroy(HWND hwnd)
{
	KillTimer(hwnd, 0);
	DestroyWindow(hwnd);
	return;
}

void MsgCrk_OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = TEXT("메시지 크랙커를 사용하는 예제입니다.");
	hdc = BeginPaint(hwnd, &ps);
	TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
	EndPaint(hwnd, &ps);
	return;
}

void MsgCrk_OnKey(HWND hwnd, UINT vk, BOOL fDown, int nRepeat, UINT flags)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc = GetDC(hwnd);
	SetRect(&rt, 10, 80, 500, 100);
	FillRect(hdc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str, TEXT("%c키를 눌렀습니다."), vk);
	TextOut(hdc, 10, 80, str, lstrlen(str));
	ReleaseDC(hwnd, hdc);
	return;
}

void MsgCrk_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc = GetDC(hwnd);
	SetRect(&rt, 10, 100, 500, 120);
	FillRect(hdc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	lstrcpy(str, TEXT("마우스 왼쪽 버튼을 눌렀습니다."));
	TextOut(hdc, 10, 100, str, lstrlen(str));
	ReleaseDC(hwnd, hdc);
}

void MsgCrk_OnTimer(HWND hwnd, UINT id)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	static int Count = 0;
	hdc = GetDC(hwnd);
	SetRect(&rt, 10, 60, 500, 80);
	FillRect(hdc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str, TEXT("%d초 경과했습니다."), Count++);
	TextOut(hdc, 10, 60, str, lstrlen(str));
	ReleaseDC(hwnd, hdc);
	return;
}

void MsgCrk_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc = GetDC(hwnd);
	SetRect(&rt, 10, 40, 500, 60);
	FillRect(hdc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str, TEXT("현재 작업영역의 크기:%d*%d"), cx, cy);
	TextOut(hdc, 10, 40, str, lstrlen(str));
	ReleaseDC(hwnd, hdc);
}
