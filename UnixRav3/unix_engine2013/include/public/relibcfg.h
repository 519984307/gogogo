#ifndef __RE_LIBLOADER_CONFIG_H__
#define __RE_LIBLOADER_CONFIG_H__

//库类型和索引对象影射关系表结构
struct ST_IndexObject_Map
{
	UINT32 dwType;
	CLSID clsid;
};
//库文件
struct ST_IndexLibFile_Map
{
	UINT32	dwLibFile;		//库文件编号
	CHAR	lpszFileName[0x20];	//库文件名
};

//库存储
struct ST_IndexLibStore_MAP
{
	UINT32	dwLibType;		//病毒库类型
	UINT32	dwLibFile;		//库文件编号
};

//09PE库编译器对应的子库类型
struct ST_IndexCompilerLibType_MAP
{
	UINT16	wCompiler;			//编译器ID
	UINT32	dwFCLibType;		//FC病毒库类型
	UINT32	dwNormalLibType;	//Normal病毒库类型
};

#endif
