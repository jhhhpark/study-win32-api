#include <Windows.h>
#include "resource.h"

INT_PTR CALLBACK ReadingDlgProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hDlgMain;
LPCTSTR lpszClass = TEXT("ReadingCounter");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	g_hInst = hInstance;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_READING), HWND_DESKTOP, ReadingDlgProc);

	return 0;
}

int PageStart, PageEnd, NowPage;
int PageSec, NowSec;
enum tag_Status { WAIT, RUN, PAUSE };
tag_Status Status;

INT_PTR CALLBACK ReadingDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR Mes[256] = {};

	switch (iMessage)
	{
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		Status = WAIT;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNSTART:
			PageStart = GetDlgItemInt(hDlg, IDC_EDSTART, NULL, FALSE);
			PageEnd = GetDlgItemInt(hDlg, IDC_EDEND, NULL, FALSE);
			if (PageStart == 0 || PageEnd == 0 || PageStart >= PageEnd)
			{
				MessageBox(hDlg, TEXT("페이지 번호를 다시 입력하세요"), TEXT("알림"), MB_OK);
				return TRUE;
			}
			NowPage = PageStart;
			PageSec = GetDlgItemInt(hDlg, IDC_EDTIME, NULL, FALSE);
			if (PageSec <= 0)
			{
				MessageBox(hDlg, TEXT("시간을 다시 입력하시오."), TEXT("알림"), MB_OK);
				return TRUE;
			}
			NowSec = PageSec;
			SetTimer(hDlg, 1, 1000, NULL);
			SendMessage(hDlg, WM_TIMER, (WPARAM)1, (LPARAM)0);
			Status = RUN;
			return (INT_PTR)TRUE;
		case IDC_BTNPAUSE:
			if (Status == PAUSE)
			{
				SetTimer(hDlg, 1, 1000, NULL);
				Status = RUN;
			}
			else if (Status == RUN)
			{
				KillTimer(hDlg, 1);
				Status = PAUSE;
			}
			break;
		case IDCANCEL:
			KillTimer(hDlg, 1);
			EndDialog(hDlg, IDCANCEL);
			Status = WAIT;
			return (INT_PTR)TRUE;
		}
		return TRUE;
	case WM_TIMER:
		NowSec--;
		if (NowSec == 0)
		{
			if (IsDlgButtonChecked(hDlg, IDC_CHKBEEP) == BST_CHECKED)
			{
				MessageBeep(0);
			}
			NowPage++;
			NowSec = PageSec;
			if (NowPage > PageEnd)
			{
				SetDlgItemText(hDlg, IDC_STTIME, TEXT("다 읽었습니다."));
				KillTimer(hDlg, 1);
				Status = WAIT;
				return TRUE;
			}
		}
		wsprintf(Mes, TEXT("%d 페이지 읽는 중 : %d초 남음"), NowPage, NowSec);
		SetDlgItemText(hDlg, IDC_STTIME, Mes);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (INT_PTR)FALSE;
}