// header files
#include <windows.h>
#include "ClassFactoryWithDefAndRegFile.h"

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
ISum * gpISum = NULL;
ISum* gpISumAgain = NULL;

// entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdline, int nCmdshow)
{
	WNDCLASSEX wndclass;
	TCHAR AppName[] = TEXT("Mywindow");
	HWND hwnd;
	MSG msg;
	HRESULT hr;

	hr = CoInitialize(NULL);

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);

	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(
		AppName,
		TEXT("MyWindow"),
		WS_OVERLAPPED | WS_CAPTION |WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	UpdateWindow(hwnd);
	ShowWindow(hwnd, nCmdshow);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();

	return (int)(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int iNum1, iNum2, iAns;
	HRESULT hr;
	TCHAR str[255];

	switch(iMsg)
	{
		case WM_CREATE:

			hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&gpISum);
			if(FAILED(hr))
			{
				MessageBox(NULL, TEXT("Failed t get ISum"), TEXT("ERROR"), MB_OK);
				DestroyWindow(hwnd);
			}

			gpISum->QueryInterface(IID_ISum, (void**)&gpISumAgain);

			if (gpISum == gpISumAgain)
			{
				MessageBox(hwnd, TEXT("Relexity True"), TEXT("True"), MB_OK);
			}
			else
			{
				MessageBox(hwnd, TEXT("Relexity False"), TEXT("False"), MB_OK);
			}
						
			gpISum->Release();
			gpISum = NULL;

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
