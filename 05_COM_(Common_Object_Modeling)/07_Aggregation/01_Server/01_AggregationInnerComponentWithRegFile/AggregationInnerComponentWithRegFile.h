
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

// {ACED9F87-3D31-4D7E-BDA9-79CDF3B9D381}
const CLSID CLSID_MultiplicationDivision = { 0xaced9f87, 0x3d31, 0x4d7e, 0xbd, 0xa9, 0x79, 0xcd, 0xf3, 0xb9, 0xd3, 0x81 };

// {EB50A644-8CC1-4FAC-A6B2-1EC02FB4797B}
const IID IID_IMultiplication = { 0xeb50a644, 0x8cc1, 0x4fac, 0xa6, 0xb2, 0x1e, 0xc0, 0x2f, 0xb4, 0x79, 0x7b };

// {08A3607B-C55B-41AC-B913-4B320A6C9AD9}
const IID IID_IDivision = { 0x8a3607b, 0xc55b, 0x41ac, 0xb9, 0x13, 0x4b, 0x32, 0xa, 0x6c, 0x9a, 0xd9 };
