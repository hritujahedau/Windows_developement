
class IMultiplication : public IUnknown
{
public:
	// IMultiplication Specific Method							  
	virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*) = 0;
};

class IDivision : public IUnknown
{
public:
	// IDivision specifica Method
	virtual HRESULT __stdcall	DivisionOfTwoIntegers(int, int, int*) = 0;
};

// {7883F8E7-1CE8-4ACD-BDDF-C274B1736DDD}
const CLSID CLSID_MultiplicationDivision = { 0x7883f8e7, 0x1ce8, 0x4acd, 0xbd, 0xdf, 0xc2, 0x74, 0xb1, 0x73, 0x6d, 0xdd };

// {4BDFABEB-ECBA-459D-AF1B-4043C23EF64A}
const IID IID_IMultiplication = { 0x4bdfabeb, 0xecba, 0x459d, 0xaf, 0x1b, 0x40, 0x43, 0xc2, 0x3e, 0xf6, 0x4a };

// {33658BAF-7793-4DBB-BF4C-824876A52183}
const IID IID_IDivision = { 0x33658baf, 0x7793, 0x4dbb, 0xbf, 0x4c, 0x82, 0x48, 0x76, 0xa5, 0x21, 0x83 };
