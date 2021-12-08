#ifndef __IRX_FETCH__
#define __IRX_FETCH__

class IRXFetchUnknown : public IUnknown
{
public:
	STDMETHOD(Fetch)( IUnknown * target, xv::Value * output, LPVOID param = 0 ) PURE;
	RE_DECLARE_IID;
};

// {0CBD3DA2-598B-4B18-AF3C-B59A69056A9D}
RE_DEFINE_IID(IRXFetchUnknown, "{0CBD3DA2-598B-4B18-AF3C-B59A69056A9D}",
			0xcbd3da2, 0x598b, 0x4b18, 0xaf, 0x3c, 0xb5, 0x9a, 0x69, 0x5, 0x6a, 0x9d);

#endif