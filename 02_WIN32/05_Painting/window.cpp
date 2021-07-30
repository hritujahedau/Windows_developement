// Header files
#include <windows.h>

// Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable declaration

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	TCHAR str[255];

	// code

	// 1. Initialize WndClass structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;


	// 2. Register class
	RegisterClassEx(&wndclass);

	// 3. Create window in memory
	hwnd = CreateWindow(
			szAppName,
			TEXT("HRH: My First Window"),
			WS_OVERLAPPEDWINDOW ,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL, // Parent Handle, null i.e. desktop == HWND_DESKTOP 
			NULL, // Menu Handle 
			hInstance,
			NULL // create window structure pointer (far void *) callled as creation parameter
			);

	// 4. Show window 
	ShowWindow(hwnd, iCmdShow); // SW_SHOWNORMAL 
	UpdateWindow(hwnd);

	// 5. Message loop 

	/*  Different names of message loop -
		1. Windows - Message Loop
		2. XWindows - Event Loop
		3. MAC OS X - Run Loop
		4. RTR/Gaming - Game Loop
		5. Android - Event loop / Loopper
	*/

	/*
	1. HWND 
	2. UINT Message 
	3. WPARAM 
	4. LPARAM 
	5. DWORD time 
	6. Extra information like if mouse clicked then coordinates in point struct 
	*/

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declaration
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	static int iPaintFlag = 0;

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'R':
		case 'r':
			iPaintFlag = 1;			
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'G':
		case 'g':
			iPaintFlag = 2;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'B':
		case 'b':
			iPaintFlag = 3;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'C':
		case 'c':
			iPaintFlag = 4;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'M':
		case 'm':
			iPaintFlag = 5;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'Y':
		case 'y':
			iPaintFlag = 6;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'K':
		case 'k':
			iPaintFlag = 7;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		case 'W':
		case 'w':
			iPaintFlag = 8;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		switch (iPaintFlag)
		{
		case 1:
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 2:
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		case 3:
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		case 4:
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			break;
		case 5:
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			break;
		case 6:
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 7:
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			break;
		case 8:
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			break;
		default:
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			break;
		}

		SelectObject(hdc, hBrush);
		FillRect(hdc, &rc, hBrush);
		DeleteObject(hBrush);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	// default wnd proc 
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
