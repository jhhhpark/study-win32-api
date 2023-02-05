#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MessageTrap");

CONST TCHAR* arMes[1024] = {
		"WM_NULL","WM_CREATE","WM_DESTROY","WM_MOVE","","WM_SIZE","WM_ACTIVATE","WM_SETFOCUS",
		"WM_KILLFOCUS","","WM_ENABLE","WM_SETREDRAW","WM_SETTEXT","WM_GETTEXT","WM_GETTEXTLENGTH","WM_PAINT",
		/*10*/	"WM_CLOSE","WM_QUERYENDSESSION","WM_QUIT","WM_QUERYOPEN","WM_ERASEBKGND","WM_SYSCOLORCHANGE","WM_ENDSESSION","",
				"WM_SHOWWINDOW","","WM_WININICHANGE","WM_DEVMODECHANGE","WM_ACTIVATEAPP","WM_FONTCHANGE","WM_TIMECHANGE","WM_CANCELMODE",
				/*20*/	"WM_SETCURSOR","WM_MOUSEACTIVATE","WM_CHILDACTIVATE","WM_QUEUESYNC","WM_GETMINMAXINFO","","WM_PAINTICON","WM_ICONERASEBKGND",
						"WM_NEXTDLGCTL","","WM_SPOOLERSTATUS","WM_DRAWITEM","WM_MEASUREITEM","WM_DELETEITEM","WM_VKEYTOITEM","WM_CHARTOITEM",
						/*30*/	"WM_SETFONT","WM_GETFONT","WM_SETHOTKEY","WM_GETHOTKEY","","","","WM_QUERYDRAGICON",
								"","WM_COMPAREITEM","","","","WM_GETOBJECT","","",
								/*40*/	"","WM_COMPACTION","","","WM_COMMNOTIFY","","WM_WINDOWPOSCHANGING","WM_WINDOWPOSCHANGED",
										"WM_POWER","","WM_COPYDATA","WM_CANCELJOURNAL","","","WM_NOTIFY","",
										/*50*/	"WM_INPUTLANGCHANGEREQUEST","WM_INPUTLANGCHANGE","WM_TCARD","WM_HELP","WM_USERCHANGED","WM_NOTIFYFORMAT","","",
												"","","","","","","","",
												/*60*/	"","","","","","","","",
														"","","","","","","","",
														/*70*/	"","","","","","","","",
																"","","","WM_CONTEXTMENU","WM_STYLECHANGING","WM_STYLECHANGED","WM_DISPLAYCHANGE","WM_GETICON",
																/*80*/	"WM_SETICON","WM_NCCREATE","WM_NCDESTROY","WM_NCCALCSIZE","WM_NCHITTEST","WM_NCPAINT","WM_NCACTIVATE","WM_GETDLGCODE",
																		"WM_SYNCPAINT","","","","","","","",
																		/*90*/	"","","","","","","","",
																				"","","","","","","","",
																				/*A0*/	"WM_NCMOUSEMOVE","WM_NCLBUTTONDOWN","WM_NCLBUTTONUP","WM_NCLBUTTONDBLCLK","WM_NCRBUTTONDOWN","WM_NCRBUTTONUP","WM_NCRBUTTONDBLCLK","WM_NCMBUTTONDOWN",
																						"WM_NCMBUTTONUP","WM_NCMBUTTONDBLCLK","","","","","","",
																						/*B0*/	"","","","","","","","",
																								"","","","","","","","",
																								/*C0*/	"","","","","","","","",
																										"","","","","","","","",
																										/*D0*/	"","","","","","","","",
																												"","","","","","","","",
																												/*E0*/	"","","","","","","","",
																														"","","","","","","","",
																														/*F0*/	"","","","","","","","",
																																"","","","","","","","",
																																/*100*/	"WM_KEYDOWN","WM_KEYUP","WM_CHAR","WM_DEADCHAR","WM_SYSKEYDOWN","WM_SYSKEYUP","WM_SYSCHAR","WM_SYSDEADCHAR",
																																		"","","","","","WM_IME_STARTCOMPOSITION","WM_IME_ENDCOMPOSITION","WM_IME_COMPOSITION",
																																		/*110*/	"WM_INITDIALOG","WM_COMMAND","WM_SYSCOMMAND","WM_TIMER","WM_HSCROLL","WM_VSCROLL","WM_INITMENU","WM_INITMENUPOPUP",
																																				"","","","","","","","WM_MENUSELECT",
																																				/*120*/	"WM_MENUCHAR","WM_ENTERIDLE","WM_MENURBUTTONUP","WM_MENUDRAG","WM_MENUGETOBJECT","WM_UNINITMENUPOPUP","WM_MENUCOMMAND","",
																																						"","","","","","","","",
																																						/*130*/	"","","WM_CTLCOLORMSGBOX","WM_CTLCOLOREDIT","WM_CTLCOLORLISTBOX","WM_CTLCOLORBTN","WM_CTLCOLORDLG","WM_CTLCOLORSCROLLBAR",
																																								"WM_CTLCOLORSTATIC","","","","","","","",
																																								/*140*/	"","","","","","","","",
																																										"","","","","","","","",
																																										/*150*/	"","","","","","","","",
																																												"","","","","","","","",
																																												/*160*/	"","","","","","","","",
																																														"","","","","","","","",
																																														/*170*/	"","","","","","","","",
																																																"","","","","","","","",
																																																/*180*/	"","","","","","","","",
																																																		"","","","","","","","",
																																																		/*190*/	"","","","","","","","",
																																																				"","","","","","","","",
																																																				/*1A0*/	"","","","","","","","",
																																																						"","","","","","","","",
																																																						/*1B0*/	"","","","","","","","",
																																																								"","","","","","","","",
																																																								/*1C0*/	"","","","","","","","",
																																																										"","","","","","","","",
																																																										/*1D0*/	"","","","","","","","",
																																																												"","","","","","","","",
																																																												/*1E0*/	"","","","","","","","",
																																																														"","","","","","","","",
																																																														/*1F0*/	"","","","","","","","",
																																																																"","","","","","","","",
																																																																/*200*/	"WM_MOUSEMOVE","WM_LBUTTONDOWN","WM_LBUTTONUP","WM_LBUTTONDBLCLK","WM_RBUTTONDOWN","WM_RBUTTONUP","WM_RBUTTONDBLCLK","WM_MBUTTONDOWN",
																																																																		"WM_MBUTTONUP","WM_MBUTTONDBLCLK","WM_MOUSEWHEEL","","","","","",
																																																																		/*210*/	"WM_PARENTNOTIFY","WM_ENTERMENULOOP","WM_EXITMENULOOP","WM_NEXTMENU","WM_SIZING","WM_CAPTURECHANGED","WM_MOVING","",
																																																																				"","WM_DEVICECHANGE","","","","","","",
																																																																				/*220*/	"WM_MDICREATE","WM_MDIDESTROY","WM_MDIACTIVATE","WM_MDIRESTORE","WM_MDINEXT","WM_MDIMAXIMIZE","WM_MDITILE","WM_MDICASCADE",
																																																																						"WM_MDIICONARRANGE","WM_MDIGETACTIVE","","","","","","",
																																																																						/*230*/	"WM_MDISETMENU","WM_ENTERSIZEMOVE","WM_EXITSIZEMOVE","WM_DROPFILES","WM_MDIREFRESHMENU","","","",
																																																																								"","","","","","","","",
																																																																								/*240*/	"","","","","","","","",
																																																																										"","","","","","","","",
																																																																										/*250*/	"","","","","","","","",
																																																																												"","","","","","","","",
																																																																												/*260*/	"","","","","","","","",
																																																																														"","","","","","","","",
																																																																														/*270*/	"","","","","","","","",
																																																																																"","","","","","","","",
																																																																																/*280*/	"","WM_IME_SETCONTEXT","WM_IME_NOTIFY","WM_IME_CONTROL","WM_IME_COMPOSITIONFULL","WM_IME_SELECT","WM_IME_CHAR","",
																																																																																		"WM_IME_REQUEST","","","","","","","",
																																																																																		/*290*/	"WM_IME_KEYDOWN","WM_IME_KEYUP","","","","","","",
																																																																																				"","","","","","","","",
																																																																																				/*2A0*/	"WM_MOUSEHOVER","WM_MOUSELEAVE","","","","","","",
																																																																																						"","","","","","","","",
																																																																																						/*2B0*/	"","","","","","","","",
																																																																																								"","","","","","","","",
																																																																																								/*2C0*/	"","","","","","","","",
																																																																																										"","","","","","","","",
																																																																																										/*2D0*/	"","","","","","","","",
																																																																																												"","","","","","","","",
																																																																																												/*2E0*/	"","","","","","","","",
																																																																																														"","","","","","","","",
																																																																																														/*2F0*/	"","","","","","","","",
																																																																																																"","","","","","","","",
																																																																																																/*300*/	"WM_CUT","WM_COPY","WM_PASTE","WM_CLEAR","WM_UNDO","WM_RENDERFORMAT","WM_RENDERALLFORMATS","WM_DESTROYCLIPBOARD",
																																																																																																		"WM_DRAWCLIPBOARD","WM_PAINTCLIPBOARD","WM_VSCROLLCLIPBOARD","WM_SIZECLIPBOARD","WM_ASKCBFORMATNAME","WM_CHANGECBCHAIN","WM_HSCROLLCLIPBOARD","WM_QUERYNEWPALETTE",
																																																																																																		/*310*/	"WM_PALETTEISCHANGING","WM_PALETTECHANGED","WM_HOTKEY","","","","","WM_PRINT",
																																																																																																				"WM_PRINTCLIENT","","","","","","","",
};

HWND hList;
BOOL bWriteLog = TRUE;
MSG Last;
MSG PreList[1000];
void LogMessage(BOOL bQueue, UINT M, WPARAM W, LPARAM L)
{
	TCHAR str[256];
	static int count = 0;
	int i;
	static BOOL bListFirstCreated = TRUE;

	// 리스트가 생성되기 전에는 아직 표시할 곳이 없으므로 배열에 일단 기록해 놓는다.
	if (hList == NULL) {
		for (i = 0;; i++) {
			if (PreList[i].message == 0) break;
		}
		PreList[i].message = M;
		PreList[i].wParam = (WPARAM)bQueue;
		return;
	}

	if (M < 1024 && M != 308 && bWriteLog) {
		// 리스트 박스 생성 후 기록해 놓은 메시지들을 리스트 박스에 출력한다.
		if (bListFirstCreated) {
			for (i = 0;; i++) {
				if (PreList[i].message == 0) break;
				wsprintf(str, "%d = %s : %s(%d), W=%x, L=%x", count++,
					PreList[i].wParam ? "Queue" : "NonQu",
					arMes[PreList[i].message], PreList[i].message, W, L);
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
			}
			bListFirstCreated = FALSE;
		}
		wsprintf(str, "%d = %s : %s(%d), W=%x, L=%x", count++, bQueue ? "Queue" : "NonQu",
			arMes[M], M, W, L);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
		SendMessage(hList, LB_SETCURSEL, SendMessage(hList, LB_GETCOUNT, 0, 0) - 1, 0);
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {

		// 리스트 박스의 메시지는 기록하지 않고 메인 윈도우의 메시지만 기록한다.
		if (Message.hwnd == hWnd) {
			LogMessage(TRUE, Message.message, Message.wParam, Message.lParam);
			Last = Message;
		}
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	CONST TCHAR* Mes = "메시지 감시 예제. F1,F2:기록 중지/재개, F3:목록 비움, F4,F5:타이머 설치/제거";

	// 큐로부터 온 메시지는 넌큐로 중복 기록하지 않는다.
	if (Last.message != iMessage) {
		LogMessage(FALSE, iMessage, wParam, lParam);
	}
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			LBS_NOTIFY | WS_VSCROLL, 0, 0, 0, 0, hWnd, (HMENU)0, g_hInst, NULL);
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			MoveWindow(hList, 0, 40, 500, HIWORD(lParam) - 40, TRUE);
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F1:
			bWriteLog = FALSE;
			break;
		case VK_F2:
			bWriteLog = TRUE;
			break;
		case VK_F3:
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			break;
		case VK_F4:
			SetTimer(hWnd, 1, 1000, NULL);
			break;
		case VK_F5:
			KillTimer(hWnd, 1);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
		// 리스트 박스에서 포커스를 뺏기지 않도록 하여 메시지를 지속적으로 살펴본다.
	case WM_KILLFOCUS:
		SetFocus(hWnd);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
