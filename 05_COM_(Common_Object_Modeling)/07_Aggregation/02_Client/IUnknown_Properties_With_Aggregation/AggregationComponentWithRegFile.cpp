// header files
#include <windows.h>

#include "CombinedAggregationInnerOuterComponentWithRegFile.h"

// globle function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// globle variable declaration
ISum* gpISum = NULL;
ISubtract* gpISubtract = NULL;
IMultiplication* gpIMultiplication = NULL;
IDivision* gpIDivision = NULL;
IUnknown* gpIUnknownOuter = NULL;
IUnknown* gpIUnknownInner = NULL;

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
		hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void**)&gpISum);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("\nFailed to get ISum"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		hr = gpISum->QueryInterface(IID_ISubtract, (void **)&gpISubtract);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to Get ISubtract"), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
		}

		hr = gpISubtract->QueryInterface(IID_IMultiplication,(void **)&gpIMultiplication);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get IMultiplication Interface"), TEXT("IMultiplication"), MB_OK);
			DestroyWindow(hwnd);
		}

		hr = gpIMultiplication->QueryInterface(IID_IDivision, (void**)&gpIDivision);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get IDivision Interface"), TEXT("IDivision"), MB_OK);
			DestroyWindow(hwnd);
		}
				
		hr = gpISum->QueryInterface(IID_IUnknown, (void**)&gpIUnknownOuter);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get outer IUnknown Interface from ISum"), TEXT("IUnknown"), MB_OK);
			DestroyWindow(hwnd);
		}

		hr = gpIMultiplication->QueryInterface(IID_IUnknown, (void**)&gpIUnknownInner);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get inner IUnknown Interface from IMultiplication"), TEXT("IUnknown"), MB_OK);
			DestroyWindow(hwnd);
		}
		
		if (gpIUnknownInner == gpIUnknownOuter)
		{
			MessageBox(hwnd, TEXT("Both IUnknown from same component"), TEXT("SAME"), MB_OK);
			DestroyWindow(hwnd);
		}
		else
		{
			MessageBox(hwnd, TEXT("Both IUnknown are NOT from same component"), TEXT("NOT SAME"),MB_OK);
			DestroyWindow(hwnd);
		}

		DestroyWindow(hwnd);

		break;
	case WM_DESTROY:
		if (gpIUnknownOuter)
		{
			gpIUnknownOuter->Release();
			gpIUnknownOuter = NULL;
		}

		if (gpIUnknownInner)
		{
			gpIUnknownInner->Release();
			gpIUnknownInner = NULL;
		}

		if (gpIDivision)
		{
			gpIDivision->Release();
			gpIDivision = NULL;
		}

		if (gpIMultiplication)
		{
			gpIMultiplication->Release();
			gpIMultiplication = NULL;
		}

		if (gpISubtract)
		{
			gpISubtract->Release();
			gpISubtract = NULL;
		}

		if (gpISum)
		{
			gpISum->Release();
			gpISum = NULL;
		}

		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

