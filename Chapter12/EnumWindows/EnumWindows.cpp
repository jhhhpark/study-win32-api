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

// �ݹ��Լ�, EnumWindows �Լ��� �����츦 ã�� ������ �� ������ �Ѱ��ش�.
// ������ �ڵ��� �޾� �� �������� ĸ���� �����ϰų� ������ ���� �ִ�.
BOOL CALLBACK MyEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR str[300] = {};
	TCHAR Cap[255];
	RECT rt;
	++num;
	// �ڵ�κ��� ĸ��, ��ǥ�� �����Ͽ� ����Ʈ �ڽ��� ����Ѵ�.
	GetWindowText(hWnd, Cap, 255);
	GetWindowRect(hWnd, &rt);
	wsprintf(str, TEXT("%d. �ڵ�=0x%x(%d), ĸ��=%s, ��ǥ=(%d, %d, %d, %d)"),
		num, (int)hWnd, (int)hWnd, Cap, rt.left, rt.top, rt.right, rt.bottom);
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = TEXT("������ �ٴ��� ���� ���콺 ��ư���� Ŭ���ϸ� ������")
		TEXT(" ����� ���� �帱����");
	
	switch (iMessage)
	{
	case WM_CREATE:
		// ������ ����� �����ֱ� ���� ����Ʈ �ڽ� �ϳ� ����
		hList = CreateWindow(TEXT("listbox"), NULL, WS_VISIBLE | WS_CHILD | WS_BORDER |
			LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
			10, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		// ����Ʈ �ڽ��� ���� ������ ����� �����Ѵ�.
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
		// ����Ʈ �ڽ��� �׻� �����츦 ���� ä�쵵��...
		MoveWindow(hList, 0, 50, LOWORD(lParam), HIWORD(lParam) - 50, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}