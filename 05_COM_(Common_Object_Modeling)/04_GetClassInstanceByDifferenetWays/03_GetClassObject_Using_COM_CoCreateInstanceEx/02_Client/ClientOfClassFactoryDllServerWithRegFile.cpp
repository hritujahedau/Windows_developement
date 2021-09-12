// header files
#include <windows.h>

#include "ClassFactoryServerWithDefAndRegFile.h"

// globle function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// globle variable declaration
MULTI_QI gMultiQi[] =
{
	{ &IID_ISum, 0, 0 },
	{ &IID_ISubtract, 0, 0}
};

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
	TCHAR str[255];
	HRESULT hr;

	switch (uMsg)
	{
	case WM_CREATE:
		MessageBox(hwnd, TEXT("In WM_CREATE"), TEXT("Welcome"), MB_OK);

		hr = CoCreateInstanceEx(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, NULL, 2, gMultiQi);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed CoCreateInstanceEx()"), TEXT("ERROR"), MB_OK);
			//DestroyWindow(hwnd);
		}

		if (FAILED(gMultiQi[0].hr))
		{
			MessageBox(hwnd, TEXT("Failed to ISum Interface"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}

		if (FAILED(gMultiQi[1].hr))
		{
			MessageBox(hwnd, TEXT("Failed to ISubtract Interface"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}

		iNum1 = 20;
		iNum2 = 10;
		iAns = 0;
		reinterpret_cast<ISum*>(gMultiQi[0].pItf)->SumOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Sum of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("Sum"), MB_OK);

		gMultiQi[0].pItf->Release();
		gMultiQi[0].pItf = NULL;

		// ISubtract
		iNum1 = 20;
		iNum2 = 10;
		iAns = 0;
		reinterpret_cast<ISubtract*>(gMultiQi[1].pItf)->SubtractionOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Sub of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("Sub"), MB_OK);
			

		gMultiQi[1].pItf->Release();
		gMultiQi[1].pItf = NULL;

		//DestroyWindow(hwnd);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

