#define WINVER 0x500
#include <windows.h>
#include <imm.h>

#pragma comment(lib, "Imm32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("LCDTest");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = NULL;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(WS_EX_TOPMOST, lpszClass, lpszClass, WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int MonCount;
struct tagMon {
	HMONITOR hMon;
	RECT rcMon;
	BOOL bPrimary;
} arMon[10];
int NowMon = 0;

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX mi;

	arMon[MonCount].hMon = hMonitor;
	arMon[MonCount].rcMon = *lprcMonitor;
	mi.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mi);
	arMon[MonCount].bPrimary = (mi.dwFlags & MONITORINFOF_PRIMARY);
	MonCount++;
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	RECT crt;
	CONST TCHAR* Mes = TEXT("LCD 테스트 프로그램 Ver 1.1\r\n스페이스:색상 변경, Esc:종료,")
		TEXT(" Enter:문자열 이동\r\nM:모니터 이동, RGBWK:각 색상 선택");
	int sx, sy;
	static int idx = 4;
	static COLORREF col[] = { RGB(255,0,0),RGB(0,255,0),RGB(0,0,255),
		RGB(255,255,255),RGB(0,0,0) };
	static UINT Align = DT_CENTER;

	switch (iMessage) {
	case WM_CREATE:
		// 최대 크기로 확장
		sx = GetSystemMetrics(SM_CXSCREEN);
		sy = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(hWnd, 0, 0, sx, sy, TRUE);
		ShowCursor(FALSE);
		ImmDisableIME(GetCurrentThreadId());
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			idx++;
			if (idx == sizeof(col) / sizeof(col[0]))
				idx = 0;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case VK_RETURN:
			if (Align == 1000) Align = DT_LEFT;
			else if (Align == DT_LEFT) Align = DT_CENTER;
			else if (Align == DT_CENTER) Align = DT_RIGHT;
			else Align = 1000;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'R':
			idx = 0;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'G':
			idx = 1;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'B':
			idx = 2;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'W':
			idx = 3;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'K':
			idx = 4;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 'M':
			if (GetSystemMetrics(SM_CMONITORS) > 1) {
				MonCount = 0;
				EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)NULL);
				if (MonCount > 1) {
					NowMon = (NowMon == MonCount - 1 ? 0 : NowMon + 1);
					MoveWindow(hWnd, arMon[NowMon].rcMon.left, arMon[NowMon].rcMon.top,
						arMon[NowMon].rcMon.right - arMon[NowMon].rcMon.left,
						arMon[NowMon].rcMon.bottom - arMon[NowMon].rcMon.top, TRUE);
				}
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBrush = CreateSolidBrush(col[idx]);
		GetClientRect(hWnd, &crt);
		FillRect(hdc, &crt, hBrush);
		DeleteObject(hBrush);

		if (idx == 4) {
			SetTextColor(hdc, RGB(255, 255, 255));
		}
		SetBkMode(hdc, TRANSPARENT);
		if (Align != 1000) {
			DrawText(hdc, Mes, -1, &crt, Align);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		ShowCursor(TRUE);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
