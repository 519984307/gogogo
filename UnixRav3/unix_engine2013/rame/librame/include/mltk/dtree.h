#ifndef __I_RX_DTREE__
#define __I_RX_DTREE__

namespace dt 
{

	typedef LE_F64			IEEE754_Float64;
	typedef IEEE754_Float64	Float64;

	typedef struct _VECTOR64
	{
		Float64*	Values;
		SIZE_T		Count;
	} VECTOR64, *LPVECTOR64;
	
	class IVectorization : public IUnknown
	{
	public:
		STDMETHOD_(SIZE_T, GetVectorSize )( ) PURE;
		STDMETHOD( TestAdaptability )( IUnknown* lpArchive ) PURE;
		STDMETHOD( Vectorize )( IUnknown* lpArchive, LPVECTOR64 lpVector, UINT32* lpPartId = NULL ) PURE;
		//STDMETHOD( GetPartitionId )( IUnknown* lpArchive, UINT32* lpPartId ) PURE;
		RE_DECLARE_IID;
	};

	// {C66D82B8-A54F-4F52-B2B3-63C450795D1D}
	RE_DEFINE_IID( IVectorization, "{C66D82B8-A54F-4F52-B2B3-63C450795D1D}",
	0xc66d82b8, 0xa54f, 0x4f52, 0xb2, 0xb3, 0x63, 0xc4, 0x50, 0x79, 0x5d, 0x1d);

	namespace _16
	{

	#pragma pack(1)

		typedef struct DTNODE16
		{
			LE_UI16 Left;
			LE_UI16 Right;
			LE_UI16 Feature;
			LE_I16 Result;
			Float64	Threshold;
		} __attribute__((packed)) DTNODE, *LPDTNODE;

		enum { END_NODE_ID = 0xFFFF };

	#pragma pack()

		typedef struct MODEL16
		{
			CONST DTNODE *	Nodes;
			SIZE_T			Count;
		} MODEL, * LPMODEL;

		class IPredict : public IUnknown
		{
		public:
			STDMETHOD( Predict )( LPMODEL lpModel, LPVECTOR64 lpVector, UINT16* lpHitNode ) PURE;
			RE_DECLARE_IID;
		};
		// {16088D52-591A-485F-B881-DDF0821F1710}
		RE_DEFINE_IID( IPredict, "{16088D52-591A-485F-B881-DDF0821F1710}",
			0x16088d52, 0x591a, 0x485f, 0xb8, 0x81, 0xdd, 0xf0, 0x82, 0x1f, 0x17, 0x10);
	};

	namespace _32
	{

#pragma pack(1)

		typedef struct DTNODE32
		{
			LE_UI32 Left;
			LE_UI32	Right;
			LE_UI32	Feature;
			LE_I16 Result;
			Float64	Threshold;
		} __attribute__((packed)) DTNODE, *LPDTNODE;

		enum { END_NODE_ID = 0xFFFFFFFF };

#pragma pack()

		typedef struct MODEL32
		{
			CONST DTNODE *	Nodes;
			SIZE_T			Count;
		} MODEL, * LPMODEL;

		class IPredict : public IUnknown
		{
		public:
			STDMETHOD( Predict )( LPMODEL lpModel, LPVECTOR64 lpVector, UINT32* lpHitNode ) PURE;
			RE_DECLARE_IID;
		};

		// {40791384-616D-4943-8A81-D7884A5D8C47}
		RE_DEFINE_IID( IPredict, "{40791384-616D-4943-8A81-D7884A5D8C47}",
			0x40791384, 0x616d, 0x4943, 0x8a, 0x81, 0xd7, 0x88, 0x4a, 0x5d, 0x8c, 0x47);
	};


};

#endif