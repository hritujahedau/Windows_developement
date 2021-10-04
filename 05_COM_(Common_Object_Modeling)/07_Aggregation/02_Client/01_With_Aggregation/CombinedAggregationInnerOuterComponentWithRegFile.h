
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

// {9913841C-0566-416A-8D48-C4390505DD7D}
const CLSID CLSID_SumSubtract = { 0x9913841c, 0x566, 0x416a, 0x8d, 0x48, 0xc4, 0x39, 0x5, 0x5, 0xdd, 0x7d };

// {D86A4463-5613-4670-A712-101358C23319}
const IID IID_ISum = { 0xd86a4463, 0x5613, 0x4670, 0xa7, 0x12, 0x10, 0x13, 0x58, 0xc2, 0x33, 0x19 };

// {78E74AD3-75D4-4F64-9890-F72A5970EFC7}
const IID IID_ISubtract = { 0x78e74ad3, 0x75d4, 0x4f64, 0x98, 0x90, 0xf7, 0x2a, 0x59, 0x70, 0xef, 0xc7 };

// {EB50A644-8CC1-4FAC-A6B2-1EC02FB4797B}
const IID IID_IMultiplication = { 0xeb50a644, 0x8cc1, 0x4fac, 0xa6, 0xb2, 0x1e, 0xc0, 0x2f, 0xb4, 0x79, 0x7b };

// {08A3607B-C55B-41AC-B913-4B320A6C9AD9}
const IID IID_IDivision = { 0x8a3607b, 0xc55b, 0x41ac, 0xb9, 0x13, 0x4b, 0x32, 0xa, 0x6c, 0x9a, 0xd9 };
