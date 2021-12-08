#ifndef __I_RX_MINWISE__
#define __I_RX_MINWISE__

namespace minwise
{
	typedef struct FEATURES
	{
		SIZE_T	sizeElement;
		LPCVOID	vpElements;
		SIZE_T	numElements;
	} FEATURES, *LPFEATURES;

	typedef struct _B2T_nGram_MinHash100
	{
		SIZE_T	NGram;
		SIZE_T	MinNum;
	} PARAM_NGRAM, *PARAM_NGRAM_PTR; 
	typedef const PARAM_NGRAM * C_PARAM_NGRAM_PTR;

	static const PARAM_NGRAM B2T_DEFAULT_PARAM = { 8, 63 };

	template < class LT, class RT >
	FORCEINLINE SIZE_T HammingDistance( LPCVOID lp, LPCVOID rp, SIZE_T c )
	{
		const LT * lhs = (const LT *)lp;
		const RT * rhs = (const RT *)rp;
		size_t diffs = 0;
		for(size_t i = 0; i < c; ++i ) 
		{
			if( lhs[i] != rhs[i]) diffs ++;
		}
		return diffs;
	};
};

class IRXMinHashSelector : public IUnknown
{
public:
	STDMETHOD( Start )( LPCVOID lpParam = 0 ) PURE;
	STDMETHOD( Update )( LPCVOID vpData, SIZE_T cbData ) PURE;
	STDMETHOD( GetHashes )( minwise::LPFEATURES lpFeature ) PURE;
	RE_DECLARE_IID;
};

// {52217E39-176C-49FC-9BBD-97B051BEE97B}
RE_DEFINE_IID( IRXMinHashSelector, "{52217E39-176C-49FC-9BBD-97B051BEE97B}",
			  0x52217e39, 0x176c, 0x49fc, 0x9b, 0xbd, 0x97, 0xb0, 0x51, 0xbe, 0xe9, 0x7b);

class IRXMinHashSelector2 : public IUnknown
{
public:
	STDMETHOD( SelectFromArchive )( IUnknown * arch, minwise::LPFEATURES lpFeature, LPCVOID lpParam = NULL ) PURE;
	STDMETHOD( TestAdaptability )( IUnknown * arch ) PURE;
	RE_DECLARE_IID;
};
// {7BF49BC0-82DE-47F8-8D6A-A94DE65F3545}
RE_DEFINE_IID( IRXMinHashSelector2, "{7BF49BC0-82DE-47F8-8D6A-A94DE65F3545}", 
			  0x7bf49bc0, 0x82de, 0x47f8, 0x8d, 0x6a, 0xa9, 0x4d, 0xe6, 0x5f, 0x35, 0x45);



#endif