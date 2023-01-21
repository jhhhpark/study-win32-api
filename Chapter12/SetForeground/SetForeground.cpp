#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SetForeground");

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
	static HWND hNotePad;
	CONST TCHAR* Mes = TEXT("1:메모장 즉시 활성화, 2,4: 메모장 3초 후에 활성화, "
		TEXT("3,5:3초 후 스스로 활성화"));

	switch (iMessage)
	{
	case WM_CREATE:
		hNotePad = FindWindow("NotePad", NULL);
		if (hNotePad == NULL)
		{
			MessageBox(NULL, TEXT("이 예제를 실행하기 전에 메모장을 먼저 실행하십시오."), TEXT("알림"), MB_OK);
			return -1;
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
			// SetActiveWindow(hNotePad); // 다른 스레드 내의 윈도우라서 활성화 안됨
			SetForegroundWindow(hNotePad);
			break;
		case '2':
			SetTimer(hWnd, 2, 3000, NULL);
			break;
		case '3':
			SetTimer(hWnd, 3, 3000, NULL);
			break;
		case '4':
			SetTimer(hWnd, 4, 3000, NULL);
			break;
		case '5':
			SetTimer(hWnd, 4, 3000, NULL);
			break;

		default:
			break;
		}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 2:
			KillTimer(hWnd, 2);
			SetForegroundWindow(hNotePad);
			break;
		case 3:
			KillTimer(hWnd, 3);
			SetForegroundWindow(hWnd);
			break;
		case 4:
			KillTimer(hWnd, 4);
			BringWindowToTop(hNotePad);
			break;
		case 5:
			KillTimer(hWnd, 4);
			BringWindowToTop(hWnd);
			break;
		default:
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}