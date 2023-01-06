#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MajorDlgProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("DlgCheck");

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

BOOL bMale;
enum tag_Major { MANAGE, KOREAN, ENGLISH, LAW } Major;
CONST TCHAR* arMajor[] = { TEXT("경영학과"), TEXT("국문학과"), TEXT("영문학과"), TEXT("법학과") };
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[128] = {};

	switch (iMessage)
	{
	case WM_CREATE:
		bMale = TRUE;
		Major = KOREAN;
		return 0;
	case WM_LBUTTONDOWN:
		if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MajorDlgProc) == IDOK)
		{
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("성별 = %s"), bMale ? TEXT("남자") : TEXT("여자"));
		TextOut(hdc, 10, 10, str, lstrlen(str));
		wsprintf(str, TEXT("전공 = %s"), arMajor[Major]);
		TextOut(hdc, 10, 30, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

int GetCheckRadioButton(HWND hDlg, int First, int Last)
{
	int id;
	for (id = First; id <= Last; ++id)
	{
		if (IsDlgButtonChecked(hDlg, id) == BST_CHECKED)
			return id;
	}

	return -1;
}

INT_PTR CALLBACK MajorDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		CheckDlgButton(hDlg, IDC_MALE, bMale ? BST_CHECKED : BST_UNCHECKED);
		CheckRadioButton(hDlg, IDC_MAJOR1, IDC_MAJOR4, IDC_MAJOR1 + Major);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			bMale = (IsDlgButtonChecked(hDlg, IDC_MALE) == BST_CHECKED);
			Major = (tag_Major)(GetCheckRadioButton(hDlg, IDC_MAJOR1, IDC_MAJOR4) - IDC_MAJOR1);
			EndDialog(hDlg, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}

	return (INT_PTR)FALSE;
}
