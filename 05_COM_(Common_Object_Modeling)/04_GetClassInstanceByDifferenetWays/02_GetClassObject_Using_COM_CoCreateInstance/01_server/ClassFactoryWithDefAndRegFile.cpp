#include <windows.h>

#include "ClassFactoryWithDefAndRegFile.h"

class CSumSubtract : public ISum, ISubtract
{
private:
	long m_cRef;
public:

	CSumSubtract();
	~CSumSubtract();

	// IUnkonwn method
	HRESULT	__stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall	AddRef();
	ULONG __stdcall	Release();

	// ISum methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
	
	// ISubtract methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};

class CSumSubtractClassFactory :public IClassFactory
{
private:
	long m_cRef;
public:

	CSumSubtractClassFactory();
	~CSumSubtractClassFactory();
	
	// IUnknown methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall	AddRef();
	ULONG __stdcall	 Release();

	// IClassFacory methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);

};

long glNumberOfActiveComponents = 0;
long glNumberOfServerLock = 0;

BOOL WINAPI DllMain(HRESULT dllInstance, DWORD dwReason, LPVOID reserved)
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
	}
	return (TRUE);
}

// CSumSubtract
CSumSubtract::CSumSubtract()
{
	// code
	m_cRef = 0;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract()
{
	// code
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISum)
	{
		*ppv = static_cast<ISum*>(this);
	}
	else if (riid == IID_ISubtract)
	{
		*ppv = static_cast<ISubtract*>(this);
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
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CSumSubtract::Release()
{
	InterlockedDecrement(&m_cRef);
	if(m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CSumSubtract::SumOfTwoIntegers(int iNum1, int iNum2, int* iAns)
{
	*iAns = iNum1 + iNum2;
	return S_OK;
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int iNum1, int iNum2, int* iAns)
{
	*iAns = iNum1 - iNum2;
	return S_OK;
}

// CSumSubtractClassFactory
CSumSubtractClassFactory::CSumSubtractClassFactory()
{
	m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory()
{
	// no code 
	// why?
}

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		*ppv = static_cast<IClassFactory*>(this);
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
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CSumSubtractClassFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr;

	if (pUnkOuter != NULL)
	{
		return (CLASS_E_NOAGGREGATION);
	}
	pCSumSubtract = new CSumSubtract();
	if (pCSumSubtract == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	hr = pCSumSubtract->QueryInterface(riid, ppv);
	pCSumSubtract->Release();
	return hr;
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if (fLock == TRUE)
	{
		InterlockedIncrement(&glNumberOfServerLock);
	}
	else
	{
		InterlockedDecrement(&glNumberOfServerLock);
	}
	return S_OK;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID refclsid, REFIID riid, void** ppv)
{
	// variable declarations
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;

	// code
	if (refclsid != CLSID_SumSubtract)
	{
		return (CLASS_E_CLASSNOTAVAILABLE);
	}

	pCSumSubtractClassFactory = new CSumSubtractClassFactory();
	if (pCSumSubtractClassFactory == NULL)
	{
		return (E_OUTOFMEMORY);
	}
	hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
	pCSumSubtractClassFactory->AddRef();
	return (hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfServerLock == 0))
	{
		return (S_OK);
	}
	else
	{
		return (S_FALSE);
	}
}
