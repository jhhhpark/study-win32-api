#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Test");

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

// #define NO_STRICT���� �������� ������ �̰� ���������� ���� HANDLE Ÿ���ε��� 
// ������ �̷������ �ʴ´�.
// WHY?
/*
* winnt.h ��� ������ ����
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT
typedef void *HANDLE;
#if 0 && (_MSC_VER > 1000)
#define DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
��� �Ǿ��ְ�,
STRICT�� ���ǵǾ� ���� ������(NO_STRICT ����),
typedef PVOID HANDLE;�� HANDLE Ÿ���� �� PVOID ���̴�.
�׷��� DECLARE_HANDLE(name) ��ũ�� �Լ��� ȣ�� �� �ش� name�� ���� Ÿ����
typedef HANDLE name���� �����ϰ� �ִ�.

STRICT�� ���ǵǾ� ������,
typedef void *HANDLE;�� HANDLE void*Ÿ������ ���ǵǸ�
DECLARE_HANDLE(name) ��ũ�� �Լ��� ȣ�� �� �ش� name�� ���� Ÿ����
���Ӱ� �����ϰ� �ִ�.
���� ���
DECLARE_HANDLE(HWND) ��� ��ũ�� �Լ��� ȣ���ϸ�

[NO_STRICT ����]
typedef HANDLE HWND; -> HWND�� HANDLEŸ����

[STRICT ����]
struct HWND__{int unused;}; <- intũ�⸸ŭ�� ����ü
typedef HWND__ *HWND; <- �� ����ü�� ������ Ÿ���� HWND
-> HWND�� HWND__ ������ Ÿ����

winnt.h ��������� ���� ���� HANDLE�� Ÿ�Ե� (HWND, HDC, HPEN, HBRUSH....)��
DECLARE_HANDLE()��ũ�� �Լ��� ���� ���ǵǴµ�
�̷��� Ÿ�Ե��� ����� ������ �±װ� �ٸ��� ������ ���� �ٸ� ����ü�̰�
���� HWND�� HDC�� ���� ȣȯ������ �ʴ� Ÿ������ ���ǵȴ�.
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen;
	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		// ERROR
		hPen = GetDC(hWnd);
		Elipse(hWnd, 1, 2, 3, 4);
		ReleaseDC(hdc, hWnd);
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