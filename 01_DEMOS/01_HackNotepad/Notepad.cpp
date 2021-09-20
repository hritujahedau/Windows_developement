#include<windows.h>
#include<stdio.h>
#include "Notepad.h"


enum {
	IDB_HACKNOTEPAD = 3131,
	IDB_HSIZEINCREASE,
	IDB_HSIZEDESCREASE,
	IDB_VSIZEINCREASE,
	IDB_VSIZEDESCREASE,
	IDB_MOVE,
	IDB_ORIGINAL,
	IDB_MENU,
	IDB_RENAME,
	IDB_FONTCHANGE,
	IDB_MOVEUP,
	IDB_MOVEDOWN,
	IDB_MOVERIGHT,
	IDB_MOVELEFT,
	IDB_ICON,
	IDB_CLOSE
};

#define		NOTEPADHACK				"Hack Notepad"
#define		NOTEPADCLOSE			"Close Notepad"
#define		NOTEPADHSIZEINC			"Increase Horizontal size"
#define		NOTEPADHSIZEDESC		"Descrease Horizontal size"
#define		NOTEPADVSIZEINC			"Increase Vertical size"
#define		NOTEPADVSIZEDESC		"Descrease Vertical size"
#define		NOTEPADMOVE				"Move Window (10,10,800,600)"
#define		NOTEPADMOVEORIGINAL		"Set To Initial Window"
#define		NOTEPADREMOVEICON		"Change Icon"
#define		NOTEPADREMOVEMENU		"Remove Menu"
#define		NOTEPADWNDNAME			"Rename Window Name"
#define		NOTEPADFONTCHNAGE		"Add Text with changes font"
#define		NOTEPADUP				"Move Notepad up"
#define		NOTEPADDOWN				"Move Notepad down"
#define		NOTEPADRIGHT			"Move Notepad right"
#define		NOTEPADLEFT				"Move Notepad left"

#define		CHANGE 50

//Gloal Function Declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateButtons(HWND hwnd);
bool CompareTcharString(TCHAR* str, TCHAR* szCharString);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam);

//GloalVariale Declaration

RECT rcWnd , rcNotepadWnd, rcNotepadClient, rcNotepadOriginal;

HWND ButtonHackNotepad =0, ButtonClose = 0, ButtonHSizeInc = 0, ButtonHSizeDesc = 0, ButtonVSizeInc = 0,
ButtonVSizeDesc = 0, ButtonMove = 0, ButtonIcon = 0, ButtonOriginal = 0, ButtonWndName = 0,
ButtonMenu = 0, ButtonFont = 0, ButtonUp = 0, ButtonDown = 0, ButtonRight = 0, ButtonLeft = 0;

HWND hNotepad = NULL;
HMENU hMenu;
HICON hNotepadIcon;
HICON hIcon;

TCHAR szSearchNotepadWindowName[255] = TEXT( "Untitled - Notepad");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR lszptrCammandLine, int iCmdShow) {

	//function Declaration
	void CreateButtons(HWND hwnd);

	//variale  Declaration

	WNDCLASSEX wndClassEx;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");

	// code 
	//initialization 
	
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	wndClassEx.hInstance = hInstance;
	wndClassEx.lpszClassName = szAppName;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hIcon = hIcon;
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hIconSm = hIcon;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;


	RegisterClassEx(&wndClassEx);
	//SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWnd, 0);
	hwnd = CreateWindow(
		szAppName,
		TEXT("Centering Window"),
		WS_OVERLAPPEDWINDOW,
		1000,
		200,
		900,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);


	// message loop

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		CreateButtons(hwnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDB_HACKNOTEPAD:			
			if (hNotepad == NULL) {
				EnumWindows(EnumWindowsProc, lParam);
				GetWindowRect(hNotepad, &rcNotepadWnd);
				GetClientRect(hNotepad, &rcNotepadClient);
				rcNotepadOriginal.left = rcNotepadWnd.left;
				rcNotepadOriginal.top = rcNotepadWnd.top;
				rcNotepadOriginal.right = rcNotepadClient.right;
				rcNotepadOriginal.bottom = rcNotepadClient.bottom;
				hMenu = GetMenu(hNotepad);
				hNotepadIcon = (HICON)SendMessage(hNotepad, WM_GETICON, ICON_BIG, 0);
				MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			}			
			break;
		case IDB_MENU:
			SetMenu(hNotepad, NULL);
			break;
		case IDB_FONTCHANGE:
			EnumChildWindows(hNotepad, EnumChildWindowsProc,1);
			break;
		case IDB_ICON:
			SendMessage(hNotepad, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			break;
		// Resize Hrozonatl and Vertical 
		case IDB_HSIZEINCREASE:
			rcNotepadClient.bottom = rcNotepadClient.bottom + CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_HSIZEDESCREASE:
			rcNotepadClient.bottom = rcNotepadClient.bottom - CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_VSIZEINCREASE:
			rcNotepadClient.right = rcNotepadClient.right + CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_VSIZEDESCREASE:
			rcNotepadClient.right = rcNotepadClient.right - CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_MOVE:
			MoveWindow(hNotepad, 10, 10, 800, 600, TRUE);
			rcNotepadWnd.left = 10;
			rcNotepadWnd.top = 10;
			rcNotepadClient.right = 800;
			rcNotepadClient.bottom = 600;
			break;
		// Move Notepad Window
		case IDB_MOVEUP:
			rcNotepadWnd.top = rcNotepadWnd.top - CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_MOVEDOWN:
			rcNotepadWnd.top = rcNotepadWnd.top + CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_MOVERIGHT:
			rcNotepadWnd.left = rcNotepadWnd.left + CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;
		case IDB_MOVELEFT:
			rcNotepadWnd.left = rcNotepadWnd.left - CHANGE;
			MoveWindow(hNotepad, rcNotepadWnd.left, rcNotepadWnd.top, rcNotepadClient.right, rcNotepadClient.bottom, TRUE);
			break;

		case IDB_ORIGINAL:
			rcNotepadWnd.left = rcNotepadOriginal.left;
			rcNotepadWnd.top = rcNotepadOriginal.top;
			rcNotepadClient.right = rcNotepadOriginal.right;
			rcNotepadClient.bottom = rcNotepadOriginal.bottom;

			SendMessage(hNotepad, WM_SETICON, ICON_BIG, (LPARAM)hNotepadIcon);
			SetMenu(hNotepad, hMenu);
			DrawMenuBar(hNotepad);
			MoveWindow(hNotepad, rcNotepadOriginal.left, rcNotepadOriginal.top, rcNotepadOriginal.right, rcNotepadOriginal.bottom, TRUE);
			SendMessage(hNotepad, WM_SETTEXT, 0, (LPARAM)szSearchNotepadWindowName);
			EnumChildWindows(hNotepad, EnumChildWindowsProc, 0);						
			break;

		case IDB_RENAME:
			SendMessage(hNotepad, WM_SETTEXT, 0, (LPARAM)TEXT("Hrituja Window"));
			break;
		case IDB_CLOSE:
			PostMessage(hNotepad, WM_DESTROY, 0, 0);
			hNotepad = NULL;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

void CreateButtons(HWND hwnd) {

	ButtonHackNotepad = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADHACK), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		10,
		10,
		200,
		100,
		hwnd,
		(HMENU)IDB_HACKNOTEPAD,
		NULL,
		NULL
	);

	ButtonClose = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADCLOSE), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		650,
		10,
		200,
		500,
		hwnd,
		(HMENU)IDB_CLOSE,
		NULL,
		NULL
	);

	ButtonMove = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADMOVE), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		10,
		110,
		200,
		100,
		hwnd,
		(HMENU)IDB_MOVE,
		NULL,
		NULL
	);


	ButtonHSizeInc = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADHSIZEINC), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		25 + 200,
		10,
		200,
		100,
		hwnd,
		(HMENU)IDB_HSIZEINCREASE,
		NULL,
		NULL
	);

	ButtonHSizeDesc = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADHSIZEDESC), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		30 + 400,
		10,
		200,
		100,
		hwnd,
		(HMENU)IDB_HSIZEDESCREASE,
		NULL,
		NULL
	);

	ButtonVSizeInc = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADVSIZEINC), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		25 + 200,
		110,
		200,
		100,
		hwnd,
		(HMENU)IDB_VSIZEINCREASE,
		NULL,
		NULL
	);

	ButtonVSizeDesc = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADVSIZEDESC), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		30 + 400,
		110,
		200,
		100,
		hwnd,
		(HMENU)IDB_VSIZEDESCREASE,
		NULL,
		NULL
	);

	ButtonOriginal = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADMOVEORIGINAL), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		10,
		210,
		200,
		100,
		hwnd,
		(HMENU)IDB_ORIGINAL,
		NULL,
		NULL
	);

	ButtonIcon = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADREMOVEICON), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		230,
		210,
		200,
		100,
		hwnd,
		(HMENU)IDB_ICON,
		NULL,
		NULL
	);

	ButtonWndName = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADWNDNAME), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		430,
		210,
		200,
		100,
		hwnd,
		(HMENU)IDB_RENAME,
		NULL,
		NULL
	);

	ButtonMenu = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADREMOVEMENU), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		10,
		310,
		200,
		100,
		hwnd,
		(HMENU)IDB_MENU,
		NULL,
		NULL
	);

	ButtonUp = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADUP), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		220,
		310,
		200,
		100,
		hwnd,
		(HMENU)IDB_MOVEUP,
		NULL,
		NULL
	);

	ButtonDown = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADDOWN), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		220,
		410,
		200,
		100,
		hwnd,
		(HMENU)IDB_MOVEDOWN,
		NULL,
		NULL
	);

	ButtonLeft = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADLEFT), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		10,
		410,
		200,
		100,
		hwnd,
		(HMENU)IDB_MOVELEFT,
		NULL,
		NULL
	);

	ButtonRight = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADRIGHT), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		420,
		410,
		200,
		100,
		hwnd,
		(HMENU)IDB_MOVERIGHT,
		NULL,
		NULL
	);

	ButtonFont = CreateWindow(
		TEXT("BUTTON"),  // predefined class
		TEXT(NOTEPADFONTCHNAGE), // button text
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, //style
		420,
		310,
		200,
		100,
		hwnd,
		(HMENU)IDB_FONTCHANGE,
		NULL,
		NULL
	);

}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	TCHAR windowName[255] = TEXT("");
	GetWindowText(hwnd, windowName, 50);
	if (CompareTcharString(windowName, szSearchNotepadWindowName)) {
		hNotepad = hwnd;
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam) {
	TCHAR str[50] = TEXT("");
	HFONT hFont;
	LOGFONT logFont;	
	HDC hdc = GetDC(hNotepad);

	ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 30;
	logFont.lfItalic = TRUE;
	logFont.lfUnderline = TRUE;
	logFont.lfWeight = FW_EXTRABOLD;
	hFont = CreateFontIndirect(&logFont);

	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);
	if (int(lParam)) {
		wsprintf(str, TEXT("This is sample text for font change"));
	}
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)(LPCSTR)str);

	return FALSE;
}

bool CompareTcharString(TCHAR* str, TCHAR* szCharString) {
	int i;
	int lenStr, lenCharString;
	int eqaul = 1;
	for (i = 0; str[i] != '\0'; i++);
	lenStr = i;
	for (i = 0; szCharString[i] != '\0'; i++);
	lenCharString = i;
	if (lenStr != lenCharString) {
		return false;
	}
	for (i = 0; str[i] != '\0'; i++) {
		if (szCharString[i] == (char)((int)str[i])) {
			continue;
		}
		else {
			eqaul = 0;
			break;
		}
	}
	if (!eqaul) {
		return false;
	}
	return true;
}


