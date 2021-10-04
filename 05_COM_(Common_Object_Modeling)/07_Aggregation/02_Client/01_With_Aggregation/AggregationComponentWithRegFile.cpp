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
		wsprintf(str, TEXT("Subtraction of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("SUBTRACTION"), MB_OK);

		hr = gpISubtract->QueryInterface(IID_IMultiplication,(void **)&gpIMultiplication);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get IMultiplication Interface"), TEXT("IMultiplication"), MB_OK);
			DestroyWindow(hwnd);
		}

		gpIMultiplication->MultiplicationOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Multiplication of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("MULTIPLICATION"), MB_OK);

		hr = gpIMultiplication->QueryInterface(IID_IDivision, (void**)&gpIDivision);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("Failed to get IDivision Interface"), TEXT("IDivision"), MB_OK);
			DestroyWindow(hwnd);
		}

		gpIDivision->DivisionOfTwoIntegers(iNum1, iNum2, &iAns);
		wsprintf(str, TEXT("Division of %d and %d is %d"), iNum1, iNum2, iAns);
		MessageBox(hwnd, str, TEXT("DIVISION"), MB_OK);
		
		DestroyWindow(hwnd);

		break;
	case WM_DESTROY:
		if (gpIDivision)
		{
			MessageBox(hwnd, TEXT("Going to destroy IDivision Interface"), TEXT("RELEASE"), MB_OK);
			gpIDivision->Release();
			gpIDivision = NULL;
		}

		if (gpIMultiplication)
		{
			MessageBox(hwnd, TEXT("Going to destroy IMultiplication Interface"), TEXT("RELEASE"), MB_OK);
			gpIMultiplication->Release();
			gpIMultiplication = NULL;
		}

		if (gpISubtract)
		{
			MessageBox(hwnd, TEXT("Going to destroy ISubtract Interface"), TEXT("RELEASE"), MB_OK);
			gpISubtract->Release();
			gpISubtract = NULL;
		}

		if (gpISum)
		{
			MessageBox(hwnd, TEXT("Going to destroy ISum Interface"), TEXT("RELEASE"), MB_OK);
			gpISum->Release();
			gpISum = NULL;
		}

		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

