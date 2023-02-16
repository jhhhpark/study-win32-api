#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("CreateDC");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HDC hdc, hMemDC;
	HBITMAP MyBitmap;
	int x, y;

	int sx = GetSystemMetrics(SM_CXSCREEN);
	int sy = GetSystemMetrics(SM_CYSCREEN);	
	// hdc = GetDC(NULL);
	hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hdc);
	MyBitmap = CreateCompatibleBitmap(hdc, sx, sy);
	SelectObject(hMemDC, MyBitmap);
	BitBlt(hMemDC, 0, 0, sx, sy, hdc, 0, 0, SRCCOPY);

	for (y = sy; y >= 0; y -= 1)
	{
		BitBlt(hdc, 0, y, sx, sy, hMemDC, 0, 0, SRCCOPY);
		Sleep(10);
	}

	for (x = sx; x >= 0; x -= 1)
	{
		BitBlt(hdc, x, 0, sx, sy, hMemDC, 0, 0, SRCCOPY);
		Sleep(10);
	}

	BitBlt(hdc, 0, 0, sx, sy, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);
	DeleteDC(hdc);
	DeleteObject(MyBitmap);
	return 0;
}
