// Header files
#include <windows.h>
#include "MyMathOne.h"

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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	// code
	switch (iMsg)
	{
	case WM_CREATE:
		MakeSquare(10);
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
