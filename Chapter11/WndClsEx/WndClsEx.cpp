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

	wndClassEx.cbSize = sizeof(WNDCLASSEX); // �߰��� �κ�
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClassEx.hIconSm = NULL; // �߰��� �κ�
	wndClassEx.hInstance = hInstance;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.lpszClassName = lpszClass;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	if (RegisterClassEx(&wndClassEx) == 0)
	{
		MessageBox(NULL, TEXT("������ Ŭ���� ��� ����"), TEXT("����"), MB_OK);
		return -1;
	}
	// 16x16 ������(hIconSm)�� NULL�� �����ߴµ� 
	// �̷��� hIcon����� �����ϴ� ���������κ��� ���� �������� ������ ã�� ����Ѵ�.

	/*
	* �̰��� ���ܳ� ����� ���� �����찡 �����ִ� ���
	* ã���� �ϴ� �����츦 ã�� �������. �׷��� �̰��� ������ ã���� ���ԵǾ���.
	* �׸��� �� 2���� �������� �����ؾ߸� �ϴ� ���� �ƴϴ�. ū �����ܸ� �����ϴ��� ��������� ū �������� ������
	* ���� �������� ������� �ٸ��� �׸� ���� �ִ�.
	* WNDCLASSEX�� ���� �������� ��� �����ϴٴ� �� �ܿ��� WNDCLASS�� ū ���̰� ����.
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