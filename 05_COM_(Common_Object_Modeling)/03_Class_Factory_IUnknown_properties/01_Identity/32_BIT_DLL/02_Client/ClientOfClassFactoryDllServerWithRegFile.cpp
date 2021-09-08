// header files
#include <windows.h>

#include "ClassFactoryServerWithDefAndRegFile.h"

// globle function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// globle variable declaration
ISum* gpISum = NULL;
ISubtract* gpISubtract = NULL;

IUnknown* gpIUknownFromISum = NULL;
IUnknown* gpIUnknownFromISubtract = NULL;

// entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	MSG msg;
	TCHAR AppName[] = TEXT("ComClient");
	HWND hwnd;
	HRESULT hr;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to initialize Com"), TEXT("COM"), MB_OK);
		exit(0);
	}
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;
	wndclass.lpfnWndProc = WndProc;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(
		AppName,
		TEXT("Clinet of Class Factory"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();
	return (int)(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iNum1, iNum2, iAns;
	TCHAR str[255], str2[255];
	HRESULT hr;
	OLECHAR* oleStr;
	CLSID getCLSID;

	switch (uMsg)
	{
	case WM_CREATE:

		hr = ProgIDFromCLSID(CLSID_SumSubtract, &oleStr);
		wcstombs(str, oleStr, 255);
		wsprintf(str2, TEXT("Prog ID is %s"), str);
		MessageBox(NULL, str, TEXT("ProgID From CLSID"), MB_OK);

		hr = CLSIDFromProgID(oleStr, &getCLSID);

		if (getCLSID == CLSID_SumSubtract)
		{
			MessageBox(NULL, TEXT("Got the clsid from ProgID"), TEXT("Success"), MB_OK);
		}
		else
		{
			MessageBox(NULL, TEXT("Didn't get the clsid from ProgID"), TEXT("Failed"), MB_OK);
		}

		hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&gpISum);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("\nFailed to get ISum"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		iNum1 = 45;
		iNum2 = 55;
		iAns = 0;
		hr = gpISum->SumOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Sum of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("SUM"), MB_OK);

		hr = gpISum->QueryInterface(IID_ISubtract, (void **)&gpISubtract);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to Get ISubtract"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}

		iNum1 = 100;
		iNum2 = 25;
		iAns = 0;
		gpISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Subtrction of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("SUBTRACTION"), MB_OK);
		
		gpISum->QueryInterface(IID_IUnknown, (void**)&gpIUknownFromISum);
		gpISubtract->QueryInterface(IID_IUnknown, (void**)&gpIUnknownFromISubtract);

		if (gpIUknownFromISum == gpIUnknownFromISubtract)
		{
			MessageBox(hwnd, TEXT("Both IUknown are from same component"), TEXT("Success"), MB_OK);
		}
		else
		{
			MessageBox(hwnd, TEXT("Both IUknown are from NOT same component"), TEXT("Failed"), MB_OK);
		}

		// Release Interfaces
		gpISum->Release();
		gpISum = NULL;

		gpIUknownFromISum->Release();
		gpIUknownFromISum = NULL;

		gpIUnknownFromISubtract->Release();
		gpIUnknownFromISubtract = NULL;

		gpISubtract->Release();
		gpISubtract = NULL;
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

