#ifndef __RX_INTERNAL_CLASS_CREATOR__
#define __RX_INTERNAL_CLASS_CREATOR__

typedef size_t	InternalClass;
class IRXPrivateClassCreator : public IUnknown
{
public:
	STDMETHOD(CreateObject)( InternalClass cls, IRefable** ppRef ) PURE;
	RE_DECLARE_IID;
};

// {8690BFE3-C8C8-488d-97FE-9522F4958B6A}
RE_DEFINE_IID(IRXPrivateClassCreator, "{8690BFE3-C8C8-488d-97FE-9522F4958B6A}",
			  0x8690bfe3, 0xc8c8, 0x488d, 0x97, 0xfe, 0x95, 0x22, 0xf4, 0x95, 0x8b, 0x6a);

#endif