#include <Windows.h>
#include "resource.h"

INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MainDlgProc);
	return 0;
}
INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int Age = 100;
	switch (iMessage)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
			EndDialog(hDlg, (INT_PTR)IDOK);
			return TRUE;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return (INT_PTR)FALSE;
}