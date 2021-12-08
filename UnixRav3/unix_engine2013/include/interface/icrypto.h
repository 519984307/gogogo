#ifndef ICRYPTO_H_
#define ICRYPTO_H_

class IREXCipher : public IUnknown
{
public:
	STDMETHOD(Init)(
		LPCBYTE,
		DWORD,
		BYTE []
		) PURE;
	STDMETHOD(Code)(
		LPBYTE,
		DWORD 
		) PURE;
	RE_DECLARE_IID;
};
// {3E540722-5CB4-4370-BECA-893238EDCC40}
RE_DEFINE_IID(IREXCipher, "{3E540722-5CB4-4370-BECA-893238EDCC40}",
			  0X3E540722, 0X5CB4, 0X4370, 0XBE, 0XCA, 0X89, 0X32, 0X38, 0XED, 0XCC, 0X40);

class IREXHash: public IUnknown
{
public:
	STDMETHOD(Final)(
		LPVOID Data
		) PURE;
	STDMETHOD(Init)(
		) PURE;
	STDMETHOD(Update)(
		LPCVOID Data,
		DWORD Size
		) PURE;
	RE_DECLARE_IID;
};
// {51BBF6DA-B393-4120-AA39-5CC53463DF95} 
RE_DEFINE_IID(IREXHash, "{51BBF6DA-B393-4120-AA39-5CC53463DF95}",
			  0X51BBF6DA, 0XB393, 0X4120, 0XAA, 0X39, 0X5C, 0XC5, 0X34, 0X63, 0XDF, 0X95);

class IREXHashPassword: public IUnknown
{
public:
	STDMETHOD(SetPassword)(
		LPCBYTE,			// password
		DWORD				// size of password
		)PURE;
	RE_DECLARE_IID;
};
// {A34DD8E7-EA34-443D-876D-CC66B815403B}
RE_DEFINE_IID(IREXHashPassword,"{A34DD8E7-EA34-443D-876D-CC66B815403B}",
			  0XA34DD8E7, 0XEA34, 0X443D, 0X87, 0X6D, 0XCC, 0X66, 0XB8, 0X15, 0X40, 0X3B);

class IREXPKCS : public IUnknown
{
public:
	STDMETHOD(Generate)(
		void			*P,			// password, an octet string
		DWORD			PSize,		// password len
		void			*S,			// salt, an eight-octet string
		DWORD			SSize,		// salt len
		DWORD			c,			// iteration count, a positive integer
		DWORD			dkLen,		// intended length in octets of derived key
		OUT void		*DK			// derived key, a dkLen-octet string
		) PURE;
	RE_DECLARE_IID;
};
// {B121A818-EB91-4FD4-8A94-4CC85C05B22E} 
RE_DEFINE_IID(IREXPKCS, "{B121A818-EB91-4FD4-8A94-4CC85C05B22E}",
			  0XB121A818, 0XEB91, 0X4FD4, 0X8A, 0X94, 0X4C, 0XC8, 0X5C, 0X05, 0XB2, 0X2E);

class IREXClone : public IUnknown
{
public:
	STDMETHOD(Duplicate)(
		IUnknown **ppunk
		) PURE;
	RE_DECLARE_IID;
};
// {68E929A1-6457-4D1D-950E-A2D3B16BD271} 
RE_DEFINE_IID(IREXClone, "{68E929A1-6457-4D1D-950E-A2D3B16BD271}",
			  0X68E929A1, 0X6457, 0X4D1D, 0X95, 0X0E, 0XA2, 0XD3, 0XB1, 0X6B, 0XD2, 0X71);

struct IREXCryptoProperty
{
	struct crc16
	{
		enum 
		{
			POLY,		// VT_UI2	// no default
			INITVAL,	// VT_UI2	// no default
			XOROUTVAL,	// VT_UI2	// no default
		};
	};
	struct crc32
	{
		enum
		{
			POLY,		// VT_UI4	// default is....
		};
	};
};


#endif
