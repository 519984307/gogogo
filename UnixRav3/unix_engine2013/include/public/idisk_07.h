#ifndef _SCANEXEC_DISKOBJECT_INTERFACE_H_
#define _SCANEXEC_DISKOBJECT_INTERFACE_H_


typedef enum _RISING_MEDIA_TYPE
{
	RISING_Unknown,                // Format is unknown
	RISING_F5_1Pt2_512,            // 5.25", 1.2MB,  512 bytes/sector
	RISING_F3_1Pt44_512,           // 3.5",  1.44MB, 512 bytes/sector
	RISING_F3_2Pt88_512,           // 3.5",  2.88MB, 512 bytes/sector
	RISING_F3_20Pt8_512,           // 3.5",  20.8MB, 512 bytes/sector
	RISING_F3_720_512,             // 3.5",  720KB,  512 bytes/sector
	RISING_F5_360_512,             // 5.25", 360KB,  512 bytes/sector
	RISING_F5_320_512,             // 5.25", 320KB,  512 bytes/sector
	RISING_F5_320_1024,            // 5.25", 320KB,  1024 bytes/sector
	RISING_F5_180_512,             // 5.25", 180KB,  512 bytes/sector
	RISING_F5_160_512,             // 5.25", 160KB,  512 bytes/sector
	RISING_RemovableMedia,         // Removable media other than floppy
	RISING_FixedMedia,             // Fixed hard disk media
	RISING_F3_120M_512,            // 3.5", 120M Floppy
	RISING_F3_640_512,             // 3.5" ,  640KB,  512 bytes/sector
	RISING_F5_640_512,             // 5.25",  640KB,  512 bytes/sector
	RISING_F5_720_512,             // 5.25",  720KB,  512 bytes/sector
	RISING_F3_1Pt2_512,            // 3.5" ,  1.2Mb,  512 bytes/sector
	RISING_F3_1Pt23_1024,          // 3.5" ,  1.23Mb, 1024 bytes/sector
	RISING_F5_1Pt23_1024,          // 5.25",  1.23MB, 1024 bytes/sector
	RISING_F3_128Mb_512,           // 3.5" MO 128Mb   512 bytes/sector
	RISING_F3_230Mb_512,           // 3.5" MO 230Mb   512 bytes/sector
	RISING_F8_256_128,             // 8",     256KB,  128 bytes/sector
	RISING_F3_200Mb_512,           // 3.5",   200M Floppy (HiFD)
	RISING_F3_240M_512,            // 3.5",   240Mb Floppy (HiFD)
	RISING_F3_32M_512              // 3.5",   32Mb Floppy
} RISING_MEDIA_TYPE;


// The following structure is returned on an IOCTL_DISK_GET_DRIVE_GEOMETRY
// request and an array of them is returned on an IOCTL_DISK_GET_MEDIA_TYPES
// request.
typedef struct _RISING_DISK_GEOMETRY
{
	ULONGLONG Cylinders;
	RISING_MEDIA_TYPE MediaType;
	DWORD TracksPerCylinder;
	DWORD SectorsPerTrack;
	DWORD BytesPerSector;
} _RISING_DISK_GEOMETRY;


#pragma pack(1)
typedef struct tagSTPARTBL
{
	BYTE byActiveFlag;    //0x80 or 0x00
	BYTE abyHSCBeg[3];    //Partition Begin Header, Sector, Cylinder
	BYTE byPartitionFlag; //0x00 : Partition not used
						  //0x05, 0x0F : Extend Partition
						  //0x06 : FAT16
						  //0x0B, 0x1B, 0x0C, 0x1C : FAT32
						  //0x07 : NTFS
	BYTE abyHSCEnd[3];    //Partition End Header, Sector, Cylinder
	BYTE abySectorBeg[4]; //Partition Begin Sector
	BYTE abySectors[4];   //Partition Sectors
}RS_PACK_ONEBYTE STPARTBL;
#pragma pack()

// boot stream property:
// VT_UI2
DEFINE_GUID(PID_DISK_BOOTTYPE,		0xac052f7d, 0x89c5, 0x433a, 0x8e, 0x00, 0xd4, 0xe0, 0xa6, 0x12, 0xdb, 0x61);
// DEFINE_GUID(PID_DISK_DEVICEPARAM,	0x424afe55, 0xaccc, 0x4073, 0xb5, 0x80, 0x9c, 0xef, 0x9d, 0xe4, 0x28, 0x42);


class IDisk_07 : public IUnknown
{
public:
	//返回值：0x00 或 0x80
	STDMETHOD_(int, GetPCBIOSDiskNumber)() = 0;
	//取系统参数
	STDMETHOD_(BOOL, GetPara)(_RISING_DISK_GEOMETRY *disk_geometry) = 0;
	//返回虚拟执行的起始扇区号：
	STDMETHOD_(DWORD, GetBootSectorNumber)() = 0;
	//当前盘是否只读
	STDMETHOD_(BOOL, IsReadOnly)() = 0;
	//磁盘对象读
	STDMETHOD_(BOOL, Read)(ULONGLONG Offset, void *pBuffer, DWORD nNumberOfBytesToRead) = 0;
	//磁盘对象写
	STDMETHOD_(BOOL, Write)(ULONGLONG Offset, const void *pBuffer, DWORD nNumberOfBytesToWrite) = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IDisk_07, "{C6FF369F-7A57-4f5f-8182-501224095ABF}",
0xc6ff369f, 0x7a57, 0x4f5f, 0x81, 0x82, 0x50, 0x12, 0x24, 0x9, 0x5a, 0xbf);


class IDiskEx_07 : public IUnknown
{
public:
	//0x00 A盘, 0x01 B盘, 0x80 硬盘1, 0x81 硬盘2, 0x82 硬盘3, 0x83 硬盘4:
	STDMETHOD_(BOOL, InitDisk)(DWORD nDeviceNo, IREStream *pcFile = NULL) = 0;
	STDMETHOD_(BOOL, NextBoot)() = 0;
	STDMETHOD_(BOOL, FlushBuffer)() = 0;
	STDMETHOD_(BOOL, VerifyBoot)(BYTE *pbyBoot) = 0;
	STDMETHOD_(BOOL, IsMainBoot)() = 0;
	STDMETHOD_(BYTE*, GetOriginalBoot)() = 0;
	STDMETHOD_(BOOL, RestoreOriginalBoot)() = 0;
	STDMETHOD_(BOOL, RestoreStandardBoot)() = 0;
	STDMETHOD_(DWORD, GetDeviceNumber)() = 0;
	STDMETHOD_(void, LockDiskReadOnly)(BOOL bReadOnly) = 0;
	STDMETHOD_(BOOL, IsBootImage)() = 0;
public:
	RE_DECLARE_IID;
};
RE_DEFINE_IID(IDiskEx_07, "{D3613C71-0880-4a0e-8462-D0E90FE71042}",
0xd3613c71, 0x880, 0x4a0e, 0x84, 0x62, 0xd0, 0xe9, 0xf, 0xe7, 0x10, 0x42);


#endif //#ifndef _SCANEXEC_DISKOBJECT_INTERFACE_H_
