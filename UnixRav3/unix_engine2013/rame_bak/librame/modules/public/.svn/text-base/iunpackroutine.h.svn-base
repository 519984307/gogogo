#ifndef __RSENGINE_UNPACK_ROUTINE_INTERFACE_H__
#define __RSENGINE_UNPACK_ROUTINE_INTERFACE_H__


//////////////////////////////////////////////////////////////////////////
// 09 dynamic hook init:
// (IREPEDynInit)
// (ur000.dat):
// {F38FF442-7381-4c07-BA13-064BBA2F5814}
DEFINE_GUID(CLSID_CREURoutine_ScanEx_Init,
			0xf38ff442, 0x7381, 0x4c07, 0xba, 0x13, 0x6, 0x4b, 0xba, 0x2f, 0x58, 0x14);


//////////////////////////////////////////////////////////////////////////
// 09 routine utils:
// (IREPEStdMethod)
// (urutils.dll):
// {4F2559DF-FF90-4f89-8045-ECE1C27D1A10}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Dump,
			0x4f2559df, 0xff90, 0x4f89, 0x80, 0x45, 0xec, 0xe1, 0xc2, 0x7d, 0x1a, 0x10);
// 09 PE engine stash storage:
// VT_UNKNOWN
// {C68080F7-4DFF-4bd8-9E4B-276E3105AA85}
DEFINE_GUID(PID_PEStash_Unpack_DumpFile,
			0xc68080f7, 0x4dff, 0x4bd8, 0x9e, 0x4b, 0x27, 0x6e, 0x31, 0x5, 0xaa, 0x85);

class IGetApiNameByHash : public IUnknown
{
public:
	STDMETHOD_(const char*,GetApiNameByHash)(char* dllname,DWORD dwCRC32) = 0;
	STDMETHOD_(bool,IsDllHashed)(char* dllname) = 0;
	STDMETHOD_(const char**,GetApiNameArray)(char* dllname,DWORD& dwApiCount) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IGetApiNameByHash, "{C0801AFE-FF8E-4a3b-B3C0-871B6A81246E}",
			  0xc0801afe, 0xff8e, 0x4a3b, 0xb3, 0xc0, 0x87, 0x1b, 0x6a, 0x81, 0x24, 0x6e);

// {7C29443C-E38E-47ca-90D5-0B1A1D7DAB59}
DEFINE_GUID(CLSID_CREURoutine_GetApiNameByHash, 
			0x7c29443c, 0xe38e, 0x47ca, 0x90, 0xd5, 0xb, 0x1a, 0x1d, 0x7d, 0xab, 0x59);


//////////////////////////////////////////////////////////////////////////
// 09 routine utils:
// (IREPEStdMethod)
// (urutils.dll):
// {FF02E161-AD85-4cbe-8F47-566D48BAD592}
DEFINE_GUID(CLSID_CREURoutine_Post_FixPE_IAT,
			0xff02e161, 0xad85, 0x4cbe, 0x8f, 0x47, 0x56, 0x6d, 0x48, 0xba, 0xd5, 0x92);
// 09 PE engine stash storage:
// VT_UI4
// {6AF7D42B-3757-4a5a-B65C-F38524B2A739}
DEFINE_GUID(PID_PEStash_Unpack_FixPE_IAT_RVA,
			0x6af7d42b, 0x3757, 0x4a5a, 0xb6, 0x5c, 0xf3, 0x85, 0x24, 0xb2, 0xa7, 0x39);
// VT_UI4
// {5760BF90-6323-4c26-8E0B-C5370962A79C}
DEFINE_GUID(PID_PEStash_Unpack_FixPE_IAT_Size,
			0x5760bf90, 0x6323, 0x4c26, 0x8e, 0xb, 0xc5, 0x37, 0x9, 0x62, 0xa7, 0x9c);


//////////////////////////////////////////////////////////////////////////
// 09 unpack routines:
// (IREPEStdMethod)

// (ur001.dat):
// {D3B1AB72-944C-43e7-B6B0-99818C3757F5}
DEFINE_GUID(CLSID_CREURoutine_Unpack_UPX_deal_sfx,
			0xd3b1ab72, 0x944c, 0x43e7, 0xb6, 0xb0, 0x99, 0x81, 0x8c, 0x37, 0x57, 0xf5);
// {5AE1DFC9-58A7-48ac-9880-CBE20AA4D0E9}
DEFINE_GUID(CLSID_CREURoutine_Unpack_UPX,
			0x5ae1dfc9, 0x58a7, 0x48ac, 0x98, 0x80, 0xcb, 0xe2, 0xa, 0xa4, 0xd0, 0xe9);
// {B7930379-AD73-4adb-9538-C3A5AF75E3D0}
DEFINE_GUID(CLSID_CREURoutine_Unpack_UPX_VMRUN, 
			0xb7930379, 0xad73, 0x4adb, 0x95, 0x38, 0xc3, 0xa5, 0xaf, 0x75, 0xe3, 0xd0);
// {73CAC679-0899-4a6b-B616-9255E067DD1E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_UPX_fakeupx,
			0x73cac679, 0x899, 0x4a6b, 0xb6, 0x16, 0x92, 0x55, 0xe0, 0x67, 0xdd, 0x1e);

// (ur002.dat):
// {C638F2E9-371B-413d-9DA4-76973858DB29}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Shoooo,
			0xc638f2e9, 0x371b, 0x413d, 0x9d, 0xa4, 0x76, 0x97, 0x38, 0x58, 0xdb, 0x29);

// (ur003.dat):
// {83B7F1D9-C92B-411d-A7EE-AE3D647FBDBA}
DEFINE_GUID(CLSID_CREURoutine_Unpack_PackMan,
			0x83b7f1d9, 0xc92b, 0x411d, 0xa7, 0xee, 0xae, 0x3d, 0x64, 0x7f, 0xbd, 0xba);

// (ur004.dat):
// {360EB63B-ED68-4997-90C3-4B4ABF6C1808}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Petite_2x,
			0x360eb63b, 0xed68, 0x4997, 0x90, 0xc3, 0x4b, 0x4a, 0xbf, 0x6c, 0x18, 0x8);

// (ur005.dat):
// {823D4588-0603-4b02-86DD-06F3C782BAED}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Exeshield,
			0x823d4588, 0x603, 0x4b02, 0x86, 0xdd, 0x6, 0xf3, 0xc7, 0x82, 0xba, 0xed);

// (ur006.dat):
// {E383E9D0-32B4-49c9-9390-D6364C2684E8}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pex_099,
			0xe383e9d0, 0x32b4, 0x49c9, 0x93, 0x90, 0xd6, 0x36, 0x4c, 0x26, 0x84, 0xe8);
// {A2889870-2926-46ca-AAE5-A3448D372D9E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pex_got,
			0xa2889870, 0x2926, 0x46ca, 0xaa, 0xe5, 0xa3, 0x44, 0x8d, 0x37, 0x2d, 0x9e);
// {658AAD18-271B-4882-A060-70C7C851C796}
DEFINE_GUID(CLSID_CREURoutine_Unpack_bedrock,
			0x658aad18, 0x271b, 0x4882, 0xa0, 0x60, 0x70, 0xc7, 0xc8, 0x51, 0xc7, 0x96);
// {B0131F5C-71D0-4c4f-8D30-83DDF1D934DE}
DEFINE_GUID(CLSID_CREURoutine_Unpack_morphine,
			0xb0131f5c, 0x71d0, 0x4c4f, 0x8d, 0x30, 0x83, 0xdd, 0xf1, 0xd9, 0x34, 0xde);
// {205E9560-6BCA-4b9f-BB35-24B912F6B327}
DEFINE_GUID(CLSID_CREURoutine_Unpack_morphine_27b,
			0x205e9560, 0x6bca, 0x4b9f, 0xbb, 0x35, 0x24, 0xb9, 0x12, 0xf6, 0xb3, 0x27);
// {12170590-56AB-4ec3-958C-0B9A54DCF159}
DEFINE_GUID(CLSID_CREURoutine_Unpack_alexprotect,
			0x12170590, 0x56ab, 0x4ec3, 0x95, 0x8c, 0xb, 0x9a, 0x54, 0xdc, 0xf1, 0x59);
// {C556F5FE-47EB-4537-A0F3-A8F9FD68A8CB}
DEFINE_GUID(CLSID_CREURoutine_Unpack_mhrls, 
			0xc556f5fe, 0x47eb, 0x4537, 0xa0, 0xf3, 0xa8, 0xf9, 0xfd, 0x68, 0xa8, 0xcb);
// {484E9A51-DFE6-4204-9ED8-6167CE252935}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Dotfix, 
			0x484e9a51, 0xdfe6, 0x4204, 0x9e, 0xd8, 0x61, 0x67, 0xce, 0x25, 0x29, 0x35);

// (ur007.dat):
// {F8756CBA-E84A-4853-A965-99017D433647}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Armadillo,
			0xf8756cba, 0xe84a, 0x4853, 0xa9, 0x65, 0x99, 0x1, 0x7d, 0x43, 0x36, 0x47);

// (ur008.dat):
// {AE0B6FAF-B2AE-44d1-B43F-F79AB8367ADB}
DEFINE_GUID(CLSID_CREURoutine_Unpack_SimplePack,
			0xae0b6faf, 0xb2ae, 0x44d1, 0xb4, 0x3f, 0xf7, 0x9a, 0xb8, 0x36, 0x7a, 0xdb);
// {276959DA-D81A-47c7-BACF-5238E92E957E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_SimplePack_120,
			0x276959da, 0xd81a, 0x47c7, 0xba, 0xcf, 0x52, 0x38, 0xe9, 0x2e, 0x95, 0x7e);
// {8A2A8F7D-1DA0-4841-B6EA-8E0BCF62AAB2}
DEFINE_GUID(CLSID_CREURoutine_Unpack_SimplePack_121,
			0x8a2a8f7d, 0x1da0, 0x4841, 0xb6, 0xea, 0x8e, 0xb, 0xcf, 0x62, 0xaa, 0xb2);

// (ur009.dat):
// {A8905735-902A-4aa2-BE37-18075A7466B5}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_01,
			0xa8905735, 0x902a, 0x4aa2, 0xbe, 0x37, 0x18, 0x7, 0x5a, 0x74, 0x66, 0xb5);
// {42F8D138-7E7C-4f6e-A363-8F5C53607188}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_03,
			0x42f8d138, 0x7e7c, 0x4f6e, 0xa3, 0x63, 0x8f, 0x5c, 0x53, 0x60, 0x71, 0x88);
// {BF6A3200-53FB-4e69-8FF3-21A1F6D84B09}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_07,
			0xbf6a3200, 0x53fb, 0x4e69, 0x8f, 0xf3, 0x21, 0xa1, 0xf6, 0xd8, 0x4b, 0x9);
// {8620B8C1-D4DE-42f9-B7B4-1C3CBC25C902}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_041, 
			0x8620b8c1, 0xd4de, 0x42f9, 0xb7, 0xb4, 0x1c, 0x3c, 0xbc, 0x25, 0xc9, 0x2);
// {B47D672B-080C-4e20-B38F-1EFB8A294699}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_10,
			0xb47d672b, 0x80c, 0x4e20, 0xb3, 0x8f, 0x1e, 0xfb, 0x8a, 0x29, 0x46, 0x99);
// {1BD090D9-6EBF-42c5-986D-703345CA3644}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_11,
			0x1bd090d9, 0x6ebf, 0x42c5, 0x98, 0x6d, 0x70, 0x33, 0x45, 0xca, 0x36, 0x44);
// {72B12259-6AEB-4428-B310-FB160FD8DD21}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_13,
			0x72b12259, 0x6aeb, 0x4428, 0xb3, 0x10, 0xfb, 0x16, 0xf, 0xd8, 0xdd, 0x21);
// {2C76AA46-1A43-4e53-92AF-5D6BE1EF340E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_1304,
			0x2c76aa46, 0x1a43, 0x4e53, 0x92, 0xaf, 0x5d, 0x6b, 0xe1, 0xef, 0x34, 0xe);
// {65CC12B2-9DAC-4a91-BD45-91897CC1A76D}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pespin_1320,
			0x65cc12b2, 0x9dac, 0x4a91, 0xbd, 0x45, 0x91, 0x89, 0x7c, 0xc1, 0xa7, 0x6d);
// {81A83356-930B-4d0d-9233-AD686548F5D1}
DEFINE_GUID(CLSID_CREURoutine_Unpack_pelock,
			0x81a83356, 0x930b, 0x4d0d, 0x92, 0x33, 0xad, 0x68, 0x65, 0x48, 0xf5, 0xd1);

// (ur010.dat):
// {D54A8D66-ECC2-43ae-9C39-69841BC8B030}
DEFINE_GUID(CLSID_CREURoutine_Unpack_PcGuard,
			0xd54a8d66, 0xecc2, 0x43ae, 0x9c, 0x39, 0x69, 0x84, 0x1b, 0xc8, 0xb0, 0x30);
// {5F06E5DE-DAF2-475d-885C-763F89875DD9}
DEFINE_GUID(CLSID_CREURoutine_Unpack_PcGuard_50,
			0x5f06e5de, 0xdaf2, 0x475d, 0x88, 0x5c, 0x76, 0x3f, 0x89, 0x87, 0x5d, 0xd9);
// {3EDA1E91-ED92-49a5-AB29-B09AA927683C}
DEFINE_GUID(CLSID_CREURoutine_Unpack_asd,
			0x3eda1e91, 0xed92, 0x49a5, 0xab, 0x29, 0xb0, 0x9a, 0xa9, 0x27, 0x68, 0x3c);
// {FA3457C9-E69E-4251-BD25-1CBCBD0510C0}
DEFINE_GUID(CLSID_CREURoutine_Unpack_neolite,
			0xfa3457c9, 0xe69e, 0x4251, 0xbd, 0x25, 0x1c, 0xbc, 0xbd, 0x5, 0x10, 0xc0);
// {76FA78B9-CBD0-402d-8F8C-0BCDD218BE7E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_polyencoder,
			0x76fa78b9, 0xcbd0, 0x402d, 0x8f, 0x8c, 0xb, 0xcd, 0xd2, 0x18, 0xbe, 0x7e);

// (ur011.dat):
// {1C8474EF-5C9C-4b7e-890C-3CBDB0C0D129}
DEFINE_GUID(CLSID_CREURoutine_Unpack_RLpack_full,
			0x1c8474ef, 0x5c9c, 0x4b7e, 0x89, 0xc, 0x3c, 0xbd, 0xb0, 0xc0, 0xd1, 0x29);
// {25DA349D-98EA-4887-90E9-A3F968EB8DF0}
DEFINE_GUID(CLSID_CREURoutine_Unpack_RLpack_basic,
			0x25da349d, 0x98ea, 0x4887, 0x90, 0xe9, 0xa3, 0xf9, 0x68, 0xeb, 0x8d, 0xf0);
// {A727919F-B0ED-4685-98EA-420F561ACE04}
DEFINE_GUID(CLSID_CREURoutine_Unpack_RLpack_120Full, 
			0xa727919f, 0xb0ed, 0x4685, 0x98, 0xea, 0x42, 0xf, 0x56, 0x1a, 0xce, 0x4);

// (ur012.dat):
// {702D60B3-0E46-435e-9D7E-DC0236BA0BC5}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Acprotect,
			0x702d60b3, 0xe46, 0x435e, 0x9d, 0x7e, 0xdc, 0x2, 0x36, 0xba, 0xb, 0xc5);

// (ur013.dat):
// {904AB792-FFF0-40f6-A378-773DCCACBAF6}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Dbpe,
			0x904ab792, 0xfff0, 0x40f6, 0xa3, 0x78, 0x77, 0x3d, 0xcc, 0xac, 0xba, 0xf6);

// (ur014.dat):
// {0073C820-A9A4-4f40-A72B-5235A04761A9}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Execryptor,
			0x73c820, 0xa9a4, 0x4f40, 0xa7, 0x2b, 0x52, 0x35, 0xa0, 0x47, 0x61, 0xa9);

// (ur015.dat):
// {C3FB6B76-CA3E-4c0e-9689-DC7BDCA7F40C}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Svkp,
			0xc3fb6b76, 0xca3e, 0x4c0e, 0x96, 0x89, 0xdc, 0x7b, 0xdc, 0xa7, 0xf4, 0xc);

// (ur016.dat):
// {36D579A4-3F36-43f8-B056-4F0CF04BBE9E}
DEFINE_GUID(CLSID_CREURoutine_Unpack_BitArts,
			0x36d579a4, 0x3f36, 0x43f8, 0xb0, 0x56, 0x4f, 0xc, 0xf0, 0x4b, 0xbe, 0x9e);

// (ur017.dat):
// {D622561F-CB03-431a-8B8F-DCD373E55D7D}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pklite32,
			0xd622561f, 0xcb03, 0x431a, 0x8b, 0x8f, 0xdc, 0xd3, 0x73, 0xe5, 0x5d, 0x7d);

// (ur018.dat):
// {A315B473-4D84-4ba9-B698-4A9777CE2604}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Crypt,
			0xa315b473, 0x4d84, 0x4ba9, 0xb6, 0x98, 0x4a, 0x97, 0x77, 0xce, 0x26, 0x4);

// (ur019.dat):
// {2B2AF9D7-102D-49f2-9731-BA63B70A0D3A}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Expressor_12_13,
			0x2b2af9d7, 0x102d, 0x49f2, 0x97, 0x31, 0xba, 0x63, 0xb7, 0xa, 0xd, 0x3a);
// {DE867E51-4FB2-4cbc-BF8F-F5A3363239A0}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Expressor,
			0xde867e51, 0x4fb2, 0x4cbc, 0xbf, 0x8f, 0xf5, 0xa3, 0x36, 0x32, 0x39, 0xa0);

// (ur020.dat):
// {9EEDFE27-51D5-425f-9386-992F58E1C781}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pebundle_1,
			0x9eedfe27, 0x51d5, 0x425f, 0x93, 0x86, 0x99, 0x2f, 0x58, 0xe1, 0xc7, 0x81);
// {46E9C850-1BA0-4b44-82BA-EC77E0FA6858}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pebundle_2,
			0x46e9c850, 0x1ba0, 0x4b44, 0x82, 0xba, 0xec, 0x77, 0xe0, 0xfa, 0x68, 0x58);
// {40FF5ECD-C3BD-462d-87AB-B5B867920EFA}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pebundle_3, 
			0x40ff5ecd, 0xc3bd, 0x462d, 0x87, 0xab, 0xb5, 0xb8, 0x67, 0x92, 0xe, 0xfa);


// (ur021.dat):
// {729036C0-1D6A-4943-AD36-0BAE0B5296D3}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1030,
			0x729036c0, 0x1d6a, 0x4943, 0xad, 0x36, 0xb, 0xae, 0xb, 0x52, 0x96, 0xd3);
// {CC49B8A3-F16C-4e99-9540-C6A9DCD11FBC}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1030a,
			0xcc49b8a3, 0xf16c, 0x4e99, 0x95, 0x40, 0xc6, 0xa9, 0xdc, 0xd1, 0x1f, 0xbc);
// {FA58BFE9-8646-4e78-AD68-500CCCE1924B}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1030b, 
			0xfa58bfe9, 0x8646, 0x4e78, 0xad, 0x68, 0x50, 0xc, 0xcc, 0xe1, 0x92, 0x4b);
// {80DE75FC-20ED-450f-B437-5AD46377EE80}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1030d, 
			0x80de75fc, 0x20ed, 0x450f, 0xb4, 0x37, 0x5a, 0xd4, 0x63, 0x77, 0xee, 0x80);
// {28B4B7A8-FCF3-4ca2-9309-D3C07A1653B5}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1031,
			0x28b4b7a8, 0xfcf3, 0x4ca2, 0x93, 0x9, 0xd3, 0xc0, 0x7a, 0x16, 0x53, 0xb5);
// {45820CA4-FC7A-442b-B9CF-19B14CF1EB07}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1031a,
			0x45820ca4, 0xfc7a, 0x442b, 0xb9, 0xcf, 0x19, 0xb1, 0x4c, 0xf1, 0xeb, 0x7);
// {102ECECB-CCCA-44fa-9EFB-A75C5B4B81EC}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1031b, 
			0x102ececb, 0xccca, 0x44fa, 0x9e, 0xfb, 0xa7, 0x5c, 0x5b, 0x4b, 0x81, 0xec);
// {CB242AFF-F1E0-4514-B447-8A56215C9E6C}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1032,
			0xcb242aff, 0xf1e0, 0x4514, 0xb4, 0x47, 0x8a, 0x56, 0x21, 0x5c, 0x9e, 0x6c);
// {3AB8D4E6-B27B-4c0a-BB19-0B874E7DB24C}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_1033,
			0x3ab8d4e6, 0xb27b, 0x4c0a, 0xbb, 0x19, 0xb, 0x87, 0x4e, 0x7d, 0xb2, 0x4c);
// {73420E6F-5799-4235-9A2B-BE4170FE85E8}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Yoda_mutilator,
			0x73420e6f, 0x5799, 0x4235, 0x9a, 0x2b, 0xbe, 0x41, 0x70, 0xfe, 0x85, 0xe8);

// (ur022.dat):
// {4E48596A-3C2F-49b4-A1DB-4EA1FE96BF6A}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Orien_103,
			0x4e48596a, 0x3c2f, 0x49b4, 0xa1, 0xdb, 0x4e, 0xa1, 0xfe, 0x96, 0xbf, 0x6a);
// {51F52EDF-9176-4a67-85AF-AACA3F4073C7}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Orien_212,
			0x51f52edf, 0x9176, 0x4a67, 0x85, 0xaf, 0xaa, 0xca, 0x3f, 0x40, 0x73, 0xc7);

// (ur023.dat):
// {07D18517-5CA8-4caf-9326-E8CF39964BED}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pecompact2x,
			0x7d18517, 0x5ca8, 0x4caf, 0x93, 0x26, 0xe8, 0xcf, 0x39, 0x96, 0x4b, 0xed);

// (ur024.dat):
// {38454D60-2363-4d2d-A224-94AA9D1BCF55}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Pe_Armor,
			0x38454d60, 0x2363, 0x4d2d, 0xa2, 0x24, 0x94, 0xaa, 0x9d, 0x1b, 0xcf, 0x55);

// (ur025.dat):
// {D46ACC21-62C4-48e1-99C2-0D12E80A3281}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspack_100b,
			0xd46acc21, 0x62c4, 0x48e1, 0x99, 0xc2, 0xd, 0x12, 0xe8, 0xa, 0x32, 0x81);
// {99DBE904-9961-4c05-90E8-DD6EAFF9A864}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspack_10803,
			0x99dbe904, 0x9961, 0x4c05, 0x90, 0xe8, 0xdd, 0x6e, 0xaf, 0xf9, 0xa8, 0x64);
// {6D2C22B8-0C87-4506-848A-481B19E1DCF3}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspack_2000,
			0x6d2c22b8, 0xc87, 0x4506, 0x84, 0x8a, 0x48, 0x1b, 0x19, 0xe1, 0xdc, 0xf3);

// (ur026.dat):
// {F01C3A07-B5F3-40bb-9CB0-F9EE84306404}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Enigma,
			0xf01c3a07, 0xb5f3, 0x40bb, 0x9c, 0xb0, 0xf9, 0xee, 0x84, 0x30, 0x64, 0x4);

// (ur027.dat):
// {C7284D13-C56B-491b-BB09-0D17340D4B6C}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspr_1x,
			0xc7284d13, 0xc56b, 0x491b, 0xbb, 0x9, 0xd, 0x17, 0x34, 0xd, 0x4b, 0x6c);
// {DDF0139B-88BB-4191-A202-CE1AEDBA4690}
DEFINE_GUID(CLSID_CREURoutine_Fix_Aspr_1x,
			0xddf0139b, 0x88bb, 0x4191, 0xa2, 0x2, 0xce, 0x1a, 0xed, 0xba, 0x46, 0x90);
// {FF405A92-AE0A-41b3-9637-58F5E259683F}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspr_2x,
			0xff405a92, 0xae0a, 0x41b3, 0x96, 0x37, 0x58, 0xf5, 0xe2, 0x59, 0x68, 0x3f);
// {5DA176FD-4036-4590-950D-DE678A9F568B}
DEFINE_GUID(CLSID_CREURoutine_Fix_Aspr_2x, 
			0x5da176fd, 0x4036, 0x4590, 0x95, 0xd, 0xde, 0x67, 0x8a, 0x9f, 0x56, 0x8b);
// {26E77115-A811-4fe4-839D-DA2E4D3F1398}
DEFINE_GUID(CLSID_CREURoutine_Unpack_Aspr_2x_new,
			0x26e77115, 0xa811, 0x4fe4, 0x83, 0x9d, 0xda, 0x2e, 0x4d, 0x3f, 0x13, 0x98);

// (ur028.dat)
// {70AD1974-5554-4ff8-9F3E-F6F72EB08905}
DEFINE_GUID(CLSID_CREURoutine_MoleBox_A, 
			0x70ad1974, 0x5554, 0x4ff8, 0x9f, 0x3e, 0xf6, 0xf7, 0x2e, 0xb0, 0x89, 0x5);
// {2C0A2034-AAE7-4116-9294-AC10E5F65F90}
DEFINE_GUID(CLSID_CREURoutine_MoleBox_B, 
			0x2c0a2034, 0xaae7, 0x4116, 0x92, 0x94, 0xac, 0x10, 0xe5, 0xf6, 0x5f, 0x90);
// {58310F28-D926-4cbf-AB21-35B126D93444}
DEFINE_GUID(CLSID_CREURoutine_MoleBox_C, 
			0x58310f28, 0xd926, 0x4cbf, 0xab, 0x21, 0x35, 0xb1, 0x26, 0xd9, 0x34, 0x44);
// {4B220D70-0FF8-41eb-B98A-7127F47D660F}
DEFINE_GUID(CLSID_CREURoutine_MoleBox_D, 
			0x4b220d70, 0xff8, 0x41eb, 0xb9, 0x8a, 0x71, 0x27, 0xf4, 0x7d, 0x66, 0xf);
// {53FB8665-4C6A-4c44-8230-9F9CFBF8F5D4}
DEFINE_GUID(CLSID_CREURoutine_MoleBox_E, 
			0x53fb8665, 0x4c6a, 0x4c44, 0x82, 0x30, 0x9f, 0x9c, 0xfb, 0xf8, 0xf5, 0xd4);
// {B836FB72-DEDC-4d11-85EB-996FF9483798}
DEFINE_GUID(CLSID_CREURoutine_ZProtect, 
			0xb836fb72, 0xdedc, 0x4d11, 0x85, 0xeb, 0x99, 0x6f, 0xf9, 0x48, 0x37, 0x98);

// {3CE35CD0-1D1B-46e6-B05B-7E0A7475F819}
DEFINE_GUID(CLSID_CREURoutine_Thinstall2x, 
			0x3ce35cd0, 0x1d1b, 0x46e6, 0xb0, 0x5b, 0x7e, 0xa, 0x74, 0x75, 0xf8, 0x19);
//////////////////////////////////////////////////////////////////////////


static const int PROP_LC	=	0;
static const int PROP_LP	=	1;
static const int PROP_PB	=	2;

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
RE_DEFINE_IID(IREXBufferCoderEx, "{66CB2600-BE50-4058-B66E-256ADEA94F4D}",
			0x66cb2600, 0xbe50, 0x4058, 0xb6, 0x6e, 0x25, 0x6a, 0xde, 0xa9, 0x4f, 0x4d);



// {691D3661-D855-4aec-AE14-F1849549F42C}
DEFINE_GUID(CLSID_CRECompressor_UCL_nrv2b_decompress_8, 
			0x691d3661, 0xd855, 0x4aec, 0xae, 0x14, 0xf1, 0x84, 0x95, 0x49, 0xf4, 0x2c);

// {2922AD19-1135-49aa-A1B6-D22A8FA1B13D}
DEFINE_GUID(CLSID_CRECompressor_UCL_nrv2d_decompress_8_s, 
			0x2922ad19, 0x1135, 0x49aa, 0xa1, 0xb6, 0xd2, 0x2a, 0x8f, 0xa1, 0xb1, 0x3d);

// {9E9B674A-15DC-4313-A6D2-B061889496F1}
DEFINE_GUID(CLSID_CRECompressor_UCL_upx_decompress_b, 
			0x9e9b674a, 0x15dc, 0x4313, 0xa6, 0xd2, 0xb0, 0x61, 0x88, 0x94, 0x96, 0xf1);

// {ABB54167-6B9E-4e1f-983B-475FE54D7561}
DEFINE_GUID(CLSID_CRECompressor_UCL_upx_decompress_d, 
			0xabb54167, 0x6b9e, 0x4e1f, 0x98, 0x3b, 0x47, 0x5f, 0xe5, 0x4d, 0x75, 0x61);

// {1EA93536-6205-46cd-B481-9DDB058FEEDB}
DEFINE_GUID(CLSID_CRECompressor_UCL_upx_decompress_s_d, 
			0x1ea93536, 0x6205, 0x46cd, 0xb4, 0x81, 0x9d, 0xdb, 0x5, 0x8f, 0xee, 0xdb);

// {751CCBD9-86FD-4359-AD46-47DA4E3A9C23}
DEFINE_GUID(CLSID_CRECompressor_UCL_upx_decompress_s_b, 
			0x751ccbd9, 0x86fd, 0x4359, 0xad, 0x46, 0x47, 0xda, 0x4e, 0x3a, 0x9c, 0x23);

// {EA6132B8-A887-4504-B6F2-AED6BAA90D06}
DEFINE_GUID(CLSID_CRECompressor_LZMA, 
			0xea6132b8, 0xa887, 0x4504, 0xb6, 0xf2, 0xae, 0xd6, 0xba, 0xa9, 0xd, 0x6);

// {EDDF5441-E74F-4e8e-AA8E-6183BDBA5F32}
DEFINE_GUID(CLSID_CRECompressor_aPLib_010b, 
			0xeddf5441, 0xe74f, 0x4e8e, 0xaa, 0x8e, 0x61, 0x83, 0xbd, 0xba, 0x5f, 0x32);

// {34FC5287-67BD-438f-A048-BE7D99609BDC}
DEFINE_GUID(CLSID_CRECompressor_aPLib_017b, 
			0x34fc5287, 0x67bd, 0x438f, 0xa0, 0x48, 0xbe, 0x7d, 0x99, 0x60, 0x9b, 0xdc);

// {9DB48057-D809-4cc3-8D06-428ACD4C2F2A}
DEFINE_GUID(CLSID_CRECompressor_aPLib_022b, 
			0x9db48057, 0xd809, 0x4cc3, 0x8d, 0x6, 0x42, 0x8a, 0xcd, 0x4c, 0x2f, 0x2a);

// {936EE4A7-B491-4e7b-A68D-67499A5AB2D0}
DEFINE_GUID(CLSID_CRECompressor_aPLib_036, 
			0x936ee4a7, 0xb491, 0x4e7b, 0xa6, 0x8d, 0x67, 0x49, 0x9a, 0x5a, 0xb2, 0xd0);

// {C3009B1C-DEB1-45c1-B505-66A47F825229}
DEFINE_GUID(CLSID_CRECompressor_Jcalg1,
			0xc3009b1c, 0xdeb1, 0x45c1, 0xb5, 0x5, 0x66, 0xa4, 0x7f, 0x82, 0x52, 0x29);

// {1f20c19a-ebc6-4e45-9b15-9e7f69df1753} 
DEFINE_GUID(CLSID_CRECompressor_LZ_Pklite32, 
			0x1f20c19a, 0xebc6, 0x4e45, 0x9b, 0x15, 0x9e, 0x7f, 0x69, 0xdf, 0x17, 0x53);

class IRERoutineParam : public IUnknown
{
public:
	STDMETHOD_(bool, Push)(void* pvParam) = 0;
	STDMETHOD_(bool, Pop)(void** ppvParam) = 0;
	STDMETHOD_(unsigned long, Size)() = 0;
	STDMETHOD_(void, Clear)() = 0;

	STDMETHOD_(bool, GetPos)(void **ppParam, int pos) = 0;//
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IRERoutineParam, "{7408C9A9-EFBB-4e01-B327-9698AEECF3B2}",
0x7408c9a9, 0xefbb, 0x4e01, 0xb3, 0x27, 0x96, 0x98, 0xae, 0xec, 0xf3, 0xb2);

// {B90F2431-CF88-45db-9CA4-7D574B9D86F7}
DEFINE_GUID(CLSID_CRERoutineParam, 
0xb90f2431, 0xcf88, 0x45db, 0x9c, 0xa4, 0x7d, 0x57, 0x4b, 0x9d, 0x86, 0xf7);


// {FB938F9A-2138-4867-8E3B-3E0E38DF3425}
DEFINE_GUID(CLSID_CRESVM, 
			0xfb938f9a, 0x2138, 0x4867, 0x8e, 0x3b, 0x3e, 0xe, 0x38, 0xdf, 0x34, 0x25);


// {ED2F8B99-AF4D-489d-A353-3B08F3AA4155}
DEFINE_GUID(CLSID_CREUnpackRoutine_V1, 
			0xed2f8b99, 0xaf4d, 0x489d, 0xa3, 0x53, 0x3b, 0x8, 0xf3, 0xaa, 0x41, 0x55);
//////////////////////////////////////////////////////////////////////////


class IRERoutine : public IUnknown
{
public:
	STDMETHOD_(DWORD, Query)(const char* pszName) = 0;
	STDMETHOD_(bool, IsExist)(DWORD idRotine) = 0;
	STDMETHOD(Invoke)(DWORD idRotine, IRERoutineParam* pParam) = 0;
public:
	enum { INVALID_ROUTINE_ID = 0xFFFFFFFF };
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IRERoutine, "{7FD5D4EF-C413-4c09-B11A-ADED757ED8E8}",
0x7fd5d4ef, 0xc413, 0x4c09, 0xb1, 0x1a, 0xad, 0xed, 0x75, 0x7e, 0xd8, 0xe8);

// {1E1D0CAA-785D-4d33-A65A-E15314CB3BAB}
DEFINE_GUID(CLSID_CREUnpackRoutine, 
0x1e1d0caa, 0x785d, 0x4d33, 0xa6, 0x5a, 0xe1, 0x53, 0x14, 0xcb, 0x3b, 0xab);



//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


enum
{
	US_RUN_ERROR = 0xffffffff,
	US_RUN_SUCCEED = 0,
	US_RUN_ALREADY_DUMP = 1,
	US_RUN_FINISH = 2,
	US_RUN_STEP = 3,
};

typedef struct tagUnpackVMRunStatus
{
	unsigned long dwJmpTimes ;       //跳转
	unsigned long dwDecodeTimes ;    //解密
	unsigned long dwLoopTimes ;      //循环  
	unsigned long dwPageJmpTimes ;   //内存块跳
	unsigned long dwSdmTimes ;       //自修改次数
	unsigned long dwApiTimes ;       //API数
	unsigned long dwExpTimes ;       //异常数
} STUnpackVMRunStatus, *PSTUnpackVMRunStatus ;

class IUnpackVMRunStatus
{
public:
	virtual const STUnpackVMRunStatus * GetVmRunStateLimit() = 0 ;
	virtual void SetVmRunStateLimit( const STUnpackVMRunStatus * pstStatus ) = 0 ;

	// in ENUMRUNSTATE:
	virtual void Register( unsigned long dwRunState ) = 0 ;
	virtual void UnRegister( unsigned long dwRunState ) = 0 ;

	// out ENUMGORESULT:
	virtual unsigned long GetLastGoResult() = 0 ;

	// in ENUMGORESULT:
	virtual void RegisterGoResult( unsigned long dwGR ) = 0 ;
	virtual void UnRegisterGoResult( unsigned long dwGR ) = 0 ;

	// 最终状态:
	virtual void RegisterFinal(void) = 0 ;
	virtual void UnRegisterFinal(void) = 0 ;
	virtual bool IsFinal(void) = 0 ;

	virtual void Release() = 0 ;

};

//////////////////////////////////////////////////////////////////////////
// add@2007/07/19:

// interface of scanpack component
class IREVERunState : public IUnknown
{
public:
	STDMETHOD(Query)(STUnpackVMRunStatus * pstStatus) = 0 ;
	STDMETHOD(GetPEObject)(IREPEArchive **ppPEObj) = 0 ;
	STDMETHOD(GetVMObject)(IREVM **ppVMObj) = 0 ;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IREVERunState, "{7FF45583-94E2-4158-A9F5-5CE34C92BFDB}",
			  0x7ff45583, 0x94e2, 0x4158, 0xa9, 0xf5, 0x5c, 0xe3, 0x4c, 0x92, 0xbf, 0xdb);


// class id for scan unknown packer:
// {7572E881-A889-4e37-A590-A73DEF8815CC}
DEFINE_GUID(CLSID_ScanUnkPack, 
0x7572e881, 0xa889, 0x4e37, 0xa5, 0x90, 0xa7, 0x3d, 0xef, 0x88, 0x15, 0xcc);


//////////////////////////////////////////////////////////////////////////


class IRSObjEng
{
public:
	 virtual  void Release() = 0;
};


//////////////////////////////////////////////////////////////////////////


class IRSPropertyBag : IRSObjEng
{
public:
	 virtual  bool SetProperty(const int id, const int var) = 0;
	 virtual  bool SetProperty(const int id, const long var) = 0;
	 virtual  bool SetProperty(const int id, const char var) = 0;
	 virtual  bool SetProperty(const int id, const float var) = 0;
	 virtual  bool SetProperty(const int id, const char * var) = 0;

	 virtual  bool GetProperty(const int id, int *var) = 0;
	 virtual  bool GetProperty(const int id, long *var) = 0;
	 virtual  bool GetProperty(const int id, char *var) = 0;
	 virtual  bool GetProperty(const int id, float *var) = 0;
	 virtual  bool GetProperty(const int id, const char ** var) = 0;

public:
	virtual void Release() = 0;
};


//////////////////////////////////////////////////////////////////////////


#include <map>
#include <string>


class CRSVariant
{
public:
	enum E_VAR_TYPE
	{
		TYPE_INT = 1,
		TYPE_LONG,
		TYPE_CHAR,
		TYPE_FLOAT,
		TYPE_STRING,
	};
	CRSVariant() { _vtype = 0; }
	CRSVariant(const int var) : _vtype(TYPE_INT), _i(var) {}
	CRSVariant(const long var) : _vtype(TYPE_LONG), _l(var) {}
	CRSVariant(const char var) : _vtype(TYPE_CHAR), _c(var) {}
	CRSVariant(const float var) : _vtype(TYPE_FLOAT), _f(var) {}
	CRSVariant(const char * var) : _vtype(TYPE_STRING), _str(var) {}

	CRSVariant (const CRSVariant & another) : 
	_vtype(another._vtype), _i(another._i),
	_l(another._l), _c(another._c),
	_str(another._str), _f(another._f) {}
	CRSVariant & operator = (const CRSVariant & another)
	{
		_vtype = another._vtype;
		_i=another._i;
		_l = another._l;
		_c = another._c;
		_str = another._str;
		_f = another._f;
		return *this;
	}
	int _vtype;
	int _i;
	long _l;
	char _c;
	std::string _str;
	float _f;
};

class CRSPropertyBag : public IRSPropertyBag
{
public:
	virtual ~CRSPropertyBag() {}

public:
	// set
	virtual bool SetProperty(const int id, const int var) 
	{ 
		_map[id] = CRSVariant(var);
		return true;
	}
	virtual bool SetProperty(const int id, const long var)
	{
		_map[id] = CRSVariant(var);
		return true;
	}
	virtual bool SetProperty(const int id, const char var)
	{
		_map[id] = CRSVariant(var);
		return true;
	}
	virtual bool SetProperty(const int id, const float var) 
	{
		_map[id] = CRSVariant(var);
		return true;
	}
	virtual bool SetProperty(const int id, const char * var) 
	{
		_map[id] = CRSVariant(var);
		return true;
	}

	// get
	virtual bool GetProperty(const int id,  int * var) 
	{
		if (_map.find(id) == _map.end()) return false;
		if (CRSVariant::TYPE_INT != _map[id]._vtype ) return false;
		*var = _map[id]._i;
		return true;
	}
	virtual bool GetProperty(const int id, long * var)
	{
		if (_map.find(id) == _map.end()) return false;
		if (CRSVariant::TYPE_LONG != _map[id]._vtype ) return false;
		*var = _map[id]._l;
		return true;
	}
	virtual bool GetProperty(const int id, char * var)
	{
		if (_map.find(id) == _map.end()) return false;
		if (CRSVariant::TYPE_CHAR != _map[id]._vtype ) return false;
		*var = _map[id]._c;
		return true;
	}
	virtual bool GetProperty(const int id, float * var) 
	{
		if (_map.find(id) == _map.end()) return false;
		if (CRSVariant::TYPE_FLOAT != _map[id]._vtype ) return false;
		*var = _map[id]._f;
		return true;
	}
	virtual bool GetProperty(const int id, const char ** var)
	{
		if (_map.find(id) == _map.end()) return false;
		if (CRSVariant::TYPE_STRING != _map[id]._vtype ) return false;
		*var = _map[id]._str.data();
		return true;
	}

public:
	void Release() { delete this; }

private:
	std::map<int, CRSVariant> _map;
};



#endif // #ifndef __RSENGINE_UNPACK_ROUTINE_INTERFACE_H__
