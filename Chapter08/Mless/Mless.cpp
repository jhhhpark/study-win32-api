#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MlessDlgProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain, hMDlg;
LPCTSTR lpszClass = TEXT("Mless");

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
		if (!IsWindow(hMDlg) || !IsDialogMessage(hMDlg, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

int x, y;
TCHAR str[128] = {};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		x = 100;
		y = 100;
		lstrcpy(str, TEXT("String"));
		return 0;
	case WM_LBUTTONDOWN:
		hMDlg = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_MLESS), hWnd, MlessDlgProc);
		ShowWindow(hMDlg, SW_SHOW); // WS_VISIBLE 속성을 주면 이 함수 생략
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

INT_PTR CALLBACK MlessDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_STR, str);
		SetDlgItemInt(hDlg, IDC_X, x, FALSE);
		SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CHANGE:
		{
			GetDlgItemText(hDlg, IDC_STR, str, 128);
			x = GetDlgItemInt(hDlg, IDC_X, NULL, TRUE);
			y = GetDlgItemInt(hDlg, IDC_Y, NULL, TRUE);
			InvalidateRect(hWndMain, NULL, TRUE);
			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		case ID_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			return (INT_PTR)TRUE;
		}
	default:
		break;
	}


	return (INT_PTR)FALSE;
}
