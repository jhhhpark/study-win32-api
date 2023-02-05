#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GetKeyState");

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
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Mes[128] = TEXT("����: ���� �˻�, ������: �񵿱� �˻�, T: �ð� ����");
	static TCHAR str1[128] = TEXT("�˻���"), str2[128] = TEXT("�˻���");
	TCHAR str3[128] = {};
	int i;

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			lstrcpy(str1, TEXT("���� �˻� : ShiftŰ�� ������"));
		}
		else
		{
			lstrcpy(str1, TEXT("���� �˻� : ShiftŰ�� �� ������"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_RBUTTONDOWN:
		if (GetAsyncKeyState(VK_SHIFT) & 0X8000)
		{
			lstrcpy(str2, TEXT("�񵿱� �˻� : ShiftŰ�� ������"));
		}
		else
		{
			lstrcpy(str2, TEXT("�񵿱� �˻� : ShiftŰ�� �� ������"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'T':
			for (i = 0; i < 30; ++i)
			{
				wsprintf(str3, TEXT("ī��Ʈ = %d"), i);
				SetWindowText(hWnd, str3);
				Sleep(100);
			}
			break;
		default:
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		TextOut(hdc, 10, 30, str1, lstrlen(str1));
		TextOut(hdc, 10, 50, str2, lstrlen(str2));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}