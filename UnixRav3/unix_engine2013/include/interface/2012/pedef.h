#ifndef __PE_HEADER_STRUCT_DEFINE__
#define __PE_HEADER_STRUCT_DEFINE__

#include "vmi/include/rsvm/pefmt.h"

#pragma pack(1)
typedef struct _LE_IMAGE_DATA_DIRECTORY {
	LE_UI32   VirtualAddress;
	LE_UI32   Size;
}RS_PACK_ONEBYTE LE_IMAGE_DATA_DIRECTORY, *PLEIMAGE_DATA_DIRECTORY;

typedef struct LE_IMAGE_COR20_HEADER
{
	LE_UI32					cb;
	LE_UI16					MajorRuntimeVersion;
	LE_UI16                 MinorRuntimeVersion;
	LE_IMAGE_DATA_DIRECTORY	MetaData;        
	LE_UI32                 Flags;           
	LE_UI32             	EntryPointToken;
	LE_IMAGE_DATA_DIRECTORY	Resources;
	LE_IMAGE_DATA_DIRECTORY	StrongNameSignature;
	LE_IMAGE_DATA_DIRECTORY CodeManagerTable;
	LE_IMAGE_DATA_DIRECTORY	VTableFixups;
	LE_IMAGE_DATA_DIRECTORY	ExportAddressTableJumps;
	LE_IMAGE_DATA_DIRECTORY	ManagedNativeHeader;
} RS_PACK_ONEBYTE LE_IMAGE_COR20_HEADER, *PLE_IMAGE_COR20_HEADER;
#pragma pack()

#endif