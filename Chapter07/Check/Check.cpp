#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("CheckBox");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS wndClass;
	g_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND c1, c2, c3, c4;
	static BOOL bElipse = FALSE;
	switch (iMessage)
	{
	case WM_CREATE:
		c1 = CreateWindow(TEXT("button"), TEXT("Draw Elipse?"), WS_CHILD | WS_VISIBLE |
			BS_CHECKBOX, 20, 20, 160, 25, hWnd, (HMENU)0, g_hInst, NULL);

		c2 = CreateWindow(TEXT("button"), TEXT("Good bye Message?"), WS_CHILD | WS_VISIBLE |
			BS_AUTOCHECKBOX, 20, 50, 160, 25, hWnd, (HMENU)1, g_hInst, NULL);

		c3 = CreateWindow(TEXT("button"), TEXT("3State"), WS_CHILD | WS_VISIBLE |
			BS_3STATE, 20, 80, 160, 25, hWnd, (HMENU)2, g_hInst, NULL);

		c4 = CreateWindow(TEXT("button"), TEXT("Auto 3State"), WS_CHILD | WS_VISIBLE |
			BS_AUTO3STATE, 20, 110, 160, 25, hWnd, (HMENU)3, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
			if (SendMessage(c1, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(c1, BM_SETCHECK, BST_CHECKED, 0);
				bElipse = TRUE;
			}
			else
			{
				SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);
				bElipse = FALSE;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case 1:
			// AutoCheckBox ?????? ???? ??
			bElipse = (BOOL)(SendMessage(c2, BM_GETCHECK, 0, 0) == BST_CHECKED);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			if (SendMessage(c3, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(c3, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				if (SendMessage(c3, BM_GETCHECK, 0, 0) == BST_INDETERMINATE)
				{
					SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				else
				{
					SendMessage(c3, BM_SETCHECK, BST_INDETERMINATE, 0);
				}
			}
			break;
		default:
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (bElipse == TRUE)
		{
			Ellipse(hdc, 200, 100, 400, 200);
		}
		else
		{
			Rectangle(hdc, 200, 100, 400, 200);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (SendMessage(c2, BM_GETCHECK, 0, 0) == BST_CHECKED)
		{
			MessageBox(hWnd, TEXT("Good bye"), TEXT("Check"), MB_OK);
		}
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}