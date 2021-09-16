// Header files
#include <windows.h>
#include <tchar.h>
#include "MonikerDllServer.h"

// Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable declaration
IOddNumber* gpIOddNumber = NULL;
IEvenNumber* gpIEvenNumber = NULL;
INumberFactory* gpINumberFactory = NULL;

IUnknown* gpIUnknownFromIOddNumber = NULL;
IUnknown* gpIUnknownFromIEvenNumber = NULL;

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	TCHAR str[255];
	HRESULT hr = NULL;

	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library Can Not Be Initialized\nProgramWill Now Exit"), TEXT("Error"), MB_OK);
		exit(0);
	}

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

	CoUninitialize();

	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	IBindCtx* pIBindCtx = NULL;
	IMoniker* pIMoniker = NULL;
	ULONG uEaten;
	HRESULT hr;
	LPOLESTR szCLSID = NULL;
	wchar_t  wszTemp[255], * ptr;
	int iFirstOddNumber;
	int iFirstEvenNumber;
	TCHAR str[255], strTemp[255];
	OLECHAR wszCLSID[] = OLESTR("clsid:6132B8CA-F9B1-4400-81FB-420425CF6A6E:");

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		// create a BindContext
		hr = CreateBindCtx(0, &pIBindCtx);
		if (!SUCCEEDED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IBindCtx Interface Pointer"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		StringFromCLSID(CLSID_Number, &szCLSID);

		wcscpy_s(wszTemp, szCLSID);
		ptr = wcschr(wszTemp, '{');
		ptr = ptr + 1; // to remove first openong '{' from CSILD string
		wcscpy_s(wszTemp, ptr);

		//wszTemp[(int)wcslen(wszTemp) - 1] = '\0';
		//wcstombs(strTemp, wszTemp, (int)wcslen(wszTemp));
//		wsprintf(wszCLSID, TEXT("clsid:%s"), wszTemp);
		//wsprintf(strTemp, TEXT("clsid:%s:"), wszTemp);
		//MessageBox(hwnd, strTemp, TEXT("CLSID"), MB_OK);
		//mbstowcs(wszCLSID, strTemp, _tcslen(strTemp));

		// Get Moniker for this CLSID
		hr = MkParseDisplayName(pIBindCtx, wszCLSID, &uEaten, &pIMoniker);

		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IMoniker Interface Pointer"), TEXT("Error"), MB_OK);
			pIBindCtx->Release();
			pIBindCtx = NULL;
			DestroyWindow(hwnd);
		}

		hr = pIMoniker->BindToObject(pIBindCtx, NULL, IID_INumberFactory, (void**)&gpINumberFactory);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IOddNumberFactory Interface Pointer"), TEXT("Error"), MB_OK);
			pIMoniker->Release();
			pIMoniker = NULL;
			pIBindCtx->Release();
			pIBindCtx = NULL;
			DestroyWindow(hwnd);
		}

		pIMoniker->Release();
		pIMoniker = NULL;

		pIBindCtx->Release();
		pIBindCtx = NULL;

		iFirstOddNumber = 57;
		iFirstEvenNumber = 56;
		hr = gpINumberFactory->SetFirstOddEvenNumber(iFirstOddNumber, iFirstEvenNumber, &gpIOddNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IOddNumber Interface Pointer"), TEXT("Error"), MB_OK);
			gpINumberFactory->Release();
			gpINumberFactory = NULL;
			DestroyWindow(hwnd);
		}

		if (gpINumberFactory)
		{
			gpINumberFactory->Release();
			gpINumberFactory = NULL;
		}

		gpIOddNumber->QueryInterface(IID_IEvenNumber, (void**)&gpIEvenNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IEvenumber Interface Pointer"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}
		
		// Get IUnknown from both interfaces
		gpIEvenNumber->QueryInterface(IID_IUnknown, (void**)&gpIUnknownFromIEvenNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IID_IUnknown Interface Pointer from IEvenInterface"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		gpIOddNumber->QueryInterface(IID_IUnknown, (void**)&gpIUnknownFromIOddNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get IID_IUnknown Interface Pointer from IOddInterface"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		if (gpIUnknownFromIOddNumber == gpIUnknownFromIEvenNumber)
		{
			MessageBox(hwnd, TEXT("Both IUnknown From Same Component"), TEXT("Success"), MB_OK);
		}
		else
		{
			MessageBox(hwnd, TEXT("Both IUnknown From NOT Same Component"), TEXT("Failed"), MB_OK);
		}

		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (gpIUnknownFromIOddNumber)
		{
			gpIUnknownFromIOddNumber->Release();
			gpIUnknownFromIOddNumber = NULL;
		}

		if (gpIUnknownFromIEvenNumber)
		{
			gpIUnknownFromIEvenNumber->Release();
			gpIUnknownFromIEvenNumber = NULL;
		}

		if (gpIEvenNumber)
		{
			gpIEvenNumber->Release();
			gpIEvenNumber = NULL;
		}

		if (gpIOddNumber)
		{
			gpIOddNumber->Release();
			gpIOddNumber = NULL;
		}
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	// default wnd proc 
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
