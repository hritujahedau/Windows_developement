
class ISum : public IUnknown
{
public:
	virtual HRESULT	__stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract : public IUnknown
{
public:
	virtual HRESULT	__stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

class IMultiplication : public IUnknown
{
public:
	// IMultiplication Specific Method
	virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*) = 0;
};

class IDivision : public IUnknown
{
public:
	// IDivision specific Method
	virtual HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*) = 0;
};

// {24FCED8F-DD04-4CA3-82C9-782A60D27225}
const CLSID CLSID_SumSubtract = { 0x24fced8f, 0xdd04, 0x4ca3, 0x82, 0xc9, 0x78, 0x2a, 0x60, 0xd2, 0x72, 0x25 };

// {A1B89B08-D497-4FFB-85B5-4FA763B3F7F0}
const IID IID_ISum = { 0xa1b89b08, 0xd497, 0x4ffb, 0x85, 0xb5, 0x4f, 0xa7, 0x63, 0xb3, 0xf7, 0xf0 };

// {A96F572C-60C5-4AEC-85D5-8C4C43D82A33}
const IID IID_ISubtract = { 0xa96f572c, 0x60c5, 0x4aec, 0x85, 0xd5, 0x8c, 0x4c, 0x43, 0xd8, 0x2a, 0x33 };

// {4BDFABEB-ECBA-459D-AF1B-4043C23EF64A}
const IID IID_IMultiplication = { 0x4bdfabeb, 0xecba, 0x459d, 0xaf, 0x1b, 0x40, 0x43, 0xc2, 0x3e, 0xf6, 0x4a };

// {33658BAF-7793-4DBB-BF4C-824876A52183}
const IID IID_IDivision = { 0x33658baf, 0x7793, 0x4dbb, 0xbf, 0x4c, 0x82, 0x48, 0x76, 0xa5, 0x21, 0x83 };
