#ifndef __RSENGINE_BUFFER_CODER_ROUTINE_INTERFACE_H__
#define __RSENGINE_BUFFER_CODER_ROUTINE_INTERFACE_H__

class IREXBufferCoder : public IUnknown
{
public:
	STDMETHOD(Code)(
		LPCVOID InBuffer,
		DWORD InSize,		
		LPVOID OutBuffer,
		LPDWORD OutSize INOUT
		) PURE;
public:
	RE_DECLARE_IID;
};
// {E5646889-8FED-407e-8B56-7FF63D212D17}
RE_DEFINE_IID(IREXBufferCoder, "{E5646889-8FED-407e-8B56-7FF63D212D17}",
			  0xe5646889, 0x8fed, 0x407e, 0x8b, 0x56, 0x7f, 0xf6, 0x3d, 0x21, 0x2d, 0x17);

class IREXBufferCoderEx : public IUnknown
{
public:
	STDMETHOD(CodeEx)(
		LPCVOID InBuffer,
		DWORD InSize,
		LPDWORD InProcessedSize OPTIONAL,
		LPVOID OutBuffer,
		DWORD OutSize,
		LPDWORD OutProcessedSize OPTIONAL
		) PURE;
public:
	RE_DECLARE_IID;
};
// {66CB2600-BE50-4058-B66E-256ADEA94F4D}
RE_DEFINE_IID(IREXBufferCoderEx, "{66CB2600-BE50-4058-B66E-256ADEA94F4D}",
			0x66cb2600, 0xbe50, 0x4058, 0xb6, 0x6e, 0x25, 0x6a, 0xde, 0xa9, 0x4f, 0x4d);


#endif // #ifndef __RSENGINE_BUFFER_CODER_ROUTINE_INTERFACE_H__
