#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	
	return 0;
}

void SetAttr(int iOpa)
{
	SetLayeredWindowAttributes(hDlgMain,RGB(0,0,255),iOpa,
		LWA_ALPHA | LWA_COLORKEY);
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, hOldBrush;
	static int iOpa=200;

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetAttr(iOpa);
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlgMain, 0);
			return TRUE;
		case IDC_BTNPLUS:
			iOpa += 25;
			if (iOpa > 255) 
				iOpa = 255;
			SetAttr(iOpa);
			return TRUE;
		case IDC_BTNMINUS:
			iOpa -= 25;
			if (iOpa < 50)
				iOpa = 50;
			SetAttr(iOpa);
			return TRUE;
		default:
			break;
		}
	case WM_PAINT:
		hdc=BeginPaint(hDlg, &ps);
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 10, 10, 300, 80);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
		EndPaint(hDlg, &ps);
		return 0;
	}
	return FALSE;
}
