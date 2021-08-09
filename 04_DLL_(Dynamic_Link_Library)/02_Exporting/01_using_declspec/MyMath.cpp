
#include <windows.h>

// entry function
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID *lpReserved)
{
	// code	
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:		
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) int makeCube(int num)
{
	// code
	int cube = num * num * num;
	return cube;
}
