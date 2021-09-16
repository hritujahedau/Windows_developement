class IOddNumber : public IUnknown
{
public:
	
	// IOddNumber specific method declarations (inherited)
	virtual HRESULT __stdcall GetNextOddNumber(int*) = 0;
};

class IEvenNumber : public IUnknown
{
public:
	// IEven Number specific method declarations 
	virtual HRESULT __stdcall GetNextEvenNumber(int*) = 0;
};

class INumberFactory :public IUnknown
{
public:
	virtual HRESULT __stdcall SetFirstOddNumber(int, IOddNumber**) = 0; 
	virtual HRESULT __stdcall SetFirstEvenNumber(int, IEvenNumber**) = 0;
	virtual HRESULT __stdcall SetFirstOddEvenNumber(int, int, IOddNumber**) = 0;
	virtual HRESULT __stdcall SetFirstOddEvenNumber(int, int, IEvenNumber**) = 0;
};

// {6132B8CA-F9B1-4400-81FB-420425CF6A6E}
const CLSID CLSID_Number = { 0x6132b8ca, 0xf9b1, 0x4400, 0x81, 0xfb, 0x42, 0x4, 0x25, 0xcf, 0x6a, 0x6e };

// {A5CB0709-EF8F-4E9C-8A63-2B687E4C4B13}
const IID IID_IOddNumber = { 0xa5cb0709, 0xef8f, 0x4e9c, 0x8a, 0x63, 0x2b, 0x68, 0x7e, 0x4c, 0x4b, 0x13 };

// {5D1E30BE-354C-421D-8ED4-7EB7EB7CA3B1}
const IID IID_IEvenNumber = { 0x5d1e30be, 0x354c, 0x421d, 0x8e, 0xd4, 0x7e, 0xb7, 0xeb, 0x7c, 0xa3, 0xb1 };

// {F266FE76-0789-4F15-B3CA-075A0E4AA94C}
const IID IID_INumberFactory = { 0xf266fe76, 0x789, 0x4f15, 0xb3, 0xca, 0x7, 0x5a, 0xe, 0x4a, 0xa9, 0x4c };
