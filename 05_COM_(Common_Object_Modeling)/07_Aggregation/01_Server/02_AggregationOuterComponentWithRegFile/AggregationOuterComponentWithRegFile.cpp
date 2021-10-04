#include <windows.h>

#include "AggregationOuterComponentWithRegFile.h"
#include "AggregationInnerComponentWithRegFile.h"

class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cRef;
	IUnknown* m_pIUnknownInner;	
	IMultiplication* m_pIMultiplication;
	IDivision* m_pIDivision;
	
public:

	CSumSubtract();
	~CSumSubtract();

	HRESULT __stdcall QueryInterface(REFIID, void** ppv);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);

	HRESULT __stdcall IntializeInnerComponent();
};

class CSumSubtractClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	CSumSubtractClassFactory();
	~CSumSubtractClassFactory();

	HRESULT __stdcall QueryInterface(REFIID, void** ppv);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

long glNumberOfActiveComponent = 0;
long glNumberOfServerLock = 0;

BOOL WINAPI DllMain(HINSTANCE gDll, DWORD dwReason, LPVOID  reserved)
{
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

	}
	return (TRUE);
}

CSumSubtract::CSumSubtract()
{
	MessageBox(NULL, TEXT("CSumSubtract Constructor"), TEXT("Server"), MB_OK);
	m_cRef = 1;
	m_pIUnknownInner = NULL;
	m_pIMultiplication = NULL;
	m_pIDivision = NULL;	
	InterlockedIncrement(&glNumberOfActiveComponent);
}

CSumSubtract::~CSumSubtract()
{
	MessageBox(NULL, TEXT("CSumSubtract Destructor"), TEXT("Server"), MB_OK);

	InterlockedDecrement(&glNumberOfActiveComponent);
	
	if (m_pIUnknownInner)
	{
		// NOTE: We will never come in this if block, just for look like completeion we wrote this block
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}
		
	if (m_pIDivision)
	{
		m_pIDivision->Release();
		m_pIDivision = NULL;
	}
	
	if (m_pIMultiplication)
	{
		m_pIMultiplication->Release();
		m_pIMultiplication = NULL;
	}
	
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void** ppv)
{
	MessageBox(NULL, TEXT("CSumSubtract QueryInterface()"), TEXT("Server"), MB_OK);

	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
		MessageBox(NULL, TEXT("CSumSubtract QueryInterface() for IUnknown"), TEXT("Server"), MB_OK);
	}
	else if (riid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
		MessageBox(NULL, TEXT("CSumSubtract QueryInterface() for ISum"), TEXT("Server"), MB_OK);
	}
	else if (riid == IID_ISubtract)
	{
		*ppv = static_cast<ISubtract*>(this);
		MessageBox(NULL, TEXT("CSumSubtract QueryInterface() for ISubtract"), TEXT("Server"), MB_OK);
	}
	else if (riid == IID_IMultiplication)
	{		
		MessageBox(NULL, TEXT("CSumSubtract QueryInterface() for IMultiplication"), TEXT("Server"), MB_OK);
		return (m_pIUnknownInner->QueryInterface(riid, ppv));
	}
	else if (riid == IID_IDivision)
	{
		MessageBox(NULL, TEXT("CSumSubtract QueryInterface() for IDivision"), TEXT("Server"), MB_OK);
		return (m_pIUnknownInner->QueryInterface(riid, ppv));
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

ULONG CSumSubtract::AddRef()
{
	TCHAR str[255];

	InterlockedIncrement(&m_cRef);

	wsprintf(str, TEXT("CSumSubtract AddRef() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);

	return m_cRef;
}

ULONG CSumSubtract::Release()
{
	TCHAR str[255];

	InterlockedDecrement(&m_cRef);
	
	if (m_cRef == 0)
	{
		wsprintf(str, TEXT("CSumSubtract Release() m_cRef = %d deleting object"), m_cRef);
		MessageBox(NULL, str, TEXT("Server"), MB_OK);
		delete(this);
		return (0);
	}
	wsprintf(str, TEXT("CSumSubtract Release() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);
	return m_cRef;
}

HRESULT CSumSubtract::SumOfTwoIntegers(int iNum1, int iNum2, int* piAns)
{
	*piAns = iNum1 + iNum2;
	return (S_OK);
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int iNum1, int iNum2, int* piAns)
{
	if (iNum1 > iNum2)
	{
		(*piAns) = iNum1 - iNum2;
	}
	else
	{
		(*piAns) = iNum2 - iNum1;
	}
	return (S_OK);
}

HRESULT CSumSubtract::IntializeInnerComponent()
{
	HRESULT hr;

	MessageBox(NULL, TEXT("Entering in CSumSubtract::IntializeInnerComponent()"), TEXT("SERVER"), MB_OK);

	hr = CoCreateInstance(CLSID_MultiplicationDivision, reinterpret_cast<IUnknown*>(this), CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pIUnknownInner);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to get IID_IUnknown Interface"), TEXT("SERVER"), MB_OK);
		return E_FAIL;
	}

	MessageBox(NULL, TEXT("Got IUnknown from inner component"), TEXT("SERVER"), MB_OK);
	
	hr = m_pIUnknownInner->QueryInterface(IID_IMultiplication, (void**)&m_pIMultiplication);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to get IMultiplication Interface"), TEXT("SERVER"), MB_OK);
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
		return E_FAIL;
	}

	MessageBox(NULL, TEXT("Got IUMultiplication from inner component"), TEXT("SERVER"), MB_OK);

	hr = m_pIUnknownInner->QueryInterface(IID_IDivision, (void**)&m_pIDivision);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to get IDivison Interface"), TEXT("SERVER"), MB_OK);

		m_pIMultiplication->Release();
		m_pIMultiplication = NULL;

		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;

		return E_FAIL;
	}
	MessageBox(NULL, TEXT("Got IDivision from inner component"), TEXT("SERVER"), MB_OK);
	
	MessageBox(NULL, TEXT("CSumSubtract::IntializeInnerComponent Successfull"), TEXT("SERVER"), MB_OK);

	return S_OK;
}

// Implementation of CSumSubtractClassFactory function
CSumSubtractClassFactory::CSumSubtractClassFactory()
{
	MessageBox(NULL, TEXT("CSumSubtractClassFactory Constructor"), TEXT("Server"), MB_OK);
	m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory()
{
	// no code
	// why?
}

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	MessageBox(NULL, TEXT("CSumSubtractClassFactory QueryInterface()"), TEXT("Server"), MB_OK);
	if (riid == IID_IUnknown)
	{
		(*ppv) = static_cast<IClassFactory*>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		(*ppv) = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return (E_NOINTERFACE);
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return (S_OK);
}

ULONG CSumSubtractClassFactory::AddRef()
{
	TCHAR str[255];
	
	InterlockedIncrement(&m_cRef);
	
	wsprintf(str, TEXT("CSumSubtractClassFactory AddRef() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);

	return m_cRef;
}

ULONG CSumSubtractClassFactory::Release()
{
	TCHAR str[255];
	
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		wsprintf(str, TEXT("CSumSubtractClassFactory Release() m_cRef = %d deleting object"), m_cRef);
		MessageBox(NULL, str, TEXT("Server"), MB_OK);
		delete(this);
		return 0;
	}

	wsprintf(str, TEXT("CSumSubtractClassFactory Release() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);
	return m_cRef;
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;
	MessageBox(NULL, TEXT("In CSumSubtractClassFactory CreateInstance()"), TEXT("Server"), MB_OK);
	if (pUnkOuter != NULL)
	{
		return (CLASS_E_NOAGGREGATION);
	}

	pCSumSubtract = new CSumSubtract();
	if (pCSumSubtract == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	hr = pCSumSubtract->IntializeInnerComponent();
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to Inititalize Inner Component"), TEXT("SERVER"), MB_OK);
		pCSumSubtract->Release();
		return hr;
	}
	MessageBox(NULL, TEXT("In CSumSubtractClassFactory CreateInstance() After Inititalize Inner Component and now going to call QueryInterface CSumSubtract::QueryInterface"), TEXT("SERVER"), MB_OK);
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	MessageBox(NULL, TEXT("In CSumSubtractClassFactory CreateInstance() After QueryInterface"), TEXT("Server"), MB_OK);
	pCSumSubtract->Release();

	MessageBox(NULL, TEXT("Exiting from CSumSubtractClassFactory CreateInstance()"), TEXT("Server"), MB_OK);

	return hr;
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfServerLock);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLock);
	}
	return (S_OK);
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	// variable dedlaration
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	MessageBox(NULL, TEXT("In DllGetClassObject()"), TEXT("Server"), MB_OK);

	// code
	if (rclsid != CLSID_SumSubtract)
	{
		return (CLASS_E_CLASSNOTAVAILABLE);
	}

	pCSumSubtractClassFactory = new CSumSubtractClassFactory();
	if (pCSumSubtractClassFactory == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
	pCSumSubtractClassFactory->Release();

	MessageBox(NULL, TEXT("Exiting from DllGetClassObject()"), TEXT("Server"), MB_OK);

	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if ((glNumberOfActiveComponent == 0) && (glNumberOfServerLock == 0))
	{
		return (S_OK);
	}	
	return (S_FALSE);
}
