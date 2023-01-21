#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WmSize2");

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

#define ID_LISTBOX 100
CONST TCHAR* items[] = { TEXT("Apple"), TEXT("Orange"), TEXT("Melon"), TEXT("Grape"), TEXT("Strawberry") };
HWND hList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int i;
	RECT crt;

	switch (iMessage)
	{
	case WM_CREATE:
		hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | LBS_NOINTEGRALHEIGHT,
			0, 0, 0, 0, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
		for (i = 0; i < 5; ++i)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)items[i]);
		}
		return 0;
	case WM_SIZE:
		GetClientRect(hWnd, &crt);
		MoveWindow(hList, crt.left, crt.top, crt.right, crt.bottom, TRUE);
		return 0;
	case WM_SIZING:
		if (wParam == WMSZ_LEFT) // 왼쪽 경계선을 드래그 해볼 것
			((RECT*)lParam)->left = ((RECT*)lParam)->left / 50 * 50; // a를 b의 근접 배수로 내림하는 공식 a = a-a%b 와 동일
		// 좌표 변경시에는 반드시 TRUE를 반환할 것
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}