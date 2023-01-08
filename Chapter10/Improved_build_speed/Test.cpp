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

// #define NO_STRICT으로 정의하지 않으면 이게 정상적으로 같은 HANDLE 타입인데도 
// 대입이 이루어지지 않는다.
// WHY?
/*
* winnt.h 헤더 파일을 보면
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
라고 되어있고,
STRICT이 정의되어 있지 않으면(NO_STRICT 정의),
typedef PVOID HANDLE;로 HANDLE 타입은 은 PVOID 형이다.
그래서 DECLARE_HANDLE(name) 매크로 함수를 호출 시 해당 name에 들어온 타입을
typedef HANDLE name으로 정의하고 있다.

STRICT이 정의되어 있으면,
typedef void *HANDLE;로 HANDLE void*타입으로 정의되며
DECLARE_HANDLE(name) 매크로 함수를 호출 시 해당 name에 들어온 타입을
새롭게 정의하고 있다.
예를 들어
DECLARE_HANDLE(HWND) 라는 매크로 함수를 호출하면

[NO_STRICT 정의]
typedef HANDLE HWND; -> HWND는 HANDLE타입임

[STRICT 정의]
struct HWND__{int unused;}; <- int크기만큼의 구조체
typedef HWND__ *HWND; <- 그 구조체의 포인터 타입이 HWND
-> HWND는 HWND__ 포인터 타입임

winnt.h 헤더파일을 보면 여러 HANDLE의 타입들 (HWND, HDC, HPEN, HBRUSH....)이
DECLARE_HANDLE()매크로 함수를 통해 정의되는데
이러한 타입들은 모양은 같지만 태그가 다르기 때문에 서로 다른 구조체이고
따라서 HWND와 HDC는 서로 호환ㅈ되지 않는 타입으로 정의된다.
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