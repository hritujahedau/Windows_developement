#include <windows.h>
#include "MonikerDllServer.h"

class CNumbers : public IOddNumber, IEvenNumber
{
private:
	long m_cRef;
	int m_iFirstOddNumber;
	int m_iFirstEvenNumber;

public:
	CNumbers(int , int);
	~CNumbers();

	// IUnknown specific method
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// this class's custom method
	BOOL __stdcall IsOdd(int);
	BOOL __stdcall IsEven(int);

	// IOddNumber specific method declarations (inherited)
	HRESULT __stdcall GetNextOddNumber(int*);
	HRESULT __stdcall GetNextEvenNumber(int*);
};

class CNumberFactory : public INumberFactory
{
private:
	long m_cRef;
public:
	CNumberFactory();
	~CNumberFactory();

	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall	AddRef();
	ULONG __stdcall Release();

	HRESULT __stdcall SetFirstOddNumber(int, IOddNumber**);
	HRESULT __stdcall SetFirstEvenNumber(int, IEvenNumber**);
	HRESULT __stdcall SetFirstOddEvenNumber(int, int, IOddNumber**);
	HRESULT __stdcall SetFirstOddEvenNumber(int, int, IEvenNumber**);

};

long glNumberOfActiveComponets = 0;
long glNumberOfServerLocks = 0;
HMODULE ghDllModule = NULL; // DLL instance handle

// Dll Main
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ghDllModule = hDll;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

CNumbers::CNumbers(int iFirstOddNumber, int iFirstEvenNumber)
{
	m_cRef = 1;
	m_iFirstOddNumber = iFirstOddNumber;
	m_iFirstEvenNumber = iFirstEvenNumber;
	InterlockedIncrement(&glNumberOfActiveComponets);
}

CNumbers::~CNumbers()
{
	InterlockedDecrement(&glNumberOfActiveComponets);
}

HRESULT CNumbers::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IOddNumber*>(this);
	}
	else if (riid == IID_IOddNumber)
	{
		*ppv = static_cast<IOddNumber*>(this);
	}
	else if (riid == IID_IEvenNumber)
	{
		*ppv = static_cast<IEvenNumber*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CNumbers::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CNumbers::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

BOOL CNumbers::IsOdd(int iFirstOddNumber)
{
	if (iFirstOddNumber != 0 && iFirstOddNumber % 2 != 0)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CNumbers::IsEven(int iFirstEvenNumber)
{
	if (iFirstEvenNumber == 0 || iFirstEvenNumber % 2 == 0)
	{
		return TRUE;
	}
	return FALSE;
}

HRESULT CNumbers::GetNextOddNumber(int* pNextOddNumber)
{
	BOOL bResult;
	bResult = IsOdd(m_iFirstOddNumber);

	if (bResult == TRUE)
	{
		*pNextOddNumber = m_iFirstOddNumber + 2;
	}
	else
	{
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CNumbers::GetNextEvenNumber(int* pNextEvenNumber)
{
	BOOL bResult;
	bResult = IsEven(m_iFirstEvenNumber);

	if (bResult == TRUE)
	{
		*pNextEvenNumber = m_iFirstEvenNumber + 2;
	}
	else
	{
		return S_FALSE;
	}
	return S_OK;
}

CNumberFactory::CNumberFactory()
{
	m_cRef = 1;
}

CNumberFactory::~CNumberFactory()
{

}

HRESULT CNumberFactory::QueryInterface(REFIID riid, void** ppv)
{
	//MessageBox(NULL, TEXT("Entering in COddNumberFactory::QueryInterface() "), TEXT("SERVER"), MB_OK);

	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<INumberFactory*>(this);
	}
	else if(riid == IID_INumberFactory)
	{
		*ppv = static_cast<INumberFactory*>(this);
		//MessageBox(NULL, TEXT("Got IOddNumberFactory "), TEXT("SERVER"), MB_OK);
	}
	else
	{
		*ppv = NULL;
		//MessageBox(NULL, TEXT("In COddNumberFactory::QueryInterface() \n Interface IID_IOddNumberFactory Not Found!!!"), TEXT("SERVER"), MB_OK);
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(this)->AddRef();
	return S_OK;
}

ULONG CNumberFactory::AddRef()
{
	//MessageBox(NULL, TEXT("In COddNumberFactory::AddRef() "), TEXT("SERVER"), MB_OK);
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CNumberFactory::Release()
{
	//MessageBox(NULL, TEXT("In COddNumberFactory::Release() "), TEXT("SERVER"), MB_OK);
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CNumberFactory::SetFirstOddNumber(int iFirstOddNumber, IOddNumber** ppIOddNumber)
{
	HRESULT hr;
	CNumbers* pCNumbers = new CNumbers(iFirstOddNumber, 0);

	if (pCNumbers == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCNumbers->QueryInterface(IID_IOddNumber, (void**)ppIOddNumber);
	pCNumbers->Release();
	return hr;
}

HRESULT CNumberFactory::SetFirstEvenNumber(int iFirstEvenNumber, IEvenNumber** ppIEvenNumber)
{
	HRESULT hr;
	CNumbers* pCNumbers = new CNumbers(0, iFirstEvenNumber);

	if (pCNumbers == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCNumbers->QueryInterface(IID_IEvenNumber, (void**)ppIEvenNumber);
	pCNumbers->Release();
	return hr;
}

HRESULT CNumberFactory::SetFirstOddEvenNumber(int iFirstOddNumber, int iFirstEvenNumber, IOddNumber** ppIOddNumber)
{
	HRESULT hr;
	CNumbers* pCNumbers = new CNumbers(iFirstOddNumber, iFirstEvenNumber);

	if (pCNumbers == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCNumbers->QueryInterface(IID_IOddNumber, (void**)ppIOddNumber);
	pCNumbers->Release();
	return hr;
}

HRESULT CNumberFactory::SetFirstOddEvenNumber(int iFirstOddNumber, int iFirstEvenNumber, IEvenNumber** ppIEvenNumber)
{
	HRESULT hr;
	CNumbers* pCNumbers = new CNumbers(iFirstOddNumber, iFirstEvenNumber);

	if (pCNumbers == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCNumbers->QueryInterface(IID_IEvenNumber, (void**)ppIEvenNumber);
	pCNumbers->Release();
	return hr;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	CNumberFactory* pCNumberFactory = NULL;
	HRESULT hr;

	//MessageBox(NULL, TEXT("Entering In DllGetClassObject"), TEXT("SERVER"), MB_OK);

	if (rclsid != CLSID_Number)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	//MessageBox(NULL, TEXT("We have CLSID_OddNumber "), TEXT("SERVER"), MB_OK);

	pCNumberFactory = new CNumberFactory();

	if (pCNumberFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCNumberFactory->QueryInterface(riid, ppv);
	pCNumberFactory->Release();
	//MessageBox(NULL, TEXT("Exiting From DllGetClassObject"), TEXT("SERVER"), MB_OK);
	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if ((glNumberOfActiveComponets == 0) && (glNumberOfActiveComponets == 0))
	{
		return S_OK;
	}
	return S_FALSE;
}

// extern "C" HRESULT __stcall
STDAPI DllRegisterServer()
{
	HKEY hCLSIDKey = NULL, hInProcSvrKey = NULL;
	LONG lRet;
	TCHAR szModulePath[MAX_PATH];
	TCHAR szClassDescription[] = TEXT("Simple Moniker Class");
	TCHAR szThreadingModel[] = TEXT("Apartment");

	__try
	{
		// Create a key under CLSID for our COM Server
		lRet = RegCreateKeyEx
				(
				HKEY_CLASSES_ROOT, 
				TEXT("CLSID\\{6132B8CA-F9B1-4400-81FB-420425CF6A6E}"), 
				0, 
				NULL, 
				REG_OPTION_NON_VOLATILE, 
				KEY_SET_VALUE | KEY_CREATE_SUB_KEY , 
				NULL, 
				&hCLSIDKey, 
				NULL
				);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		// The Default value of the key is human-readabe decription of the coclass

		lRet = RegSetValueEx
			(
			hCLSIDKey, 
			NULL, 
			0, 
			REG_SZ, 
			(const BYTE*)szClassDescription, 
			sizeof(szClassDescription)
			);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		// create the InProcServer32 key , which holds info about our coclass
		lRet = RegCreateKeyEx(
			hCLSIDKey,
			TEXT("InProcServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE ,
			NULL,
			&hInProcSvrKey,
			NULL
		);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		// The default value of the InProcServer32 key holds the full path to our dll
		GetModuleFileName(ghDllModule, szModulePath, MAX_PATH);

		lRet = RegSetValueEx(
			hInProcSvrKey,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1)
		);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}

		// The threadingModel value tells COM it should handle thread in our DLL
		lRet = RegSetValueEx
				(
					hInProcSvrKey,
					TEXT("ThreadingModel"),
					0,
					REG_SZ,
					(const BYTE *)szThreadingModel,
					sizeof(szThreadingModel)
				);

		if (ERROR_SUCCESS != lRet)
		{
			return HRESULT_FROM_WIN32(lRet);
		}
	}

	__finally
	{
		if (NULL != hCLSIDKey)
		{
			RegCloseKey(hCLSIDKey);
		}

		if (NULL != hInProcSvrKey)
		{
			RegCloseKey(hInProcSvrKey);
		}
	}
	return S_OK;
}

STDAPI DllUnregisterServer()
{
	RegDeleteKey(
		HKEY_CLASSES_ROOT,
		TEXT("CLSID\\{6132B8CA-F9B1-4400-81FB-420425CF6A6E}\\InProcServer32")
		);
	RegDeleteKey(
		HKEY_CLASSES_ROOT,
		TEXT("CLSID\\{6132B8CA-F9B1-4400-81FB-420425CF6A6E}")
	);
	return S_OK;
}

