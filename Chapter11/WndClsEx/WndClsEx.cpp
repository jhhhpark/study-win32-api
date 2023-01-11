#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("WndClsEx");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wndClassEx;
	g_hInst = hInstance;

	wndClassEx.cbSize = sizeof(WNDCLASSEX); // 추가된 부분
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClassEx.hIconSm = NULL; // 추가된 부분
	wndClassEx.hInstance = hInstance;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.lpszClassName = lpszClass;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	if (RegisterClassEx(&wndClassEx) == 0)
	{
		MessageBox(NULL, TEXT("윈도우 클래스 등록 실패"), TEXT("에러"), MB_OK);
		return -1;
	}
	// 16x16 아이콘(hIconSm)에 NULL을 지정했는데 
	// 이러면 hIcon멤버가 지정하는 아이콘으로부터 작은 아이콘을 스스로 찾아 사용한다.

	/*
	* 이것이 생겨난 배경이 여러 윈도우가 겹쳐있는 경우
	* 찾고자 하는 윈도우를 찾기 어려웠다. 그래서 이것을 빠르게 찾고자 도입되었다.
	* 그리고 꼭 2개의 아이콘을 제공해야만 하는 것은 아니다. 큰 아이콘만 제공하더라도 윈도우즈는 큰 아이콘을 적당히
	* 작은 아이콘의 모양으로 다르게 그릴 수도 있다.
	* WNDCLASSEX는 작은 아이콘을 사용 가능하다는 것 외에는 WNDCLASS와 큰 차이가 없다.
	*/

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

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
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