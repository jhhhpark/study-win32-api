#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SetParent");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hBtn;
#define IDBTN 100
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = TEXT("�¿� ���콺 ��ư�� ���� ���ʽÿ�");

	switch (iMessage) {
	case WM_CREATE:
		hBtn = CreateWindow(TEXT("button"), TEXT("�ڽ� ��ư"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 20, 100, 25, hWnd, (HMENU)IDBTN, g_hInst, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		SetParent(hBtn, GetDesktopWindow());
		SetWindowText(hBtn, TEXT("������ ��ư"));
		return TRUE;
	case WM_RBUTTONDOWN:
		SetParent(hBtn, hWnd);
		SetWindowText(hBtn, TEXT("���ƿ� ��ư"));
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDBTN:
			MessageBox(hWnd, TEXT("��ư�� �������ϴ�."), TEXT("�˸�"), MB_OK);
			break;
		}
		return TRUE;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 20, 50, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
