#pragma once

/*
class ISum : public IUnknown
{
public:
	virtual HRESULT _stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract : public IUnknown
{
public:
	virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};
*/

#undef	INTERFACE

#define INTERFACE ISum

DECLARE_INTERFACE_(ISum, IUnknown)
{
	/*fn returning HRESULT*/
	STDMETHOD(QueryInterface)(THIS_ REFIID, void**) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	STDMETHOD(SumOfTwoIntegers)(THIS_ int, int, int*) PURE;
};

#undef	INTERFACE

#define INTERFACE ISubtract

DECLARE_INTERFACE_(ISubtract, IUnknown)
{
	/*fn returning HRESULT*/
	STDMETHOD(QueryInterface)(THIS_ REFIID, void**) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	STDMETHOD(SubtractionOfTwoIntegers)(THIS_ int, int, int*) PURE;
};

// {5AB639E7-4AD9-4538-922F-A48562976370} - strigized
// IMPLEMENT_OLECREATE(<< class >> , << external_name >> ,
// 0x5ab639e7, 0x4ad9, 0x4538, 0x92, 0x2f, 0xa4, 0x85, 0x62, 0x97, 0x63, 0x70);

const CLSID CLSID_SumSubtract = { 0x5ab639e7, 0x4ad9, 0x4538, 0x92, 0x2f, 0xa4, 0x85, 0x62, 0x97, 0x63, 0x70 };

// {B3717A60-62CF-4942-81D9-7AA757E033C0}
// 0xb3717a60, 0x62cf, 0x4942, 0x81, 0xd9, 0x7a, 0xa7, 0x57, 0xe0, 0x33, 0xc0);
const IID IID_ISum = { 0xb3717a60, 0x62cf, 0x4942, 0x81, 0xd9, 0x7a, 0xa7, 0x57, 0xe0, 0x33, 0xc0 };

// {A8A9CD3E-D914-4E5C-890F-E02E69FCEABB}
// 0xa8a9cd3e, 0xd914, 0x4e5c, 0x89, 0xf, 0xe0, 0x2e, 0x69, 0xfc, 0xea, 0xbb);
const IID IID_ISubtract = { 0xa8a9cd3e, 0xd914, 0x4e5c, 0x89, 0xf, 0xe0, 0x2e, 0x69, 0xfc, 0xea, 0xbb };
