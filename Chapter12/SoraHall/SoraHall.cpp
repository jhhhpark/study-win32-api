#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("SoraHall");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_POPUP | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 250,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include "resource.h"
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);

POINT sora[] = {
	{1,167},{130,169},{118,148},{199,150},{214,146},{609,153},{688,154},
	{674,178},{798,179},{776,221},{587,217},{588,222},{294,221},{232,215},
	{215,218},{213,212},{23,212}
};
POINT hall1[] = { {27,180},{76,181},{89,205},{39,205} };
POINT hall2[] = { {722,190},{770,190},{756,214},{710,214} };

HBITMAP hSora;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	UINT nHit;
	HRGN hPoly, hEll, hRect;
	HRGN hHall1, hHall2;

	switch (iMessage) {
	case WM_CREATE:
		hPoly = CreatePolygonRgn(sora, sizeof(sora) / sizeof(sora[0]), WINDING);
		hEll = CreateEllipticRgn(214, 3, 592, 182 * 2);
		hRect = CreateRectRgn(214, 0, 592, 182);
		CombineRgn(hEll, hEll, hRect, RGN_AND);
		CombineRgn(hPoly, hPoly, hEll, RGN_OR);

		hHall1 = CreatePolygonRgn(hall1, sizeof(hall1) / sizeof(hall1[0]), WINDING);
		hHall2 = CreatePolygonRgn(hall2, sizeof(hall2) / sizeof(hall2[0]), WINDING);
		CombineRgn(hPoly, hPoly, hHall1, RGN_DIFF);
		CombineRgn(hPoly, hPoly, hHall2, RGN_DIFF);
		SetWindowRgn(hWnd, hPoly, FALSE);
		DeleteObject(hEll);
		DeleteObject(hRect);
		DeleteObject(hHall1);
		DeleteObject(hHall2);
		hSora = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBitmap(hdc, 0, 0, hSora);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_NCHITTEST:
		nHit = DefWindowProc(hWnd, iMessage, wParam, lParam);
		if (nHit == HTCLIENT)
			nHit = HTCAPTION;
		return nHit;
	case WM_DESTROY:
		DeleteObject(hSora);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}