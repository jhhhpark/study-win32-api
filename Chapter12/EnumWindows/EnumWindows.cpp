#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

BOOL CALLBACK MyEnumWindowsProc(HWND hWnd, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("EnumWindows");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS wndClass;
	g_hInst = hInstance;

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
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
HWND hList;
int num;

// 콜백함수, EnumWindows 함수가 윈도우를 찾을 때마다 그 정보를 넘겨준다.
// 윈도우 핸들을 받아 이 윈도우의 캡션을 조사하거나 변경할 수도 있다.
BOOL CALLBACK MyEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR str[300] = {};
	TCHAR Cap[255];
	RECT rt;
	++num;
	// 핸들로부터 캡션, 좌표를 조사하여 리스트 박스에 출력한다.
	GetWindowText(hWnd, Cap, 255);
	GetWindowRect(hWnd, &rt);
	wsprintf(str, TEXT("%d. 핸들=0x%x(%d), 캡션=%s, 좌표=(%d, %d, %d, %d)"),
		num, (int)hWnd, (int)hWnd, Cap, rt.left, rt.top, rt.right, rt.bottom);
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = TEXT("윈도우 바닥을 왼쪽 마우스 버튼으로 클릭하면 윈도우")
		TEXT(" 목록을 보여 드릴께요");
	
	switch (iMessage)
	{
	case WM_CREATE:
		// 윈도우 목록을 보여주기 위해 리스트 박스 하나 생성
		hList = CreateWindow(TEXT("listbox"), NULL, WS_VISIBLE | WS_CHILD | WS_BORDER |
			LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
			10, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		// 리스트 박스를 비우고 윈도우 목록을 조사한다.
		num = 0;
		SendMessage(hList, LB_RESETCONTENT, 0, 0);
		EnumWindows(MyEnumWindowsProc, NULL);
		//EnumChildWindows(hWnd, MyEnumWindowsProc, NULL);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		// 리스트 박스가 항상 윈도우를 가득 채우도록...
		MoveWindow(hList, 0, 50, LOWORD(lParam), HIWORD(lParam) - 50, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}