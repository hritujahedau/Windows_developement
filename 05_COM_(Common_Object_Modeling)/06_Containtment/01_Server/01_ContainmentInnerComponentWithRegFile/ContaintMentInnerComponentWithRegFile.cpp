#include <windows.h>
#include "ContaintMentInnerComponentWithRegFile.h"

// class declarations
class CMultiplicationDivision : public IMultiplication, IDivision
{
private:
	long m_cRef;
public:
	// constructor declarations
	CMultiplicationDivision();

	// destructor declarations
	~CMultiplicationDivision();

	// IUnknown specifc methods (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// IMultiplication specific methods (inherited)
	HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);

	// IDivision specific methods ( inherited)
	HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);

};

class CMultiplicationDivisionClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	//CMultiplicationDivisionClassFactory constrctor declarations
	CMultiplicationDivisionClassFactory();

	// CMultiplicationDivisionClassFactory destructor declarations
	~CMultiplicationDivisionClassFactory();

	// IUnknown specific method (inherited)
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	// IClassFactory specific method (inherited)
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// global variable decalarations
long glNumberOfActiveComponents = 0;
long glNumberOfActiveServerLocks = 0;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID reserved)
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
	default:
		break;
	}
	return TRUE;
}

CMultiplicationDivision::CMultiplicationDivision()
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CMultiplicationDivision::~CMultiplicationDivision()
{
	InterlockedDecrement(&m_cRef);
}

HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		(*ppv) = static_cast<IMultiplication*>(this);
	}
	else if (riid == IID_IMultiplication)
	{
		(*ppv) = static_cast<IMultiplication*>(this);
	}
	else if (riid == IID_IDivision)
	{
		(*ppv) = static_cast<IDivision*>(this);
	}
	else
	{
		(*ppv) = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CMultiplicationDivision::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CMultiplicationDivision::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int iNum1, int iNum2, int* piMul)
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::MultiplicationOfTwoIntegers()"), TEXT("SERVER"), MB_OK);
	*piMul = iNum1 * iNum2;
	MessageBox(NULL, TEXT("Exiting CMultiplicationDivision::MultiplicationOfTwoIntegers()"), TEXT("SERVER"), MB_OK);
	return S_OK;
}

HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int iNum1, int iNum2, int* piDiv)
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::DivisionOfTwoIntegers()"), TEXT("SERVER"), MB_OK);
	*piDiv = iNum1 / iNum2;
	MessageBox(NULL, TEXT("Exiting CMultiplicationDivision::DivisionOfTwoIntegers()"), TEXT("SERVER"), MB_OK);
	return S_OK;
}

CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory()
{
	m_cRef = 1;
}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory()
{

}

HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void** ppv)
{
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
		(*ppv) = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CMultiplicationDivisionClassFactory::AddRef()
{
	InterlockedIncrement(&m_cRef);
	return m_cRef;
}

ULONG CMultiplicationDivisionClassFactory::Release()
{
	InterlockedDecrement(&m_cRef);
	if (m_cRef)
	{
		delete(this);
		return 0;
	}
	return m_cRef;
}

HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown* pOutIUnknown, REFIID riid, void** ppv)
{
	CMultiplicationDivision* pCMultiplicationDivision = NULL;
	HRESULT hr = S_FALSE;

	if (pOutIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCMultiplicationDivision = new CMultiplicationDivision();
	if (pCMultiplicationDivision == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCMultiplicationDivision->QueryInterface(riid, ppv);
	pCMultiplicationDivision->Release();
	return hr;
}

HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&glNumberOfActiveServerLocks);
	}
	else
	{
		InterlockedDecrement(&glNumberOfActiveServerLocks);
	}
	return S_OK;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	CMultiplicationDivisionClassFactory* pCmultiplicationDivisionClassFactory = NULL;
	HRESULT hr;

	if (rclsid != CLSID_MultiplicationDivision)
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	pCmultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory();
	if (pCmultiplicationDivisionClassFactory == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCmultiplicationDivisionClassFactory->QueryInterface(riid, ppv);
	pCmultiplicationDivisionClassFactory->Release();
	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if ((glNumberOfActiveComponents == 0) && (glNumberOfActiveServerLocks == 0))
	{
		return S_OK;
	}
	return S_FALSE;
}
