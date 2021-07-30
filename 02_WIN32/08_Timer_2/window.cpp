// Header files
#include <windows.h>

#define ID_TIMER1 101

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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
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
	ShowWindow(hwnd, SW_MAXIMIZE); // SW_SHOWNORMAL 
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
	RECT rc, rc_temp;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	static int iPaintFlag = 0;
	int slice;
	int color_array[][3] = {
		{255, 0, 0} ,
		{0, 255, 0} ,
		{0, 0, 255} ,
		{0, 255, 255} ,
		{255, 0, 255} ,
		{255, 255, 0} ,
		{0, 0, 0} ,
		{255, 255, 255}
	};

	static bool isHorizontal = true;

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER1, 1000, NULL);
		break;
	case WM_TIMER:		
		KillTimer(hwnd, ID_TIMER1);
		iPaintFlag = iPaintFlag + 1;
		if (iPaintFlag > 8)
		{
			iPaintFlag = 0;
			isHorizontal = !isHorizontal;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		SetTimer(hwnd, ID_TIMER1, 1000, NULL);
		break;
	case WM_PAINT:		
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		GetClientRect(hwnd, &rc_temp);
		if (isHorizontal == true)
		{
			slice = rc.right / 8;
			for (int i = 0; i < iPaintFlag; i++)
			{
				hBrush = CreateSolidBrush(RGB(color_array[i][0], color_array[i][1], color_array[i][2]));
				rc_temp.left = i * slice;
				rc_temp.right = rc_temp.left + slice;
				SelectObject(hdc, hBrush);
				FillRect(hdc, &rc_temp, hBrush);
			}
		}
		else
		{
			slice = rc.bottom / 8;
			for (int i = 0; i < iPaintFlag; i++)
			{
				hBrush = CreateSolidBrush(RGB(color_array[i][0], color_array[i][1], color_array[i][2]));
				rc_temp.top = i * slice;
				rc_temp.bottom = rc_temp.top + slice;
				SelectObject(hdc, hBrush);
				FillRect(hdc, &rc_temp, hBrush);
			}
		}
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
