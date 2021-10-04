#include <windows.h>
#include "AggregationInnerComponentWithRegFile.h"

interface INoAggregationIUnknown
{
	virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) = 0;
	virtual ULONG __stdcall AddRef_NoAggregation() = 0;
	virtual ULONG __stdcall Release_NoAggregation() = 0;
};

// class declarations
class CMultiplicationDivision : public INoAggregationIUnknown, IMultiplication, IDivision
{
private:
	long m_cRef;
	IUnknown* m_pIUnknownOuter;
public:
	// constructor declarations
	CMultiplicationDivision(IUnknown *);

	// destructor declarations
	~CMultiplicationDivision();

	// INoAggregationIUnknown specifc methods
	HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**);
	ULONG __stdcall AddRef_NoAggregation();
	ULONG __stdcall Release_NoAggregation();

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

CMultiplicationDivision::CMultiplicationDivision(IUnknown *pIUnknownOuter)
{
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponents);
	if (pIUnknownOuter != NULL)
	{
		m_pIUnknownOuter = pIUnknownOuter;
	}
	else
	{
		m_pIUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INoAggregationIUnknown*>(this));
	}
}

CMultiplicationDivision::~CMultiplicationDivision()
{
	InterlockedDecrement(&m_cRef);
}

HRESULT CMultiplicationDivision::QueryInterface_NoAggregation(REFIID riid, void** ppv)
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface_NoAggregation()"), TEXT("SERVER"), MB_OK);
	if (riid == IID_IUnknown)
	{
		(*ppv) = static_cast<INoAggregationIUnknown*>(this);
		MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface_NoAggregation() for INoAggregationIUnknown"), TEXT("SERVER"), MB_OK);
	}
	else if (riid == IID_IMultiplication)
	{
		(*ppv) = static_cast<IMultiplication*>(this);
		MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface_NoAggregation() for IMultiplication"), TEXT("SERVER"), MB_OK);
	}
	else if (riid == IID_IDivision)
	{
		(*ppv) = static_cast<IDivision*>(this);
		MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface_NoAggregation() for IDivison"), TEXT("SERVER"), MB_OK);
	}
	else
	{
		(*ppv) = NULL;
		MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface_NoAggregation() Interface not found returning with error"), TEXT("SERVER ERROR"), MB_OK);
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG CMultiplicationDivision::AddRef_NoAggregation()
{
	TCHAR str[255];	
	
	InterlockedIncrement(&m_cRef);
	
	wsprintf(str, TEXT("In CMultiplicationDivision::AddRef_NoAggregation() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);

	return m_cRef;
}

ULONG CMultiplicationDivision::Release_NoAggregation()
{
	TCHAR str[255];	
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0)
	{
		wsprintf(str, TEXT("In CMultiplicationDivision::Release_NoAggregation() m_cRef = %d deleting object"), m_cRef);
		MessageBox(NULL, str, TEXT("Server"), MB_OK);
		delete(this);
		return 0;
	}
	wsprintf(str, TEXT("In CMultiplicationDivision::Release_NoAggregation() m_cRef = %d"), m_cRef);
	MessageBox(NULL, str, TEXT("Server"), MB_OK);
	return m_cRef;
}

HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void** ppv)
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::QueryInterface()"), TEXT("SERVER"), MB_OK);
	return m_pIUnknownOuter->QueryInterface(riid, ppv);
}

ULONG CMultiplicationDivision::AddRef()
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::AddRef()"), TEXT("SERVER"), MB_OK);
	return m_pIUnknownOuter->AddRef();
}

ULONG CMultiplicationDivision::Release()
{
	MessageBox(NULL, TEXT("In CMultiplicationDivision::Release()"), TEXT("SERVER"), MB_OK);
	return m_pIUnknownOuter->Release();
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
	MessageBox(NULL, TEXT("Entering in CMultiplicationDivisionClassFactory::CreateInstance()"), TEXT("SERVER"), MB_OK);
	CMultiplicationDivision* pCMultiplicationDivision = NULL;
	HRESULT hr = S_FALSE;

	if ( (pOutIUnknown != NULL) && (riid != IID_IUnknown) )
	{
		return CLASS_E_NOAGGREGATION;
	}

	pCMultiplicationDivision = new CMultiplicationDivision(pOutIUnknown);

	if (pCMultiplicationDivision == NULL)
	{
		return E_OUTOFMEMORY;
	}

	hr = pCMultiplicationDivision->QueryInterface_NoAggregation(riid, ppv);
	pCMultiplicationDivision->Release_NoAggregation();
	MessageBox(NULL, TEXT("Exiting in CMultiplicationDivisionClassFactory::CreateInstance() Successfully"), TEXT("SERVER"), MB_OK);
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
