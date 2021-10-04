// Header files
#include <windows.h>
#include <tchar.h>
#include "MonikerDllServer.h"

// Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable declaration
IOddNumber* pIOddNumber = NULL;
IOddNumberFactory* pIOddNumberFactory = NULL;

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
	int iFirstOddNumber, iNextOddNumber;
	TCHAR str[255], strTemp[255];
	OLECHAR wszCLSID[] = OLESTR("clsid:3BA98BA8-4A91-41A5-8446-98995544103F:");

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

		StringFromCLSID(CLSID_OddNumber, &szCLSID);

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

		hr = pIMoniker->BindToObject(pIBindCtx, NULL, IID_IOddNumberFactory, (void**)&pIOddNumberFactory);
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
		hr = pIOddNumberFactory->SetFirstOddNumber(iFirstOddNumber, &pIOddNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to get iOddNumber Interface Pointer"), TEXT("Error"), MB_OK);
			pIOddNumberFactory->Release();
			pIOddNumberFactory = NULL;
			DestroyWindow(hwnd);
		}

		pIOddNumberFactory->Release();
		pIOddNumberFactory = NULL;

		pIOddNumber->GetNextOddNumber(&iNextOddNumber);

		pIOddNumber->Release();
		pIOddNumber = NULL;

		wsprintf(str, TEXT("The Next Odd Number From %2d Is %2d"), iFirstOddNumber, iNextOddNumber);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		DestroyWindow(hwnd);
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
